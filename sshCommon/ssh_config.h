
#pragma once

#include <sshCommon\ssh_defs.h>

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <iostream>
//#include <tchar.h>
#include <time.h>
// intrisincs
#include <intrin.h>
#include <xmmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>
#include <io.h>
#include <new.h>
// CSIDL_LOCAL_XXX
#include <Shlobj.h>
#include <openssl\ssl.h>
#include <openssl\md5.h>
//#include <Lmcons.h>
#include <sshCommon\ssh_regx.h>
#include <sshCommon\mysql\mysql.h>
#include <sshCommon\ssh_cnv.h>

#include <Xinput.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")
#pragma comment(lib, "libmysql.lib")

#pragma warning(disable:4996)
#pragma warning(disable:4595)
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
	#define SSH_LOG(msg, ...)
#else
	#ifdef SSH_DLL
		#pragma comment(lib, "sshSTDd.lib")
	#endif
	#define SSH_ASSERT(express, msg, ...)		if(!(express)) Log::add(Log::assert, __FILEW__, __LINE__, msg, #express, __VA_ARGS__);
	#define SSH_TRACE							Tracer tracer(__FUNCSIGW__);
	#define SSH_LOG(msg)						ssh_log->add(Log::info, __FUNCTIONW__, __FILEW__, __LINE__, msg)
#endif
#define SSH_MSG(msg)						ssh_log->add_msg(msg, false)

#define MAIL_PASS				L"IfnfkjdCthutq"
