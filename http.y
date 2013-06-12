%{
#include <memory>
#include <string>
#include <syslog.h>
#include <vector>
#include "http_request.h"
using namespace std;

int yylex();
void yyerror(const char *p) { syslog(LOG_ERR, "error: %s", p); }
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
  std::vector<std::unique_ptr<HttpHeaderLine>> *header;
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
%token <request_field> TACCEPT_LANGUAGE TAUTHORIZATION TUSER_AGENT
%token <request_field> TEXPECT TFROM THOST TIF_MATCH TTE
%token <request_field> TIF_MODIFIED_SINCE TIF_NONE_MATCH TIF_RANGE
%token <request_field> TIF_UNMODIFIED_SINCE TMAX_FORWARDS
%token <request_field> TPROXY_AUTHORIZATION TRANGE TREFERER

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
                                      request->request_line.reset($1);
                                      request->header.reset($2); }
        ;

request_line : method uri THTTP name TCRLF { $$ = new HttpRequestLine();
                                             $$->method = $1;
                                             $$->uri.reset($2);
                                             $$->protocol_version.reset($4); }
             ;

method : TGET { $$ = GET; }
       | TPOST { $$ = POST; }
       ;

header: { $$ = new vector<unique_ptr<HttpHeaderLine>>(); }
      | header header_line TCRLF { unique_ptr<HttpHeaderLine> h($2);
                                   $1->push_back(move(h)); }
      ;

header_line : general_header_line { $$ = $1; }
            | request_header_line { $$ = $1; }
            | entity_header_line { $$ = $1; }

general_header_line : general_field TCOLON value
                    { $$ = new HttpGeneralHeaderLine();
                      $$->field = $1;
                      $$->value.reset($3);
                      $$->type = GENERAL; }

request_header_line : request_field TCOLON value
                    { $$ = new HttpRequestHeaderLine();
                      $$->field = $1;
                      $$->value.reset($3);
                      $$->type = REQUEST; }

entity_header_line : entity_field TCOLON value
                    { $$ = new HttpEntityHeaderLine();
                      $$->field = $1;
                      $$->value.reset($3);
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
              | TIF_MODIFIED_SINCE { $$ = IF_MODIFIED_SINCE; }
              | TIF_NONE_MATCH { $$ = IF_NONE_MATCH; }
              | TIF_RANGE { $$ = IF_RANGE; }
              | TIF_UNMODIFIED_SINCE { $$ = IF_UNMODIFIED_SINCE; }
              | TMAX_FORWARDS { $$ = MAX_FORWARDS; }
              | TPROXY_AUTHORIZATION { $$ = PROXY_AUTHORIZATION; }
              | TRANGE { $$ = RANGE; }
              | TREFERER { $$ = REFERER; }
              | TTE { $$ = TE; }
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
