#ifndef _H_TDR_XML_READER_SCANNER_H
#define _H_TDR_XML_READER_SCANNER_H

#include "tdr_xml_reader.h"

#define YYSTYPE tdr_xml_reader_scanner_context_t
#define YYLTYPE_IS_DECLARED
#define YYCTYPE   char
#define YYFILL(n) 
#define YYCURSOR  sp->yy_cursor
#define YYLIMIT   sp->yy_limit
#define YYMARKER sp->yy_marker
#define YYGETCONDITION()  sp->yy_state
#define YYSETCONDITION(s) sp->yy_state = s
#define yytext sp->yy_text
#define yyleng sp->yy_leng

#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYSTATE      YYGETCONDITION()


typedef enum tdr_xml_reader_token_e
{
	tok_end = 0,						//½âÎö½áÊø
	tok_error = 1,						//½âÎö´íÎó
	tok_tag_begin = 2,
	tok_tag_end = 3,
}tdr_xml_reader_token_t;

tdr_xml_reader_token_t tdr_xml_reader_scan(tdr_xml_reader_t *self);

void tdr_xml_reader_locate(tdr_xml_reader_t *self);

tdr_xml_reader_token_t tdr_xml_reader_get_token(tdr_xml_reader_t *self);

#endif//_H_TDR_XML_READER_SCANNER_H

