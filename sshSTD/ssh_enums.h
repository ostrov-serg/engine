
#pragma once

namespace ssh
{
	#define SSH_ENUM( enumName, ... ) SSH_ENUM_DETAIL_MAKE( class, enumName, __VA_ARGS__ )
	#define SSH_ENUM_NS( enumName, ... ) SSH_ENUM_DETAIL_MAKE( namespace, enumName, __VA_ARGS__ )
	#define SSH_ENUM_DETAIL_SPEC_namespace extern "C"{} inline
	#define SSH_ENUM_DETAIL_SPEC_class friend
	#define SSH_ENUM_DETAIL_STR(x) L#x
	#define SSH_ENUM_DETAIL_MAKE(spec, enumName, ...) enum enumName:int { __VA_ARGS__ };		\
    SSH_ENUM_DETAIL_SPEC_##spec const ::EnumReflector& _detail_reflector_(enumName)				\
    {																							\
        static const ::EnumReflector _reflector( []{											\
            static int _detail_sval;															\
            _detail_sval = 0;																	\
            struct _detail_val_t																\
            {																					\
                _detail_val_t(const _detail_val_t& rhs) : _val(rhs) { _detail_sval = _val + 1; }\
                _detail_val_t(int val)                  : _val(val) { _detail_sval = _val + 1; }\
                _detail_val_t()                 : _val(_detail_sval){ _detail_sval = _val + 1; }\
																								\
                _detail_val_t& operator=(const _detail_val_t&) { return *this; }				\
                _detail_val_t& operator=(int) { return *this; }									\
                operator int() const { return _val; }											\
                int _val;																		\
            } __VA_ARGS__;																		\
            const int _detail_vals[] = { __VA_ARGS__ };											\
            return ::EnumReflector( _detail_vals, sizeof(_detail_vals)/sizeof(int), L#enumName, \
								SSH_ENUM_DETAIL_STR((__VA_ARGS__))  );							\
        }() );																					\
        return _reflector;																		\
    }


	class EnumReflector
	{
	public:
		template<typename EnumType> static const EnumReflector& get(EnumType val = EnumType()) { return _detail_reflector_(val); }
		class Enumerator
		{
		public:
			const String& name() const { return _er._data->values[_index].name; }
			int value() const { return _er._data->values[_index].value; }
			int index() const { return _index; }
			const EnumReflector& Reflector() const { return _er; }
			bool operator!=(const Enumerator& rhs) const { return _index != rhs._index; }
			Enumerator& operator++() { ++_index; return *this; }
			const Enumerator& operator*() const { return *this; }
		private:
			friend class EnumReflector;
			Enumerator(const EnumReflector& er, int index) : _er(er), _index(index) {}
			const EnumReflector& _er;
			int _index;
		};
		EnumReflector(const int*, int, ssh_cws, ssh_cws);
		EnumReflector(EnumReflector&& rhs) : _data(rhs._data) { rhs._data = nullptr; }
		~EnumReflector() { delete _data; }
		int count() const { return (int)_data->count; }
		Enumerator find(const String& name) const
		{
			for(int i = 0; i < (int)_data->count; ++i)
				if(_data->values[i].name == name) return at(i);
			return at(count());
		}
		Enumerator find(int value) const
		{
			for(int i = 0; i < (int)_data->count; ++i)
				if(_data->values[i].value == value) return at(i);
			return at(count());
		}
		Enumerator at(int index) const { return Enumerator(*this, index); }
		Enumerator operator[](int index) const { return at(index); }
	private:
		struct Private
		{
			struct Enumerator
			{
				String name;
				int value;
			};
			Enumerator values[32];
			String enumName;
			int count;
		};
		Private* _data;
	};

	static bool IsIdentChar(ssh_ws c)
	{
		return (c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') || (c >= L'0' && c <= L'9') || (c == L'_');
	}

	EnumReflector::EnumReflector(const int* vals, int count, ssh_cws name, ssh_cws body) : _data(new Private)
	{
		_data->enumName = name;
		_data->count = 0;
		enum states { state_start, state_ident, state_skip, } state = state_start;
		++body;
		ssh_cws ident_start(nullptr);
		int value_index = 0, level = 0;
		for(;;)
		{
			switch(state)
			{
				case state_start:
					if(IsIdentChar(*body)) { state = state_ident; ident_start = body; }
					++body;
					break;
				case state_ident:
					if(!IsIdentChar(*body))
					{
						state = state_skip;
						_data->values[value_index].name = String(ident_start, body - ident_start);
						_data->values[value_index].value = vals[value_index];
						_data->count++;
						++value_index;
					}
					else ++body;
					break;
				case state_skip:
					if(*body == L'(') level++;
					else if(*body == L')')
					{
						if(level == 0) return;
						--level;
					}
					else if(level == 0 && *body == L',') state = state_start;
					++body;
			}
		}
	}
}