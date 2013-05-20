%{
#include <iostream>
#include <string>
using namespace std;

int yylex();
void yyerror(const char *p) { cerr << "Error!\n"; }
%}

%union {
  int num;
  char sym;
  std::string *str;
}

%token <sym> TGET TPOST THTTP 
%token <sym> TLOALPHA THIALPHA TCRLF TOCTET
%token <sym> TCOLON
%token <num> TNUM

%type <str> header
%type <str> header_line header_lines
%type <str> request_line name value uri

%%
header : header_lines TCRLF { cout << "HEADER!"; }
       ;

header_lines : { cout << "header lines done"; }
             | header_line TCRLF header_lines { cout << "HEADER LINES"; }
             ;

header_line : request_line { cout << "REQUEST\n"; }
            | name TCOLON value { cout << "MAPPING\n"; }
            ;

request_line : TGET uri THTTP name TCRLF { cout << "GET\n"; }
             | TPOST uri THTTP name TCRLF { cout << "POST"; }
             ;

name : { cout << "name done\n"; }
     | TLOALPHA name { cout << $1 << endl; }
     | THIALPHA name { cout << $1 << endl; }
     | TOCTET name { cout << $1 << endl; }
     | TNUM name { cout << $1 << endl; }
     ;
value : name {}
      ;
uri : name {}
    ;

%%

int main() {
  yyparse();
  return 0;
}
