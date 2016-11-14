
#pragma once

extern "C"
{
	void*	asm_ssh_wton(ssh_cws str, ssh_u radix, ssh_ws* end);
	ssh_cws	asm_ssh_ntow(const void* num, ssh_u radix, ssh_ws* end = nullptr);
};

namespace ssh
{
	ssh_u SSH ssh_hash(ssh_cws wcs);
	enum class Radix { _dec, _bin, _oct, _hex, _dbl, _flt, _bool };

	class SSH String
	{
		friend class Base;
	public:
		// конструкторы
		String() { init(); }
		String(std::nullptr_t) { }
		String(String&& str) { buf = str.buf; str.init(); }
		String(ssh_cws cws, ssh_l len = -1);
		String(ssh_ws* ws, ssh_l len = -1) : String((ssh_cws)ws, len) {}
		String(ssh_ccs ccs, ssh_l len = -1);
		String(const String& str) { init(); *this = str; }
		String(ssh_ws ws, ssh_l rep);
		String(const float v) { init(); num(v, Radix::_flt); }
		String(const double v) { init(); num(v, Radix::_dbl); }
		template <typename T> String(const T& v, Radix r = Radix::_dec) { init(); num(v, r); }
		// деструктор
		~String() { empty(); }
		// привидение типа
		operator ssh_cws() const { return buf; }
		operator double() const { return to_num<double>(0, Radix::_dbl); }
		operator float() const { return to_num<float>(0, Radix::_flt); }
		operator bool() const { return to_num<bool>(0, Radix::_bool); }
		template<typename T> operator T() const { return to_num<T>(0, Radix::_dec); }
		template <typename T> T to_num(ssh_l idx, Radix R = Radix::_dec) const { return *(T*)asm_ssh_wton(buf + idx, (ssh_u)R, nullptr); }
		template <typename T> void num(const T& v, Radix R = Radix::_dec) { ssh_u tmp(0); *(T*)&tmp = v; *this = asm_ssh_ntow(&tmp, (ssh_u)R); }
		// вернуть по индексу
		ssh_ws operator[](ssh_u idx) const { return get(idx); }
		// операторы сравнения
		friend bool operator == (const String& str1, const String& str2) { return (str1.hash() == str2.hash()); }
		friend bool operator == (const String& str, ssh_cws wcs) { return (wcs ? (wcscmp(str, wcs) == 0) : false); }
		friend bool operator == (ssh_cws wcs, const String& str) { return (wcs ? (wcscmp(wcs, str) == 0) : false); }
		friend bool operator != (const String& str1, const String& str2) { return !(operator == (str1, str2)); }
		friend bool operator != (const String& str, ssh_cws wcs) { return !(operator == (str, wcs)); }
		friend bool operator != (ssh_cws wcs, const String& str) { return !(operator == (wcs, str)); }
		// операторы присваивания
		const String& operator = (const String& str) { return make(str, str.length()); }
		const String& operator = (String&& str) { empty(); buf = str.buf; str.init(); return *this; }
		const String& operator = (ssh_ws ws) { return make((ssh_cws)&ws, 1); }
		const String& operator = (ssh_cws wcs) { return make(wcs, wcs ? wcslen(wcs) : 0); }
		// операторы контакенции
		const String& operator += (const String& str) { return add(str, str.length()); }
		const String& operator += (ssh_ws ws) { return add((ssh_cws)&ws, 1); }
		const String& operator += (ssh_cws wcs) { return (wcs ? add(wcs, wcslen(wcs)) : *this); }
		// дружественные операторы
		friend String operator + (ssh_ws ws, const String& str) { return String::add((ssh_cws)&ws, 1, str, str.length()); }
		friend String operator + (ssh_cws wcs, const String& str) { return String::add(wcs, wcs ? wcslen(wcs) : 0, str, str.length()); }
		friend String operator + (const String& str1, const String& str2) { return String::add(str1, str1.length(), str2, str2.length()); }
		friend String operator + (const String& str, ssh_ws ws) { return String::add(str, str.length(), (ssh_cws)&ws, 1); }
		friend String operator + (const String& str, ssh_cws wcs) { return String::add(str, str.length(), wcs, wcs ? wcslen(wcs) : 0); }
		// методы
		ssh_ws* buffer(ssh_u offs = 0) const { return buf + offs; }
		ssh_l length() const { return data()->len; }
		ssh_ws get(ssh_u idx) const { return (idx < (ssh_u)length() ? buf[idx] : L'0'); }
		void set(ssh_u idx, ssh_ws ws) { if(idx < (ssh_u)length()) buf[idx] = ws; }
		void empty() { if(!is_empty()) { delete data(); init(); } }
		bool is_empty() const { return (buf == (ssh_ws*)((ssh_cs*)_empty + sizeof(STRING_BUFFER))); }
		bool compare(ssh_cws wcs) const { return (_wcsicmp(buf, wcs) == 0); }
		ssh_u hash() const { return data()->hash; }
		// модификация
		const String& load(ssh_u id);
		const String& lower() { _wcslwr(buf); return *this; }
		const String& upper() { _wcsupr(buf); return *this; }
		const String& reverse() { _wcsrev(buf); return *this; }
		const String& replace(ssh_cws _old, ssh_cws _new);
		const String& replace(ssh_ws _old, ssh_ws _new);
		const String& replace(ssh_cws* _old, ssh_cws _new);
		const String& remove(ssh_cws wcs);
		const String& remove(ssh_ws ws);
		const String& remove(ssh_l idx, ssh_l len = -1);
		const String& insert(ssh_l idx, ssh_cws wcs);
		const String& insert(ssh_l idx, ssh_ws ws);
		const String& trim() { return trim(L" "); }
		const String& trim(ssh_cws wcs) { trim_left(wcs); return trim_right(wcs); }
		const String& trim_left(ssh_cws wcs);
		const String& trim_right(ssh_cws wcs);
		// форматирование
		static String fmt(ssh_cws pattern, ...);
		static String fmt(ssh_cws pattern, va_list argList);
		// поиск
		ssh_l find(ssh_cws wcs, ssh_l idx = 0) const { return (idx < length() ? (wcsstr(buf + idx, wcs) - buf) : -1); }
		ssh_l find(ssh_ws ws, ssh_l idx = 0) const { return (idx < length() ? (wcschr(buf + idx, ws) - buf) : -1); }
		ssh_l find_rev(ssh_ws ws) const { return (ssh_l)(wcsrchr(buf, ws) - buf); }
		String substr(ssh_l idx, ssh_l len = -1) const;
		String left(ssh_l idx) const { return substr(0, idx); }
		String right(ssh_l idx) const { return substr(length() - idx); }
	protected:
		struct STRING_BUFFER
		{
			// длина данных
			int len;
			// длина буфера
			int len_buf;
			// хэш
			ssh_u hash;
			// вернуть указатель на буфер
			ssh_ws* data() { return (ssh_ws*)(this + 1); }
			// пересчитать хэш
			void update() { hash = ssh_hash(data()); }
		};
		static String add(ssh_cws wcs1, ssh_l len1, ssh_cws wcs2, ssh_l len2);
		void _replace(ssh_cws _old, ssh_cws _new);
		void init() { buf = (ssh_ws*)((ssh_cs*)_empty + sizeof(STRING_BUFFER)); }
		bool alloc(ssh_l size, bool is_copy);
		const String& add(ssh_cws wcs, ssh_l len);
		const String& make(ssh_cws wcs, ssh_l len);
		STRING_BUFFER* data() const { return ((STRING_BUFFER*)buf) - 1; }
		ssh_ws* buf;
	private:
		static ssh_cws _empty;
	};
}
