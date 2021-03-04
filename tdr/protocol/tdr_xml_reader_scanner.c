#include "tdr_xml_reader_scanner.h"

#include <string.h>

void tdr_xml_reader_locate(tdr_xml_reader_t *self)
{
	const char *i;
	tdr_xml_reader_scanner_context_t *sp = &self->scanner_context_stack[self->scanner_context_stack_num - 1];
	for(i = sp->yy_last; i < sp->yy_cursor;++i)
	{
		if(*i == '\n')
		{
			++(sp->yylineno);
			sp->yycolumn = 1;
		}		
		else if(*i == '\r')
		{
			sp->yycolumn = 1;		
		}
		else
		{
			++(sp->yycolumn);
		}
	}
	sp->yy_last = sp->yy_cursor;
}

tdr_xml_reader_token_t tdr_xml_reader_get_token(tdr_xml_reader_t *self)
{
	tdr_xml_reader_token_t token = tok_end;
	tdr_xml_reader_scanner_context_t *sp = NULL;

	token = tdr_xml_reader_scan(self);
	if(token == tok_end)
	{
		goto done;
	}
	sp = &self->scanner_context_stack[self->scanner_context_stack_num - 1];
	sp->yylloc.last_line = sp->yylineno;
	sp->yylloc.last_column = sp->yycolumn;

	switch(token)
	{
	case tok_tag_begin:
		{
			if(sp->yy_leng - 2 >= TDR_MAX_LENGTH_OF_IDENTIFIER)
			{
				goto ERROR_RET;
			}
			memcpy(sp->tag_name, sp->yy_text + 1, sp->yy_leng - 2);
			sp->tag_name[sp->yy_leng - 2] = 0;

			//记录值开始的位置
			sp->content_begin = sp->yy_cursor;			
			break;
		}
	case tok_tag_end:
		{
			if(sp->yy_leng - 3 >= TDR_MAX_LENGTH_OF_IDENTIFIER)
			{
				goto ERROR_RET;
			}
			memcpy(sp->tag_name, sp->yy_text + 2, sp->yy_leng - 3);
			sp->tag_name[sp->yy_leng - 3] = 0;
			break;
		}
	default:
		goto ERROR_RET;
	}
done:
	return token;

ERROR_RET:
	return tok_error;
}
