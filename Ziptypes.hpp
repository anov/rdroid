// Borland C++ Builder
// Copyright (c) 1995, 1998 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'ziptypes.pas' rev: 3.00

#ifndef ziptypesHPP
#define ziptypesHPP
#include <SysInit.hpp>
#include <System.hpp>

//-- user supplied -----------------------------------------------------------

namespace Ziptypes
{
//-- type declarations -------------------------------------------------------
typedef int nWord;

typedef char buftype[262145];

typedef char TDirtype[260];

#pragma pack(push, 1)
struct TZipRec
{
	buftype *buf;
	Word bufsize;
	Word localstart;
	int Time;
	int Size;
	int CompressSize;
	int headeroffset;
	char FileName[260];
	Word PackMethod;
	Byte Attr;
} ;
#pragma pack(pop)

struct TReportRec;
typedef TReportRec *pReportRec;

#pragma pack(push, 1)
struct TReportRec
{
	char FileName[260];
	int Time;
	int Size;
	int CompressSize;
	int Attr;
	Word PackMethod;
	Byte Ratio;
	int Status;
} ;
#pragma pack(pop)

typedef void __stdcall (*UnzipReportProc)(int Retcode, pReportRec Rec);

typedef bool __stdcall (*UnzipQuestionProc)(pReportRec Rec);

//-- var, const, procedure ---------------------------------------------------
#define tBufSize (int)(262144)
#define tFSize (Word)(259)
#define file_starting (short)(-1000)
#define file_unzipping (short)(-1001)
#define file_completed (short)(-1002)
#define file_Failure (short)(-1003)
#define unzip_starting (short)(-1004)
#define unzip_completed (short)(-1005)
#define unzip_Ok (Byte)(0)
#define unzip_CRCErr (Shortint)(-1)
#define unzip_WriteErr (Shortint)(-2)
#define unzip_ReadErr (Shortint)(-3)
#define unzip_ZipFileErr (Shortint)(-4)
#define unzip_UserAbort (Shortint)(-5)
#define unzip_NotSupported (Shortint)(-6)
#define unzip_Encrypted (Shortint)(-7)
#define unzip_InUse (Shortint)(-8)
#define unzip_InternalError (Shortint)(-9)
#define unzip_NoMoreItems (Shortint)(-10)
#define unzip_FileError (Shortint)(-11)
#define unzip_NotZipfile (Shortint)(-12)
#define unzip_SeriousError (Shortint)(-100)
#define unzip_MissingParameter (short)(-500)
extern PACKAGE char *Unzipmethods[10];
extern PACKAGE char *UnzipActions[10];
extern PACKAGE System::ShortString __fastcall Upper( System::ShortString &s);

}	/* namespace Ziptypes */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Ziptypes;
#endif
//-- end unit ----------------------------------------------------------------
#endif	// ziptypes
