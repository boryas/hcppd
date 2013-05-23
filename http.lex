%{
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "http.h"
#include "http.tab.hpp"

using namespace std;
%}

%s colon_seen

%option noyywrap

%%
OPTIONS { return TOPTIONS; }
GET { return TGET; }
HEAD { return THEAD; }
POST { return TPOST; }
PUT { return TPUT; }
DELETE { return TDELETE; }
TRACE { return TTRACE; }
CONNECT { return TCONNECT; }
HTTP { return THTTP; }

Cache-Control { return TCACHE_CONTROL; }
Connection { return TCONNECTION; }
Date { return TDATE; }
Pragma { return TPRAGMA; }
Trailer { return TTRAILER; }
Transfer-Encoding { return TTRANSFER_ENCODING; }
Upgrade { return TUPGRADE; }
Via { return TVIA; }
Warning { return TWARNING; }

Accept { return TACCEPT; }
Accept-Charset { return TACCEPT_CHARSET; }
Accept-Encoding { return TACCEPT_ENCODING; }
Accept-Language { return TACCEPT_LANGUAGE; }
Authorization { return TAUTHORIZATION; }
Expect { return TEXPECT; }
From { return TFROM; }
Host { return THOST; }
If-Match { return TIF_MATCH; }
User-Agent { return TUSER_AGENT; }

Allow { return TALLOW; }
Content-Encoding { return TCONTENT_ENCODING; }
Content-Language { return TCONTENT_LANGUAGE; }
Content-Length { return TCONTENT_LENGTH; }
Content-Location { return TCONTENT_LOCATION; }
Content-MD5 { return TCONTENT_MD5; }
Content-Range { return TCONTENT_RANGE; }
Content-Type { return TCONTENT_TYPE; }
Expires { return TEXPIRES; }
Last-Modified { return TLAST_MODIFIED; }

[a-z] { yylval.sym = yytext[0]; return TLOALPHA; }
[A-Z] { yylval.sym = yytext[0]; return THIALPHA; }
[0-9] { yylval.sym = yytext[0]; return TNUM; }
<colon_seen>: { yylval.sym = yytext[0]; return TOCTET; }
: { BEGIN(colon_seen); return TCOLON; }
[ \t] {}
\r\n { BEGIN(INITIAL); return TCRLF; }
. { yylval.sym = yytext[0]; return TOCTET; }
%%
