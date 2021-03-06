/* Generated by re2c 1.0.1 on Tue Feb 23 17:07:57 2021 */
#line 1 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
#include "../tdr_xlsx_reader.h"
#include "tdr_xlsx_reader_workbook_l.h"
#include <string.h>

#define YYGETCONDITION()  self->scanner.state
#define YYSETCONDITION(s) self->scanner.state = s
#define STATE(name)  yyc##name
#define BEGIN(state) YYSETCONDITION(STATE(state))
#define YYCURSOR self->scanner.cursor
#define YYLIMIT self->scanner.limit
#define YYMARKER self->scanner.marker
#define YYCTYPE char

const char* tdr_xlsx_reader_workbook_search_rid(tdr_xlsx_reader_t *self, const char* sheet)
{
	const char* rid_begin = NULL;
	const char* name_begin = NULL;

	self->scanner.cursor = self->workbook_buff;
	self->scanner.limit = self->workbook_buff + self->workbook_buff_size;
	self->scanner.marker = NULL;
	self->scanner.state = yycINITIAL;

restart:
	if(YYCURSOR >= YYLIMIT)
	{
		return NULL;
	}


#line 34 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
{
	YYCTYPE yych;
	switch (YYGETCONDITION()) {
	case yycINITIAL: goto yyc_INITIAL;
	case yycIN_SHEETS: goto yyc_IN_SHEETS;
	}
/* *********************************** */
yyc_INITIAL:
	yych = *YYCURSOR;
	switch (yych) {
	case '<':	goto yy5;
	default:	goto yy3;
	}
yy3:
	++YYCURSOR;
yy4:
#line 65 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
	{ goto restart;				}
#line 53 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
yy5:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy6;
	case 's':	goto yy8;
	default:	goto yy4;
	}
yy6:
	yych = *++YYCURSOR;
	switch (yych) {
	case 's':	goto yy9;
	default:	goto yy7;
	}
yy7:
	YYCURSOR = YYMARKER;
	goto yy4;
yy8:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'h':	goto yy10;
	default:	goto yy7;
	}
yy9:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'h':	goto yy11;
	default:	goto yy7;
	}
yy10:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'e':	goto yy12;
	default:	goto yy7;
	}
yy11:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'e':	goto yy13;
	default:	goto yy7;
	}
yy12:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'e':	goto yy14;
	default:	goto yy7;
	}
yy13:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'e':	goto yy15;
	default:	goto yy7;
	}
yy14:
	yych = *++YYCURSOR;
	switch (yych) {
	case 't':	goto yy16;
	default:	goto yy7;
	}
yy15:
	yych = *++YYCURSOR;
	switch (yych) {
	case 't':	goto yy17;
	default:	goto yy7;
	}
yy16:
	yych = *++YYCURSOR;
	switch (yych) {
	case 's':	goto yy18;
	default:	goto yy7;
	}
yy17:
	yych = *++YYCURSOR;
	switch (yych) {
	case 's':	goto yy19;
	default:	goto yy7;
	}
yy18:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy20;
	default:	goto yy7;
	}
yy19:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy22;
	default:	goto yy7;
	}
yy20:
	++YYCURSOR;
#line 33 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
	{ BEGIN(IN_SHEETS);goto restart;}
#line 146 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
yy22:
	++YYCURSOR;
#line 61 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
	{
	BEGIN(INITIAL);
	goto restart;
}
#line 154 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
/* *********************************** */
yyc_IN_SHEETS:
	yych = *YYCURSOR;
	switch (yych) {
	case '"':	goto yy28;
	case 'n':	goto yy29;
	case 'r':	goto yy30;
	default:	goto yy26;
	}
yy26:
	++YYCURSOR;
yy27:
#line 65 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
	{ goto restart;				}
#line 169 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
yy28:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case '/':	goto yy31;
	default:	goto yy27;
	}
yy29:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case 'a':	goto yy33;
	default:	goto yy27;
	}
yy30:
	yych = *(YYMARKER = ++YYCURSOR);
	switch (yych) {
	case ':':	goto yy34;
	default:	goto yy27;
	}
yy31:
	yych = *++YYCURSOR;
	switch (yych) {
	case '>':	goto yy35;
	default:	goto yy32;
	}
yy32:
	YYCURSOR = YYMARKER;
	goto yy27;
yy33:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'm':	goto yy37;
	default:	goto yy32;
	}
yy34:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'i':	goto yy38;
	default:	goto yy32;
	}
yy35:
	++YYCURSOR;
#line 55 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
	{
	if((sheet == NULL) || (strcmp(name_begin, sheet) == 0))
		return rid_begin;
	goto restart;
}
#line 217 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
yy37:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'e':	goto yy39;
	default:	goto yy32;
	}
yy38:
	yych = *++YYCURSOR;
	switch (yych) {
	case 'd':	goto yy40;
	default:	goto yy32;
	}
yy39:
	yych = *++YYCURSOR;
	switch (yych) {
	case '=':	goto yy41;
	default:	goto yy32;
	}
yy40:
	yych = *++YYCURSOR;
	switch (yych) {
	case '=':	goto yy42;
	default:	goto yy32;
	}
yy41:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy43;
	default:	goto yy32;
	}
yy42:
	yych = *++YYCURSOR;
	switch (yych) {
	case '"':	goto yy45;
	default:	goto yy32;
	}
yy43:
	++YYCURSOR;
#line 45 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
	{
	name_begin = YYCURSOR;
	while(*YYCURSOR != '\"')
		++YYCURSOR;
	*YYCURSOR = 0;
	++YYCURSOR;
	YYMARKER = YYCURSOR;
	goto restart;
}
#line 266 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
yy45:
	++YYCURSOR;
#line 35 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"
	{
	rid_begin = YYCURSOR;
	while(*YYCURSOR != '\"')
		++YYCURSOR;
	*YYCURSOR = 0;
	++YYCURSOR;
	YYMARKER = YYCURSOR;
	goto restart;
}
#line 279 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/gen/tdr_xlsx_reader_workbook_l.c"
}
#line 66 "/home/merlin/workspace/TDR-Merlin/tdr/protocol/tdr_xlsx_reader_workbook_l.re"

}



