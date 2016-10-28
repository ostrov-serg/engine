
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
#define SSH_NEW_SDECL(cls, SIZE)		static MemArray<cls, SIZE>* get_MemArray##cls() { static MemArray<cls, SIZE> memarray; return &memarray; } \
										void* operator new(size_t sz) {return (void*)get_MemArray##cls()->Alloc();} \
										void operator delete(void* p) { get_MemArray##cls()->Free((cls*)p); } \
										static void reset() { get_MemArray##cls()->Reset(); } \
										static bool valid() { return get_MemArray##cls()->Valid(); } \
										static size_t count() { return get_MemArray##cls()->Count(); } \
										static struct MemArray<cls, SIZE>::BlockFix* root() { return get_MemArray##cls()->Root(); }
#define SSH_NEW_DDECL(cls, SIZE)		static MemArray<cls, SIZE>* get_MemArray##cls() { static MemArray<cls, SIZE>* memarray(nullptr); if(!memarray) memarray = new MemArray<cls, SIZE>; return memarray; } \
										void* operator new(size_t sz) {return (void*)get_MemArray##cls()->Alloc();} \
										void operator delete(void* p) { get_MemArray##cls()->Free((cls*)p); } \
										static void reset() { get_MemArray##cls()->Reset(); } \
										static bool valid() { return get_MemArray##cls()->Valid(); } \
										static size_t count() { return get_MemArray##cls()->Count(); } \
										static struct MemArray<cls, SIZE>::BlockFix* root() { return get_MemArray##cls()->Root(); }
#define SSH_CAST(v)						static_cast<ssh_u>(v)
#define SSH_FFL							__FUNCTIONW__, __FILEW__, __LINE__
#define SSH_RTTI_DECL(cls)				class RTTI_##cls : public RTTI \
										{ \
										public: \
											RTTI_##cls() {name = L#cls;} \
											virtual RTTI* create() override {return (RTTI*)new cls();} \
										}; \
										static RTTI_##cls cls##_rtti;
#define SSH_RTTI_IMPL(cls)				cls::RTTI_##cls cls::cls##_rtti;

#define SSH_CLAMP(val, low, high)		(val < low ? low : (val > high ? high : val))
#define SSH_THROW(msg, ...)				throw Exception(SSH_FFL, msg, __VA_ARGS__)
#define SSH_LOG(msg, ...)				log->add(Log::mInfo, SSH_FFL, msg, __VA_ARGS__)
#define SSH_FAULT(code, except)			log->fault(code, SSH_FFL, except)
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
#define SSH_TYPE						0
#define SSH_PTR							1
#define SSH_

// индексы синглонов

#define SSH_SINGL_SQL					0
#define SSH_SINGL_ARCHIVE				1
#define SSH_SINGL_KEYBOARD				2
#define SSH_SINGL_MOUSE					3
#define SSH_SINGL_GAMEPAD				4
#define SSH_SINGL_LOG					5
#define SSH_SINGL_STACKTRACE			6
#define SSH_SINGL_HELPER				7
#define SSH_SINGL_						8

#define cp_ansi							L"windows-1251"
#define cp_utf							L"utf-16le"

#define ID_LIST_BASE					100
#define ID_ATTACH_STK_MAIL				101
#define ID_ATTACH_MAIL					102
#define ID_RECIPIENTS_MAIL				103
#define ID_TREE_XML						104
#define ID_RESOURCES_ARCHIVE			105
#define ID_STK_MOUSE					106
#define ID_DLL_MODULES					107
