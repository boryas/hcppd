%{
#include <iostream>
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
[a-z] { yylval.sym = yytext[0]; return TLOALPHA; }
[A-Z] { yylval.sym = yytext[0]; return THIALPHA; }
[0-9] { yylval.num = atoi(yytext); return TNUM; }
<colon_seen>: { yylval.sym = yytext[0]; return TOCTET; }
: { BEGIN(colon_seen); return TCOLON; }
[ \t] {}
\r\n { BEGIN(INITIAL); return TCRLF; }
. { yylval.sym = yytext[0]; return TOCTET; }
%%
