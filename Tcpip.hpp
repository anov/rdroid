// Borland C++ Builder
// Copyright (c) 1995, 1998 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'tcpip.pas' rev: 3.00

#ifndef tcpipHPP
#define tcpipHPP
#include "ip_misc.hpp"
#include <Messages.hpp>
#include <Windows.hpp>
#include <WinSock.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <SysInit.hpp>
#include <System.hpp>

//-- user supplied -----------------------------------------------------------

namespace Tcpip
{
//-- type declarations -------------------------------------------------------
enum t_socket_state { invalid, valid, connected, state_unknown };

enum t_timemode { tzUTC, tzLocal };

enum t_ftp_mode { tftp_download, tftp_upload, tftp_getdir };

enum t_filetype { ft_none, ft_dir, ft_file, ft_link };

enum t_lpr_types { lp_plain, lp_ascii, lp_dvi, lp_plot, lp_ditroff, lp_ps, lp_pr, lp_fortran, lp_troff, 
	lp_raster, lp_cif };

enum t_encoding { ec_base64, ec_quotedprintable, ec_none };

enum TTraceLevel { tt_proto_sent, tt_proto_get, tt_socket };

#pragma pack(push, 1)
struct t_filedata
{
	t_filetype filetype;
	int size;
	System::AnsiString name;
	System::TDateTime datetime;
} ;
#pragma pack(pop)

class DELPHICLASS ETcpIpError;
class PASCALIMPLEMENTATION ETcpIpError : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ __fastcall ETcpIpError(const System::AnsiString Msg) : Sysutils::Exception(Msg
		) { }
	/* Exception.CreateFmt */ __fastcall ETcpIpError(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ __fastcall ETcpIpError(int Ident, Extended Dummy) : Sysutils::Exception(Ident
		, Dummy) { }
	/* Exception.CreateResFmt */ __fastcall ETcpIpError(int Ident, const System::TVarRec * Args, const 
		int Args_Size) : Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ __fastcall ETcpIpError(const System::AnsiString Msg, int AHelpContext) : 
		Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ __fastcall ETcpIpError(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext
		) { }
	/* Exception.CreateResHelp */ __fastcall ETcpIpError(int Ident, int AHelpContext) : Sysutils::Exception(
		Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ __fastcall ETcpIpError(int Ident, const System::TVarRec * Args, const 
		int Args_Size, int AHelpContext) : Sysutils::Exception(Ident, Args, Args_Size, AHelpContext) { }
	
public:
	/* TObject.Destroy */ __fastcall virtual ~ETcpIpError(void) { }
	
};

class DELPHICLASS ESocketError;
class PASCALIMPLEMENTATION ESocketError : public Tcpip::ETcpIpError 
{
	typedef Tcpip::ETcpIpError inherited;
	
public:
	Word errornumber;
	__fastcall ESocketError(Word number);
public:
	/* Exception.CreateFmt */ __fastcall ESocketError(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Tcpip::ETcpIpError(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ __fastcall ESocketError(int Ident, Extended Dummy) : Tcpip::ETcpIpError(Ident
		, Dummy) { }
	/* Exception.CreateResFmt */ __fastcall ESocketError(int Ident, const System::TVarRec * Args, const 
		int Args_Size) : Tcpip::ETcpIpError(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ __fastcall ESocketError(const System::AnsiString Msg, int AHelpContext) : 
		Tcpip::ETcpIpError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ __fastcall ESocketError(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Tcpip::ETcpIpError(Msg, Args, Args_Size, AHelpContext
		) { }
	/* Exception.CreateResHelp */ __fastcall ESocketError(int Ident, int AHelpContext) : Tcpip::ETcpIpError(
		Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ __fastcall ESocketError(int Ident, const System::TVarRec * Args, const 
		int Args_Size, int AHelpContext) : Tcpip::ETcpIpError(Ident, Args, Args_Size, AHelpContext) { }
	
public:
	/* TObject.Destroy */ __fastcall virtual ~ESocketError(void) { }
	
};

class DELPHICLASS EProtocolError;
class PASCALIMPLEMENTATION EProtocolError : public Tcpip::ETcpIpError 
{
	typedef Tcpip::ETcpIpError inherited;
	
public:
	Word errornumber;
	System::AnsiString protocoll;
	__fastcall EProtocolError(const System::AnsiString proto, const System::AnsiString Msg, Word number
		);
public:
	/* Exception.CreateFmt */ __fastcall EProtocolError(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Tcpip::ETcpIpError(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ __fastcall EProtocolError(int Ident, Extended Dummy) : Tcpip::ETcpIpError(
		Ident, Dummy) { }
	/* Exception.CreateResFmt */ __fastcall EProtocolError(int Ident, const System::TVarRec * Args, const 
		int Args_Size) : Tcpip::ETcpIpError(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ __fastcall EProtocolError(const System::AnsiString Msg, int AHelpContext
		) : Tcpip::ETcpIpError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ __fastcall EProtocolError(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Tcpip::ETcpIpError(Msg, Args, Args_Size, AHelpContext
		) { }
	/* Exception.CreateResHelp */ __fastcall EProtocolError(int Ident, int AHelpContext) : Tcpip::ETcpIpError(
		Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ __fastcall EProtocolError(int Ident, const System::TVarRec * Args, 
		const int Args_Size, int AHelpContext) : Tcpip::ETcpIpError(Ident, Args, Args_Size, AHelpContext) { }
		
	
public:
	/* TObject.Destroy */ __fastcall virtual ~EProtocolError(void) { }
	
};

class DELPHICLASS EProtocolBusy;
class PASCALIMPLEMENTATION EProtocolBusy : public Tcpip::ETcpIpError 
{
	typedef Tcpip::ETcpIpError inherited;
	
public:
	__fastcall EProtocolBusy(void);
public:
	/* Exception.CreateFmt */ __fastcall EProtocolBusy(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size) : Tcpip::ETcpIpError(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ __fastcall EProtocolBusy(int Ident, Extended Dummy) : Tcpip::ETcpIpError(
		Ident, Dummy) { }
	/* Exception.CreateResFmt */ __fastcall EProtocolBusy(int Ident, const System::TVarRec * Args, const 
		int Args_Size) : Tcpip::ETcpIpError(Ident, Args, Args_Size) { }
	/* Exception.CreateHelp */ __fastcall EProtocolBusy(const System::AnsiString Msg, int AHelpContext)
		 : Tcpip::ETcpIpError(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ __fastcall EProtocolBusy(const System::AnsiString Msg, const System::TVarRec 
		* Args, const int Args_Size, int AHelpContext) : Tcpip::ETcpIpError(Msg, Args, Args_Size, AHelpContext
		) { }
	/* Exception.CreateResHelp */ __fastcall EProtocolBusy(int Ident, int AHelpContext) : Tcpip::ETcpIpError(
		Ident, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ __fastcall EProtocolBusy(int Ident, const System::TVarRec * Args, 
		const int Args_Size, int AHelpContext) : Tcpip::ETcpIpError(Ident, Args, Args_Size, AHelpContext) { }
		
	
public:
	/* TObject.Destroy */ __fastcall virtual ~EProtocolBusy(void) { }
	
};

typedef void __fastcall (__closure *TTraceProc)(const System::AnsiString s, TTraceLevel level);

typedef void __fastcall (__closure *TDataTransferProc)(System::TObject* Sender, t_ftp_mode mode, int 
	bytes);

typedef void __fastcall (__closure *TFTPActionCompleteProc)(System::TObject* Sender, t_ftp_mode mode
	);

class DELPHICLASS T_TcpIp;
class PASCALIMPLEMENTATION T_TcpIp : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
protected:
	int f_handle;
	int f_Socket;
	System::AnsiString f_hostname;
	TTraceProc f_tracer;
	short f_socket_number;
	int ip_address;
	bool f_eof;
	bool f_newdata;
	Classes::TStream* f_stream;
	void *f_buffer;
	bool f_async;
	bool f_logged_in;
	virtual void __fastcall WndProc(Messages::TMessage &Msg);
	int __fastcall Create_Socket(void);
	void __fastcall connect_socket(int &socket, short Socket_number, int ip_address);
	void __fastcall bind_socket(int &socket, Word out_port_min, Word out_port_max);
	virtual void __fastcall open_socket_out(int &socket, short Socket_number, int ip_address);
	void __fastcall open_socket_in(int &socket, short Socket_number, int ip_address);
	void __fastcall close_socket(int &socket);
	void __fastcall close_socket_linger(int &socket);
	int __fastcall accept_socket_in(int socket, sockaddr_in &SockInfo);
	t_socket_state __fastcall socket_state(int socket);
	short __fastcall Socket_by_name(const System::AnsiString service);
	System::AnsiString __fastcall read_line(int f_socket);
	void __fastcall read_var(int f_socket, void *buf, int size, int &_ok);
	void __fastcall write_buf(int f_socket, const void *buf, int size);
	void __fastcall write_s(int f_socket, const System::AnsiString s);
	void __fastcall SetStream(Classes::TStream* value);
	virtual void __fastcall action(void);
	virtual void __fastcall SendCommand(const System::AnsiString s);
	
public:
	virtual void __fastcall Login(void);
	virtual void __fastcall Logout(void);
	__property TTraceProc OnTrace = {read=f_tracer, write=f_tracer};
	__fastcall virtual T_TcpIp(Classes::TComponent* Aowner);
	__fastcall virtual ~T_TcpIp(void);
	virtual bool __fastcall eof(int f_socket);
};

class DELPHICLASS T_Finger;
class PASCALIMPLEMENTATION T_Finger : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	System::AnsiString f_user;
	
public:
	__fastcall virtual T_Finger(Classes::TComponent* Aowner);
	__property Classes::TStream* stream = {read=f_stream};
	virtual void __fastcall action(void);
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
	__property System::AnsiString User = {read=f_user, write=f_user};
public:
	/* T_TcpIp.Destroy */ __fastcall virtual ~T_Finger(void) { }
	
};

struct TFingerInfo
{
	System::AnsiString hostname;
	int address;
	System::AnsiString request;
} ;

typedef void __fastcall (__closure *TFingerRequest)(System::TObject* Sender, const TFingerInfo &FingerInfo
	);

class DELPHICLASS T_Fingerd;
class PASCALIMPLEMENTATION T_Fingerd : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	TFingerRequest f_fingerrequest;
	Classes::TStringList* f_answer;
	virtual void __fastcall WndProc(Messages::TMessage &Msg);
	void __fastcall SetAnswer(Classes::TStringList* Value);
	virtual void __fastcall do_action(void);
	
public:
	__fastcall virtual T_Fingerd(Classes::TComponent* Aowner);
	__fastcall virtual ~T_Fingerd(void);
	virtual void __fastcall action(void);
	
__published:
	__property TFingerRequest OnFingerRequest = {read=f_fingerrequest, write=f_fingerrequest};
	__property Classes::TStringList* Answer = {read=f_answer, write=SetAnswer};
};

class DELPHICLASS T_HTTP;
class PASCALIMPLEMENTATION T_HTTP : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	System::AnsiString f_url;
	System::AnsiString f_path;
	System::AnsiString f_proxy;
	System::AnsiString f_sender;
	System::AnsiString f_reference;
	System::AnsiString f_agent;
	bool f_nocache;
	int f_status_nr;
	System::AnsiString f_status_txt;
	int f_size;
	System::AnsiString f_type;
	System::AnsiString f_author;
	Classes::TStringList* f_do_author;
	System::AnsiString f_content_post;
	void __fastcall GetBody(void);
	void __fastcall GetHead(void);
	void __fastcall sendrequest(const System::AnsiString method, const System::AnsiString version);
	void __fastcall getanswer(void);
	
public:
	__property Classes::TStream* stream = {read=f_stream, write=SetStream};
	__property int content_size = {read=f_size, nodefault};
	__property System::AnsiString content_type = {read=f_type};
	__property int status_number = {read=f_status_nr, nodefault};
	__property System::AnsiString status_text = {read=f_status_txt};
	virtual void __fastcall action(void);
	void __fastcall Post(void);
	void __fastcall Get(void);
	__fastcall virtual T_HTTP(Classes::TComponent* Aowner);
	__fastcall virtual ~T_HTTP(void);
	void __fastcall DoBasicAuthorization(const System::AnsiString username, const System::AnsiString password
		);
	__property Classes::TStringList* AuthorizationRequest = {read=f_do_author};
	__property System::AnsiString Authorization = {read=f_author, write=f_author};
	
__published:
	__property System::AnsiString URL = {read=f_url, write=f_url};
	__property System::AnsiString Proxy = {read=f_proxy, write=f_proxy};
	__property System::AnsiString Sender = {read=f_sender, write=f_sender};
	__property System::AnsiString Agent = {read=f_agent, write=f_agent};
	__property System::AnsiString Reference = {read=f_reference, write=f_reference};
	__property bool NoCache = {read=f_nocache, write=f_nocache, nodefault};
	__property System::AnsiString ContentTypePost = {read=f_content_post, write=f_content_post};
	__property OnTrace ;
};

class DELPHICLASS T_FTP;
class PASCALIMPLEMENTATION T_FTP : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	System::AnsiString f_url;
	int f_status_nr;
	System::AnsiString f_status_txt;
	System::AnsiString f_user;
	System::AnsiString f_password;
	int f_comm_socket;
	bool f_passive;
	Word f_port;
	t_ftp_mode f_mode;
	t_ftp_mode f_mode_intern;
	Classes::TStringList* f_cur_dir;
	int f_cur_dir_index;
	int f_size;
	bool f_busy;
	TFTPActionCompleteProc f_onaction;
	TDataTransferProc f_ondata_got;
	Classes::TMemoryStream* f_dir_stream;
	bool f_async_data;
	void __fastcall response(void);
	System::AnsiString __fastcall read_line_comm();
	virtual void __fastcall SendCommand(const System::AnsiString s);
	void __fastcall get_datasocket(void);
	virtual void __fastcall WndProc(Messages::TMessage &Msg);
	bool __fastcall do_write(void);
	bool __fastcall do_read(void);
	void __fastcall finish_upload(void);
	void __fastcall finish_download(void);
	void __fastcall finish_getdir(void);
	
public:
	void __fastcall changedir(const System::AnsiString f_dir);
	void __fastcall removefile(const System::AnsiString filename);
	void __fastcall removedir(const System::AnsiString dirname);
	void __fastcall makedir(const System::AnsiString dirname);
	void __fastcall renamefile(const System::AnsiString prior, const System::AnsiString after);
	void __fastcall getdir(const System::AnsiString dirname);
	t_filedata __fastcall getdirentry();
	__property Classes::TStream* stream = {read=f_stream, write=SetStream};
	__property int status_number = {read=f_status_nr, nodefault};
	__property System::AnsiString status_text = {read=f_status_txt};
	__property bool busy = {read=f_busy, nodefault};
	virtual void __fastcall login(void);
	virtual void __fastcall logout(void);
	void __fastcall download(void);
	void __fastcall upload(void);
	void __fastcall abort(void);
	void __fastcall noop(void);
	__fastcall virtual T_FTP(Classes::TComponent* Aowner);
	__fastcall virtual ~T_FTP(void);
	virtual void __fastcall action(void);
	__property int Size = {read=f_size, nodefault};
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
	__property System::AnsiString URI = {read=f_url, write=f_url};
	__property System::AnsiString Password = {read=f_password, write=f_password};
	__property System::AnsiString Username = {read=f_user, write=f_user};
	__property bool Passive = {read=f_passive, write=f_passive, default=1};
	__property Word Port = {read=f_port, write=f_port, default=21};
	__property t_ftp_mode Mode = {read=f_mode, write=f_mode, default=0};
	__property TDataTransferProc OnDataReceived = {read=f_ondata_got, write=f_ondata_got};
	__property TFTPActionCompleteProc OnActionComplete = {read=f_onaction, write=f_onaction};
	__property bool Async = {read=f_async_data, write=f_async_data, nodefault};
	__property OnTrace ;
};

class DELPHICLASS T_Time;
class PASCALIMPLEMENTATION T_Time : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	System::TDateTime f_time;
	t_timemode f_timemode;
	
public:
	__fastcall virtual T_Time(Classes::TComponent* Aowner);
	virtual void __fastcall action(void);
	__property System::TDateTime time = {read=f_time};
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
	__property t_timemode TimeMode = {read=f_timemode, write=f_timemode, default=0};
public:
	/* T_TcpIp.Destroy */ __fastcall virtual ~T_Time(void) { }
	
};

class DELPHICLASS T_RCommon;
class PASCALIMPLEMENTATION T_RCommon : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	virtual void __fastcall open_socket_out(int &socket, short Socket_number, int ip_address);
	
public:
	virtual void __fastcall action(void);
	__property Classes::TStream* stream = {read=f_stream};
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
public:
	/* T_TcpIp.Create */ __fastcall virtual T_RCommon(Classes::TComponent* Aowner) : Tcpip::T_TcpIp(Aowner
		) { }
	/* T_TcpIp.Destroy */ __fastcall virtual ~T_RCommon(void) { }
	
};

class DELPHICLASS T_RExec;
class PASCALIMPLEMENTATION T_RExec : public Tcpip::T_RCommon 
{
	typedef Tcpip::T_RCommon inherited;
	
protected:
	System::AnsiString f_user;
	System::AnsiString f_pass;
	System::AnsiString f_command;
	virtual void __fastcall login(void);
	
public:
	__fastcall virtual T_RExec(Classes::TComponent* Aowner);
	
__published:
	__property System::AnsiString UserName = {read=f_user, write=f_user};
	__property System::AnsiString Password = {read=f_pass, write=f_pass};
	__property System::AnsiString Command = {read=f_command, write=f_command};
public:
	/* T_TcpIp.Destroy */ __fastcall virtual ~T_RExec(void) { }
	
};

class DELPHICLASS T_Rsh;
class PASCALIMPLEMENTATION T_Rsh : public Tcpip::T_RCommon 
{
	typedef Tcpip::T_RCommon inherited;
	
protected:
	System::AnsiString f_user_r;
	System::AnsiString f_user_l;
	System::AnsiString f_command;
	virtual void __fastcall login(void);
	
public:
	__fastcall virtual T_Rsh(Classes::TComponent* Aowner);
	
__published:
	__property System::AnsiString LocalUser = {read=f_user_l, write=f_user_l};
	__property System::AnsiString RemoteUser = {read=f_user_r, write=f_user_r};
	__property System::AnsiString Command = {read=f_command, write=f_command};
public:
	/* T_TcpIp.Destroy */ __fastcall virtual ~T_Rsh(void) { }
	
};

class DELPHICLASS T_lpr;
class PASCALIMPLEMENTATION T_lpr : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	t_lpr_types f_printtype;
	bool f_banner;
	int f_count;
	System::AnsiString f_user;
	System::AnsiString f_queue;
	System::AnsiString f_user_mail;
	System::AnsiString f_jobname;
	System::AnsiString f_title;
	void __fastcall response(void);
	virtual void __fastcall open_socket_out(int &socket, short Socket_number, int ip_address);
	
public:
	__fastcall virtual T_lpr(Classes::TComponent* Aowner);
	__property Classes::TStream* stream = {read=f_stream, write=SetStream};
	virtual void __fastcall action(void);
	void __fastcall SendPrintData(void);
	void __fastcall GetQueueStatus(bool detailed);
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
	__property System::AnsiString User = {read=f_user, write=f_user};
	__property System::AnsiString PrintQueue = {read=f_queue, write=f_queue};
	__property System::AnsiString MailTo = {read=f_user_mail, write=f_user_mail};
	__property System::AnsiString JobName = {read=f_jobname, write=f_jobname};
	__property t_lpr_types PrintType = {read=f_printtype, write=f_printtype, default=1};
	__property int CopyCount = {read=f_count, write=f_count, default=1};
	__property bool PrintBanner = {read=f_banner, write=f_banner, default=0};
	__property System::AnsiString PrintTitle = {read=f_title, write=f_title};
public:
	/* T_TcpIp.Destroy */ __fastcall virtual ~T_lpr(void) { }
	
};

class DELPHICLASS T_SMTP;
class PASCALIMPLEMENTATION T_SMTP : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	System::AnsiString f_user;
	System::AnsiString f_host;
	int f_status_nr;
	System::AnsiString f_status_txt;
	Classes::TStringList* f_receipts;
	Classes::TStringList* f_body;
	void __fastcall SetBody(Classes::TStringList* Value);
	void __fastcall SetRecipients(Classes::TStringList* Value);
	void __fastcall response(void);
	
public:
	__fastcall virtual T_SMTP(Classes::TComponent* Aowner);
	__fastcall virtual ~T_SMTP(void);
	virtual void __fastcall action(void);
	__property Classes::TStringList* Recipients = {read=f_receipts, write=SetRecipients};
	__property Classes::TStringList* Message = {read=f_body, write=SetBody};
	__property System::AnsiString Sender = {read=f_user, write=f_user};
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
	__property OnTrace ;
};

class DELPHICLASS T_Pop3;
class PASCALIMPLEMENTATION T_Pop3 : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	System::AnsiString f_user;
	System::AnsiString f_pass;
	Classes::TList* f_list;
	Classes::TStringList* f_mail;
	void __fastcall response(void);
	
public:
	__property Classes::TStringList* Mail = {read=f_mail};
	__fastcall virtual T_Pop3(Classes::TComponent* Aowner);
	__fastcall virtual ~T_Pop3(void);
	virtual void __fastcall action(void);
	virtual void __fastcall Login(void);
	void __fastcall GetHeaders(void);
	virtual void __fastcall Logout(void);
	void __fastcall GetMail(int index);
	void __fastcall DeleteMail(int index);
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
	__property System::AnsiString UserName = {read=f_user, write=f_user};
	__property System::AnsiString Password = {read=f_pass, write=f_pass};
	__property OnTrace ;
};

class DELPHICLASS T_NNTP;
class PASCALIMPLEMENTATION T_NNTP : public Tcpip::T_TcpIp 
{
	typedef Tcpip::T_TcpIp inherited;
	
protected:
	Classes::TStringList* f_news;
	Classes::TStringList* f_newsgroups;
	int f_status_nr;
	System::AnsiString f_status_txt;
	void __fastcall response(void);
	virtual void __fastcall action(void);
	void __fastcall SetNews(Classes::TStringList* value);
	void __fastcall GetArticleInternally(void);
	
public:
	__property Classes::TStringList* News = {read=f_news, write=SetNews};
	__property Classes::TStringList* NewsGroups = {read=f_newsgroups};
	__fastcall virtual T_NNTP(Classes::TComponent* Aowner);
	__fastcall virtual ~T_NNTP(void);
	virtual void __fastcall Login(void);
	virtual void __fastcall Logout(void);
	void __fastcall GetArticle(const System::AnsiString group, int index);
	void __fastcall GetArticleID(const System::AnsiString msgid);
	void __fastcall PostArticle(void);
	void __fastcall GetAllNewsgroups(void);
	void __fastcall GetNewNewsgroups(System::TDateTime since);
	void __fastcall SetGroup(const System::AnsiString group, int &low, int &high, int &count);
	void __fastcall GetArticleNr(int index);
	void __fastcall SetCurrentArticle(int index);
	void __fastcall GetCurrentArticle(void);
	void __fastcall GetNextArticle(void);
	void __fastcall GetPreviousArticle(void);
	
__published:
	__property System::AnsiString Hostname = {read=f_hostname, write=f_hostname};
	__property OnTrace ;
};

class DELPHICLASS T_MailNews;
class PASCALIMPLEMENTATION T_MailNews : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
protected:
	System::AnsiString f_from;
	System::AnsiString f_sender;
	System::AnsiString f_subject;
	Classes::TStringList* f_body;
	Classes::TStringList* f_add_header;
	Classes::TStringList* f_message;
	System::AnsiString f_references;
	System::AnsiString f_replyto;
	void __fastcall SetBody(Classes::TStringList* Value);
	void __fastcall SetHeader(Classes::TStringList* Value);
	
public:
	__fastcall virtual T_MailNews(Classes::TComponent* Aowner);
	__fastcall virtual ~T_MailNews(void);
	virtual void __fastcall action(void);
	
__published:
	__property System::AnsiString Sender = {read=f_sender, write=f_sender};
	__property System::AnsiString From = {read=f_from, write=f_from};
	__property Classes::TStringList* Body = {read=f_body, write=SetBody};
	__property Classes::TStringList* Header = {read=f_add_header, write=SetHeader};
	__property System::AnsiString Subject = {read=f_subject, write=f_subject};
	__property System::AnsiString References = {read=f_references, write=f_references};
	__property System::AnsiString ReplyTo = {read=f_replyto, write=f_replyto};
};

class DELPHICLASS T_Mail;
class PASCALIMPLEMENTATION T_Mail : public Tcpip::T_MailNews 
{
	typedef Tcpip::T_MailNews inherited;
	
protected:
	T_SMTP* f_smtp;
	Classes::TStringList* f_receipts;
	Classes::TStringList* f_cc;
	Classes::TStringList* f_bcc;
	bool f_disclose_receipts;
	void __fastcall SetRecipients(Classes::TStringList* Value);
	void __fastcall SetCC(Classes::TStringList* Value);
	void __fastcall SetBCC(Classes::TStringList* Value);
	
public:
	__fastcall virtual T_Mail(Classes::TComponent* Aowner);
	__fastcall virtual ~T_Mail(void);
	virtual void __fastcall action(void);
	
__published:
	__property T_SMTP* SMTP = {read=f_smtp, write=f_smtp};
	__property bool Disclose_Receipts = {read=f_disclose_receipts, write=f_disclose_receipts, default=0
		};
	__property Classes::TStringList* Recipients = {read=f_receipts, write=SetRecipients};
	__property Classes::TStringList* CC = {read=f_cc, write=SetCC};
	__property Classes::TStringList* BCC = {read=f_bcc, write=SetBCC};
};

class DELPHICLASS T_News;
class PASCALIMPLEMENTATION T_News : public Tcpip::T_MailNews 
{
	typedef Tcpip::T_MailNews inherited;
	
protected:
	T_NNTP* f_nntp;
	Classes::TStringList* f_newsgroups;
	System::AnsiString f_organization;
	void __fastcall SetNewsgroups(Classes::TStringList* Value);
	
public:
	__fastcall virtual T_News(Classes::TComponent* Aowner);
	__fastcall virtual ~T_News(void);
	virtual void __fastcall action(void);
	
__published:
	__property T_NNTP* NNTP = {read=f_nntp, write=f_nntp};
	__property Classes::TStringList* Newsgroups = {read=f_newsgroups, write=SetNewsgroups};
	__property System::AnsiString Organization = {read=f_organization, write=f_organization};
};

class DELPHICLASS T_Attachment;
class PASCALIMPLEMENTATION T_Attachment : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	System::AnsiString f_kind;
	System::AnsiString f_disposition;
	Classes::TStream* f_data;
	Classes::TStringList* f_text;
	t_encoding f_encoding;
	void __fastcall SetText(Classes::TStringList* value);
	void __fastcall SetData(Classes::TStream* value);
	
public:
	__fastcall T_Attachment(void);
	__fastcall virtual ~T_Attachment(void);
	__property System::AnsiString Kind = {read=f_kind, write=f_kind};
	__property System::AnsiString Disposition = {read=f_disposition, write=f_disposition};
	__property t_encoding Encoding = {read=f_encoding, write=f_encoding, nodefault};
	__property Classes::TStringList* Text = {read=f_text, write=SetText};
	__property Classes::TStream* Data = {read=f_data, write=SetData};
};

class DELPHICLASS T_Mime;
class PASCALIMPLEMENTATION T_Mime : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
protected:
	T_MailNews* f_mail;
	System::AnsiString boundary;
	Classes::TList* f_attachment;
	int __fastcall GetNumberOfAttachments(void);
	
public:
	__fastcall virtual T_Mime(Classes::TComponent* Aowner);
	__fastcall virtual ~T_Mime(void);
	int __fastcall AttachFile(const System::AnsiString filename);
	int __fastcall AttachText(Classes::TStringList* text);
	void __fastcall RemoveAllAttachments(void);
	void __fastcall action(void);
	void __fastcall SetMail(Classes::TStringList* mail);
	T_Attachment* __fastcall GetAttachment(int index);
	void __fastcall RemoveAttachment(int index);
	__property T_Attachment* Attachment[int index] = {read=GetAttachment};
	__property int NumberOfAttachments = {read=GetNumberOfAttachments, nodefault};
	
__published:
	__property T_MailNews* Mail = {read=f_mail, write=f_mail};
};

//-- var, const, procedure ---------------------------------------------------
#define uwm_socketevent (Word)(1280)
extern PACKAGE int lpr_count;
extern PACKAGE void __fastcall Register(void);

}	/* namespace Tcpip */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Tcpip;
#endif
//-- end unit ----------------------------------------------------------------
#endif	// tcpip
