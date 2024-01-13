// Borland C++ Builder
// Copyright (c) 1995, 1998 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'ip_misc.pas' rev: 3.00

#ifndef ip_miscHPP
#define ip_miscHPP
#include <Classes.hpp>
#include <WinSock.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <SysInit.hpp>
#include <System.hpp>

//-- user supplied -----------------------------------------------------------

namespace Ip_misc
{
//-- type declarations -------------------------------------------------------
typedef Byte ta_8u[65531];

enum t_encoding { uuencode, base64, mime };

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool tcpip_ready;
#define INVALID_IP_ADDRESS (Shortint)(-1)
extern PACKAGE int __fastcall posn(const System::AnsiString s, const System::AnsiString t, int count
	);
extern PACKAGE int __fastcall poscn(char c, const System::AnsiString s, int n);
extern PACKAGE System::AnsiString __fastcall filename_of(const System::AnsiString s);
extern PACKAGE int __fastcall TimeZoneBias(void);
extern PACKAGE Classes::TStringList* __fastcall encode_base64(Classes::TStream* data);
extern PACKAGE Classes::TMemoryStream* __fastcall decode_base64(Classes::TStringList* source);
extern PACKAGE System::AnsiString __fastcall eight2seven_quoteprint(const System::AnsiString s);
extern PACKAGE System::AnsiString __fastcall eight2seven_german(const System::AnsiString s);
extern PACKAGE System::AnsiString __fastcall seven2eight_quoteprint(const System::AnsiString s);
extern PACKAGE System::AnsiString __fastcall my_hostname();
extern PACKAGE int __fastcall my_ip_address(void);
extern PACKAGE System::AnsiString __fastcall internet_date(System::TDateTime date);
extern PACKAGE System::AnsiString __fastcall ip2string(int ip_address);
extern PACKAGE System::AnsiString __fastcall address_from(const System::AnsiString s, int count);
extern PACKAGE int __fastcall lookup_hostname(const System::AnsiString hostname);
extern PACKAGE System::AnsiString __fastcall resolve_hostname(int ip);

}	/* namespace Ip_misc */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Ip_misc;
#endif
//-- end unit ----------------------------------------------------------------
#endif	// ip_misc
