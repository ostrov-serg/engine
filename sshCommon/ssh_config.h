
#pragma once

#include <sshCommon\ssh_defs.h>

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <time.h>
// intrisincs
#include <intrin.h>
#include <xmmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>
#include <io.h>
#include <fcntl.h>
#include <new.h>
// CSIDL_LOCAL_XXX
#include <Shlobj.h>
//#include <sys\stat.h>
//#include <sys\types.h>
//#include <openssl\ssl.h>
//#include <openssl\md5.h>
//#include <Lmcons.h>
#include <sshCommon\ssh_regex.h>
#include <sshCommon\mysql\mysql.h>
#include <sshCommon\ssh_cnv.h>
#include <sshCommon\ssh_ext.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libmysql.lib")

#pragma warning(disable:4996)
#pragma warning(disable:4251)
#pragma warning(disable:4291)
#pragma warning(disable:4838)

#ifdef SSH_EXPORTS
	#define SSH		__declspec(dllexport)
#else
	#define SSH		__declspec(dllimport)
#endif

#ifdef NDEBUG
	#ifdef SSH_DLL
		#pragma comment(lib, "sshSTD.lib")
	#endif
	#define SSH_ASSERT(express, msg, ...)
	#define SSH_TRACE
#else
	#ifdef SSH_DLL
		#pragma comment(lib, "sshSTDd.lib")
	#endif
	#define SSH_ASSERT(express, msg, ...)		if(!(express)) Log::add(Log::mAssert, #express, __FILEW__, __LINE__, msg, __VA_ARGS__);
	#define SSH_TRACE							Tracer tracer(__FUNCTIONW__, __FILEW__, __LINE__);
#endif

#define MAIL_PASS				L"IfnfkjdCthutq"
