%{
#include <iostream>
#include <string>
#include "http.tab.hpp"
using namespace std;
%}

%option noyywrap

%%
GET { return TGET; }
POST { return TPOST; }
HTTP { return THTTP; }
[a-z] { yylval.sym = yytext[0]; return TLOALPHA; }
[A-Z] { yylval.sym = yytext[0]; return THIALPHA; }
[0-9] { yylval.num = atoi(yytext); return TNUM; }
: { return TCOLON; }
[ \t] {}
\r\n { cout << "CRLF\n"; return TCRLF; }
. { yylval.sym = yytext[0]; return TOCTET; }
%%
