%{
#include <iostream>
#include <string>
#include <vector>
#include "http.h"
using namespace std;

int yylex();
void yyerror(const char *p) { cerr << "error: " << p << endl; }
HttpRequest *request;
%}

%union {
  int num;
  char sym;
  std::string *str;
  HttpRequest *request;
  HttpHeaderLine *header_line;
  std::vector<HttpHeaderLine> *header;
  HttpRequestLine *request_line;
  HttpMethod method;
}

%token <method> TOPTIONS TGET THEAD TPOST TPUT TDELETE TTRACE TCONNECT
%token <sym> THTTP
%token <sym> TLOALPHA THIALPHA TCRLF TOCTET
%token <sym> TCOLON
%token <num> TNUM

%type <request> request
%type <header_line> header_line
%type <request_line> request_line
%type <method> method
%type <header> header
%type <str> name value uri

%error-verbose

%%
request : request_line header TCRLF { request = new HttpRequest();
                                      request->request_line = *$1;
                                      request->header = *$2; }
        ;

request_line : method uri THTTP name TCRLF { $$ = new HttpRequestLine();
                                             $$->method = $1;
                                             $$->uri = *$2;
                                             $$->protocol_version = *$4; }
             ;

header: { $$ = new vector<HttpHeaderLine>(); }
      | header header_line TCRLF { $1->push_back(*$2); }
      ;

header_line : name TCOLON value { $$ = new HttpHeaderLine();
                                  $$->name = *$1;
                                  $$->value = *$3; }
            ;

method : TGET { $$ = GET; }
       | TPOST { $$ = POST; }
       ;

name : { $$ = new string(); }
     | name TLOALPHA { $1->append(1, $2); }
     | name THIALPHA { $1->append(1, $2); }
     | name TOCTET { $1->append(1, $2); }
     | name TNUM { $1->append(1, $2); }
     ;
value : name { $$ = $1; }
      ;
uri : name { $$ = $1; }
    ;

%%

int main() {
  yyparse();
  return 0;
}
