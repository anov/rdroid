// Borland C++ Builder
// Copyright (c) 1995, 1998 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Unzip.pas' rev: 3.00

#ifndef UnzipHPP
#define UnzipHPP
#include <ziptypes.hpp>
#include <SysUtils.hpp>
#include <Messages.hpp>
#include <Windows.hpp>
#include <SysInit.hpp>
#include <System.hpp>

//-- user supplied -----------------------------------------------------------

namespace Unzip
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE int __stdcall GetSupportedMethods(void);
extern PACKAGE int __stdcall UnzipFile(char * in_name, char * out_name, int offset, int hFileAction, 
	int cm_index);
extern PACKAGE int __stdcall GetFirstInZip(char * zipfilename, Ziptypes::TZipRec &zprec);
extern PACKAGE int __stdcall GetNextInZip(Ziptypes::TZipRec &Zprec);
extern PACKAGE bool __stdcall IsZip(char * filename);
extern PACKAGE void __stdcall CloseZipFile(Ziptypes::TZipRec &Zprec);
extern PACKAGE int __stdcall FileUnzip(char * SourceZipFile, char * TargetDirectory, char * FileSpecs
	, Ziptypes::UnzipReportProc Report, Ziptypes::UnzipQuestionProc Question);
extern PACKAGE int __stdcall FileUnzipEx(char * SourceZipFile, char * TargetDirectory, char * FileSpecs
	);
extern PACKAGE int __stdcall ViewZip(char * SourceZipFile, char * FileSpecs, Ziptypes::UnzipReportProc 
	Report);
extern PACKAGE int __stdcall UnzipSize(char * SourceZipFile, int &Compressed);
extern PACKAGE void * __stdcall SetUnZipReportProc(Ziptypes::UnzipReportProc aProc);
extern PACKAGE void * __stdcall SetUnZipQuestionProc(Ziptypes::UnzipQuestionProc aProc);
extern PACKAGE void __stdcall ChfUnzip_Init(void);

}	/* namespace Unzip */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Unzip;
#endif
//-- end unit ----------------------------------------------------------------
#endif	// Unzip
