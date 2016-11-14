
#pragma once

// мои типы данных

typedef long long						ssh_l;
typedef unsigned long long				ssh_u;
typedef unsigned char					ssh_b;
typedef unsigned short					ssh_w;
typedef unsigned long					ssh_d;
typedef unsigned int					ssh_i;
typedef const char*						ssh_ccs;
typedef const wchar_t*					ssh_cws;
typedef char							ssh_cs;
typedef char*							ssh_pcs;
typedef wchar_t							ssh_ws;
typedef wchar_t*						ssh_pws;

#define SSH_SDK_VERSION					6
// макросы

#define SSH_MEMZERO(dst, len)			memset(dst, 0, len)
#define SSH_DEL(p)						{if(p) delete p; p = nullptr;}
#define SSH_DEL_A(p)					(if(p) delete [] p; p = nullptr;)
#define SSH_REL(p)						{if(p) p->release(); p = nullptr;}
#define SSH_DYNCREATE(cls)				public:\
										void* operator new(ssh_u sz, cls** obj, const String& name) \
										{\
											return (cls*)Base::operator new(sz, (Base**)obj, name, String(L#cls)); \
										}
#define SSH_NEW_DECL(cls, SIZE)			static MemArray<cls, SIZE>* get_MemArray##cls() { static MemArray<cls, SIZE> memarray; return &memarray; } \
										void* operator new(size_t sz) {return (void*)get_MemArray##cls()->Alloc();} \
										void operator delete(void* p) { get_MemArray##cls()->Free((cls*)p); } \
										static void reset() { get_MemArray##cls()->Reset(); }
#define SSH_IS_PTR(t)					(bool)(std::is_pointer<t>() && !std::is_same<t, HMODULE>())
#define SSH_CAST(v1, v2)				static_cast<v1>(v2)
#define SSH_FFL							__FUNCTIONW__, __FILEW__, __LINE__

#define ssh_mem							ssh::MemMgr::instance()
#define ssh_log							ssh::Log::instance()
#define ssh_mse							ssh::Mouse::instance()
#define ssh_key							ssh::Keyboard::instance()
#define ssh_pad							ssh::Gamepad::instance()
#define ssh_trc							ssh::StackTrace::instance()

#define SSH_CLAMP(val, low, high)		(val < low ? low : (val > high ? high : val))
#define SSH_THROW(msg, ...)				throw Exception(SSH_FFL, msg, __VA_ARGS__)
#define SSH_FAULT(code, except)			ssh_mem->fault(code, SSH_FFL, except)

#define SSH_PURE_TYPE1(T)				std::remove_all_extents_t<T>
#define SSH_PURE_TYPE2(T)				std::remove_pointer_t<SSH_PURE_TYPE1(T)>
#define SSH_IS_STRING(T)				std::is_same<SSH_PURE_TYPE2(T), String>()
#define SSH_IS_BOOL(T)					std::is_same<SSH_PURE_TYPE2(T), bool>()
#define SSH_IS_LITERAL(T)				std::is_same<SSH_PURE_TYPE2(T), ssh_ws>() | std::is_same<SSH_PURE_TYPE2(T), ssh_cs>()
#define SSH_PURE_TYPE1_D(T)				std::remove_all_extents_t<decltype(T)>
#define SSH_PURE_TYPE2_D(T)				std::remove_pointer_t<SSH_PURE_TYPE1_D(T)>
#define SSH_IS_STRING_D(T)				std::is_same<SSH_PURE_TYPE2_D(T), String>()
#define SSH_IS_BOOL_D(T)				std::is_same<SSH_PURE_TYPE2_D(T), bool>()
#define SSH_IS_LITERAL_D(T)				(std::is_same<SSH_PURE_TYPE2_D(T), ssh_ws>() | std::is_same<SSH_PURE_TYPE2_D(T), ssh_cs>())

// константы
#define SSH_EPSILON						(1e-06)
#define SSH_EPSILON2					(SSH_EPSILON * SSH_EPSILON)
#define SSH_PI							3.14f
#define SSH_HALF_PI						(0.5f * SSH_PI)
#define SSH_RGBA_A(rgba)				((rgba) >> 24)
#define SSH_RGBA_B(rgba)				(((rgba) >> 16) & 0xff)
#define SSH_RGBA_G(rgba)				(((rgba) >> 8) & 0xff)
#define SSH_RGBA_R(rgba)				((rgba) & 0xff)
#define SSH_RGBA(r, g, b, a)			((((a) << 24) | ((b) << 16) | ((g) << 8) | (r)))
#define SSH_DEG2RAD						(SSH_PI / 180.0f)
#define SSH_RAD2DEG						(180.0f / SSH_PI)
#define SSH_

#define cp_ansi							L"windows-1251"
#define cp_utf16						L"utf-16le"
#define cp_utf8							L"utf-8"
