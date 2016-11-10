
#include "stdafx.h"


//typedef void* (CALLBACK* __cnv_open)(ssh_cws to, ssh_cws from);
typedef ssh_cs* (CALLBACK* __ext_undname)(char* out, const char* name, int len_out, unsigned long flags);



ssh_u ssh_dll_proc(ssh_cws dll, ssh_ccs nm)
{
	HMODULE hdll;
	if(!(hdll = LoadLibrary(dll))) return 0;
	return (ssh_u)GetProcAddress(hdll, nm);
}

class tp : public Serialize
{
public:
//	tp() : x(0), y(0) {}
	SSH_ENUM(_enum, _one = 1, _two = 2);
	virtual ~tp() { x = y = nullptr; }
	struct stk2 { String str[3]; ssh_cws _cws; };
	struct stk { double xx, *yy[4]; stk2 _stk2; };
	virtual SCHEME* get_scheme() const override
	{
		auto& reflector = ssh::EnumReflector::get<_enum>();
		for(int i = 0; i < reflector.count(); i++)
		{
			String nm = reflector.at(i).name;
			int val = reflector.at(i).value;
		}
		SCHEME_BEGIN(tp)
			SCHEME_NODE_BEGIN(tp, _stk)
				SCHEME_VAR(stk, xx, L"null", 0, &reflector)
					SCHEME_NODE_BEGIN(stk, _stk2)
						SCHEME_VAR(stk2, str, L"null", 0, nullptr)
						SCHEME_VAR(stk2, _cws, L"null", 0, nullptr)
					SCHEME_NODE_END()
				SCHEME_VAR(stk, yy, L"null", 0, nullptr)
			SCHEME_NODE_END()
			SCHEME_VAR(tp, x, L"null", 0, nullptr)
			SCHEME_VAR(tp, y, L"null", 0, nullptr)
		SCHEME_END(tp);
	}
	float* x, *y;
	stk _stk;
};

class bs
{
public:
	bs() = delete;
protected:
	virtual ~bs() {}
private:
	String nm;
};

class bs1 : public Base
{
	SSH_DYNCREATE(bs1);
public:
	bs1() { x = 0; }
	int x;
	String str;
private:
	Base::Base;
};
SSH_ENUM_NS(serg, _1 = 1, _2 = 2, _3 = 4, _4 = 8, _5 = 16, _6 = 32, _7 = 32768, _8 = 16384)
int main() noexcept
{
	int dst[8];
	String str(L"_1|_7|_8,_2,_1|_3,_8,_7,_7|_8");
	String str1(__FILEW__);

	ssh_u num = ssh_system_value(SystemInfo::PLATFORM, CpuCaps::AES);
	Buffer buf(ssh_convert(L"utf-8", L"Шаталов Сергей"));//utf-16le
	str = ssh_convert(L"utf-8", buf, 0);
	str = ssh_base64(buf);
	Buffer buf1(ssh_base64(str));
	str1 = ssh_convert(L"utf-8", buf1, 0);
	for(ssh_u i = 0; i < 100; i++)
	{
		num = ssh_rand(10, 20);
	}
	num = ssh_system_value(SystemInfo::CPU_CAPS, CpuCaps::AVX512CD);
	num = ssh_system_value(SystemInfo::TOTAL_MEMORY, CpuCaps::AES);
	str = ssh_num_volume(num);
	num = ssh_system_value(SystemInfo::PHYSICAL_MEMORY, CpuCaps::AES);
	str = ssh_num_volume(num);
	num = ssh_system_value(SystemInfo::CPU_SPEED, CpuCaps::AES);
	str = ssh_system_paths(SystemInfo::COMP_NAME);
	str = ssh_system_paths(SystemInfo::PROG_FOLDER);
	str = ssh_system_paths(SystemInfo::WORK_FOLDER);
	str = ssh_system_paths(SystemInfo::TEMP_FOLDER);
	str = ssh_system_paths(SystemInfo::USER_FOLDER);
	str = ssh_system_paths(SystemInfo::PROG_NAME);
	str = ssh_system_paths(SystemInfo::USER_NAME);
	str = ssh_system_paths(SystemInfo::CUSTOM);
	ssh_u l = str1.length();
	str = ssh_path_in_range(str1, 42);
	l = str.length();
	num = 0x10;
	str = ssh_num_volume(num);
	GUID _g = ssh_make_guid(nullptr);
	str = ssh_make_guid(_g);
	GUID _g1 = ssh_make_guid((ssh_cws)str);
	str = ssh_gen_name(L"value = ", false);
	str = ssh_gen_name(L"value = ", true);
	str = ssh_file_ext(str1, false);
	str = ssh_file_name(str1);
	str = ssh_file_path(str1);
	str = ssh_file_title(str1);
	str = ssh_file_path_title(str1);
	str = ssh_slash_path(str);
	str = ssh_slash_path(str);
	ssh_u vec[20];
	ssh_u c = ssh_split(L'\\', str, vec, 20);
	ssh_explode(L",", str, dst, 8, 0, &EnumReflector::get<serg>(), Radix::_dec);
	str1 = ssh_make_hex_string((ssh_d*)dst, 8, str, true, true);
	String ret(ssh_implode(L",", dst, 8, L"null", &EnumReflector::get<serg>(), false, Radix::_dec));
	tp t;
	Serialize::SCHEME* _sc = t.get_scheme();
	bs1* b, *b1;
	new(&b, L"fff") bs1();
	new(&b1, L"fff1") bs1();
	b1->release();
	b1->add_ref();
	b1->release();
	b->release();
	return 0;
}
