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
  char sym;
  std::string *str;
  HttpRequest *request;
  HttpHeaderLine *header_line;
  HttpGeneralHeaderLine *general_header_line;
  HttpRequestHeaderLine *request_header_line;
  HttpEntityHeaderLine *entity_header_line;
  std::vector<HttpHeaderLine *> *header;
  HttpRequestLine *request_line;
  HttpMethod method;
  HttpGeneralHeaderField general_field;
  HttpRequestHeaderField request_field;
  HttpEntityHeaderField entity_field;
}

%token <method> TOPTIONS TGET THEAD TPOST TPUT TDELETE TTRACE TCONNECT

%token <general_field> TCACHE_CONTROL TCONNECTION TDATE TPRAGMA TTRAILER
%token <general_field> TTRANSFER_ENCODING TUPGRADE TVIA TWARNING

%token <request_field> TACCEPT TACCEPT_CHARSET TACCEPT_ENCODING
%token <request_field> TACCEPT_LANGUAGE TAUTHORIZATION
%token <request_field> TEXPECT TFROM THOST TIF_MATCH TUSER_AGENT

%token <entity_field> TALLOW TCONTENT_ENCODING TCONTENT_LANGUAGE
%token <entity_field> TCONTENT_LENGTH TCONTENT_LOCATION TCONTENT_MD5
%token <entity_field> TCONTENT_TYPE TEXPIRES TLAST_MODIFIED TCONTENT_RANGE


%token <sym> THTTP
%token <sym> TLOALPHA THIALPHA TCRLF TOCTET
%token <sym> TCOLON
%token <sym> TNUM

%type <request> request
%type <header_line> header_line
%type <general_header_line> general_header_line
%type <request_header_line> request_header_line
%type <entity_header_line> entity_header_line
%type <general_field> general_field
%type <request_field> request_field
%type <entity_field> entity_field
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

method : TGET { $$ = GET; }
       | TPOST { $$ = POST; }
       ;

header: { $$ = new vector<HttpHeaderLine *>(); }
      | header header_line TCRLF { $1->push_back($2); }
      ;

header_line : general_header_line { $$ = $1; }
            | request_header_line { $$ = $1; }
            | entity_header_line { $$ = $1; }

general_header_line : general_field TCOLON value
                    { $$ = new HttpGeneralHeaderLine();
                      $$->field = $1;
                      $$->value = *$3;
                      $$->type = GENERAL; }

request_header_line : request_field TCOLON value
                    { $$ = new HttpRequestHeaderLine();
                      $$->field = $1;
                      $$->value = *$3;
                      $$->type = REQUEST; }

entity_header_line : entity_field TCOLON value
                    { $$ = new HttpEntityHeaderLine();
                      $$->field = $1;
                      $$->value = *$3;
                      $$->type = ENTITY; }

general_field : TCACHE_CONTROL { $$ = CACHE_CONTROL; }
              | TCONNECTION { $$ = CONNECTION; }
              | TDATE { $$ = DATE; }
              | TPRAGMA { $$ = PRAGMA; }
              | TTRAILER { $$ = TRAILER; }
              | TTRANSFER_ENCODING { $$ = TRANSFER_ENCODING; }
              | TUPGRADE { $$ = UPGRADE; }
              | TVIA { $$ = VIA; }
              | TWARNING { $$ = WARNING; }
              ;

request_field : TACCEPT { $$ = ACCEPT; }
              | TACCEPT_CHARSET { $$ = ACCEPT_CHARSET; }
              | TACCEPT_ENCODING { $$ = ACCEPT_ENCODING; }
              | TACCEPT_LANGUAGE { $$ = ACCEPT_LANGUAGE; }
              | TAUTHORIZATION { $$ = AUTHORIZATION; }
              | TEXPECT { $$ = EXPECT; }
              | TFROM { $$ = FROM; }
              | THOST { $$ = HOST; }
              | TIF_MATCH { $$ = IF_MATCH; }
              | TUSER_AGENT { $$ = USER_AGENT; }
              ;

entity_field : TALLOW { $$ = ALLOW; }
             | TCONTENT_ENCODING { $$ = CONTENT_ENCODING; }
             | TCONTENT_LANGUAGE { $$ = CONTENT_LANGUAGE; }
             | TCONTENT_LENGTH { $$ = CONTENT_LENGTH; }
             | TCONTENT_LOCATION { $$ = CONTENT_LOCATION; }
             | TCONTENT_MD5 { $$ = CONTENT_MD5; }
             | TCONTENT_RANGE { $$ = CONTENT_RANGE; }
             | TCONTENT_TYPE { $$ = CONTENT_TYPE; }
             | TEXPIRES { $$ = EXPIRES; }
             | TLAST_MODIFIED { $$ = LAST_MODIFIED; }
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
  HttpRequestLine rl = request->request_line;
  cout << "protocol version: " << rl.protocol_version << endl;
  cout << "method: ";
  switch (rl.method) {
  case GET:
    cout << "GET\n";
    break;
  default:
    cout << "Not GET\n";
  }
  cout << "uri: " << rl.uri << endl;
  cout << "header:\n";
  vector<HttpHeaderLine *> h = request->header;
  for (int i=0; i<h.size(); ++i) {
    HttpHeaderLine *hl = h[i];
    hl->print_field();
    cout << ": " << hl->value << endl;
  }
  return 0;
}
