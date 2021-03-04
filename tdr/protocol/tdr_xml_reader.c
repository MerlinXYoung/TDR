#include "tdr_xml_reader.h"
#include "../tdr_util.h"
#include "../tdr_abstract_reader.h"
#include "../tdr_error_code.h"
#include "gen/tdr_xml_reader_l.h"

#include "tdr_xml_reader_scanner.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

tdr_error_code_t tdr_xml_reader_push_file(tdr_xml_reader_t *self, const char *file_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	FILE* fin;
	size_t file_size;
	char c;
	char *start, *curr, *limit;
	size_t i;

	for(i = 0; i < self->scanner_context_stack_num ; ++i)
	{
		if(self->scanner_context_stack[i].filecontent_ptr)
		{
			if(strcmp(self->scanner_context_stack[i].yylloc.file_name, file_name) == 0)
			{
				ret = E_TDR_FILE_IS_ALREADY_ON_THE_STACK;
				goto done;
			}
		}		
	}

	

	fin = fopen(file_name, "rb");
	if(fin == NULL)
	{
		size_t i;
		char fn[TDR_MAX_PATH_LENGTH];
		for(i = 0; i < self->include_num; ++i)
		{
			snprintf(fn, TDR_MAX_PATH_LENGTH, "%s/%s", self->include[i], file_name);
			fin = fopen(fn, "rb");
			if(fin != NULL)
			{
				break;
			}
		}		
	}
	if(fin == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}	

	fseek(fin, 0, SEEK_END);
	file_size = ftell(fin);
	fseek(fin, 0, SEEK_SET);
	start = (char*)malloc(file_size);
	if(start == NULL)
	{
		ret = E_TDR_OUT_OF_MEMORY;		
		goto free_file;
	}
	curr = start;
	limit = start + file_size;

	while((c = (char)fgetc(fin)) != EOF)
	{
		if(curr == limit)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto free_buff;
		}
		*curr = c;
		++curr;
	}
	
	ret = tdr_xml_reader_push_buff(self, start, limit);
	if(ret != E_TDR_NOERROR)
	{
		goto free_buff;
	}
	fclose(fin);

	strncpy(self->scanner_context_stack[self->scanner_context_stack_num - 1].yylloc.file_name, file_name, TDR_MAX_PATH_LENGTH - 1);
	self->scanner_context_stack[self->scanner_context_stack_num - 1].filecontent_ptr = start;
	
	return E_TDR_NOERROR;
free_buff:
	free(start);
free_file:
	fclose(fin);
done:
	return ret;
}

tdr_error_code_t tdr_xml_reader_push_buff(tdr_xml_reader_t *self, const char *xml_start, const char* xml_limit)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_reader_scanner_context_t *scanner = NULL;
	if(self->scanner_context_stack_num >= TDR_XML_MAX_DEEP)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto ERROR_RET;
	}
	scanner = &self->scanner_context_stack[self->scanner_context_stack_num];
	scanner->yy_state = yycINITIAL;
	scanner->yy_start = xml_start;
	scanner->yy_limit = xml_limit;
	scanner->yy_cursor = scanner->yy_start;	
	scanner->yy_marker = scanner->yy_start;
	scanner->yy_last = scanner->yy_start;
	scanner->filecontent_ptr = NULL;
	scanner->yylineno = 1;
	scanner->yycolumn = 1;	
	++self->scanner_context_stack_num;
	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}

void tdr_xml_reader_pop_file(tdr_xml_reader_t *self)
{
	tdr_xml_reader_pop_buff(self);
	free(self->scanner_context_stack[self->scanner_context_stack_num].filecontent_ptr);
}

void tdr_xml_reader_pop_buff(tdr_xml_reader_t *self)
{
	--self->scanner_context_stack_num;
}

void tdr_xml_reader_init(tdr_xml_reader_t *self)
{
	tdr_abstract_reader_init(&self->super);

	self->super.read_struct_begin = tdr_xml_read_struct_begin;
	self->super.read_struct_end = tdr_xml_read_struct_end;
	self->super.read_union_begin = tdr_xml_read_struct_begin;
	self->super.read_union_end = tdr_xml_read_struct_end;
	self->super.read_enum_begin = tdr_xml_read_enum_begin;

	self->super.read_vector_begin = tdr_xml_read_vector_begin;
	self->super.read_vector_end = tdr_xml_read_vector_end;
	self->super.read_field_begin = tdr_xml_read_field_begin;
	self->super.read_field_end = tdr_xml_read_field_end;
	self->super.read_vector_element_begin = tdr_xml_read_vector_element_begin;
	self->super.read_vector_element_end = tdr_xml_read_vector_element_end;

	self->super.read_int8 = tdr_xml_read_int8;
	self->super.read_int16 = tdr_xml_read_int16;
	self->super.read_int32 = tdr_xml_read_int32;
	self->super.read_int64 = tdr_xml_read_int64;

	self->super.read_uint8 = tdr_xml_read_uint8;
	self->super.read_uint16 = tdr_xml_read_uint16;
	self->super.read_uint32 = tdr_xml_read_uint32;
	self->super.read_uint64 = tdr_xml_read_uint64;

	self->super.read_double = tdr_xml_read_double;
	self->super.read_string = tdr_xml_read_string;
	self->super.read_char = tdr_xml_read_char;
	self->super.read_bool = tdr_xml_read_bool;

	self->pre_read_uint32_field_once = FALSE;
	self->ignore_int32_once = FALSE;
	self->scanner_context_stack_num = 0;
	self->include_num = 0;
	self->struct_deep = 0;
}

 tdr_error_code_t tdr_xml_add_include(tdr_xml_reader_t *self, const char *path)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	if(self->include_num >= TDR_XML_MAX_INCLUDE)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}
	self->include[self->include_num] = path;
	++self->include_num;
done:
	return ret;
}

tdr_error_code_t tdr_xml_read_struct_begin(tdr_abstract_reader_t *super, const char *struct_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);	
	tdr_xml_reader_token_t token;

	if(self->struct_deep != 0)
	{
		++self->struct_deep;
		goto done;
	}

	token = tdr_xml_reader_get_token(self);
	if(token != tok_tag_begin)
	{
		if(self->error_code == E_TDR_NOERROR)
		{
			ret = E_TDR_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto done;
	}

	if(strcmp(struct_name, self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name) != 0)
	{
		ret = E_TDR_MISMATCH;
		goto done;
	}
	++self->struct_deep;
done:	
	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xml_read_struct_end(tdr_abstract_reader_t *super, const char *struct_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	tdr_xml_reader_token_t token;

	if(self->struct_deep != 0)
	{
		--self->struct_deep;
		goto done;		
	}

	token = tdr_xml_reader_get_token(self);
	if(token != tok_tag_end)
	{
		if(self->error_code == E_TDR_NOERROR)
		{
			ret = E_TDR_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto done;
	}
	if(strcmp(struct_name, self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name) != 0)
	{
		ret = E_TDR_MISMATCH;
		goto done;
	}

	--self->struct_deep;	
done:	
	return ret;
}

tdr_error_code_t tdr_xml_read_enum_begin(tdr_abstract_reader_t *super, const char *enum_name)
{
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	TDR_UNUSED(enum_name);

	self->ignore_int32_once = TRUE;

	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xml_read_vector_begin(tdr_abstract_reader_t *super, const char *vec_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	uint32_t level;
	uint32_t count;
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	//copy一份是因为统计数组的长度。
	tdr_xml_reader_t self_copy = *self;
	TDR_UNUSED(vec_name);
	count = 0;
	level = 0;
	do
	{
		tdr_xml_reader_token_t token = tdr_xml_reader_get_token(&self_copy);
		switch(token)
		{
			case tok_tag_begin:				
				if(level == 1)
				{
					++count;
				}
				++level;
				break;
			case tok_tag_end:
				--level;
				break;
			default:				
				if(self->error_code == E_TDR_NOERROR)
				{
					ret = E_TDR_BAD_FILE;
				}
				else
				{
					ret = self->error_code;
				}
				goto ERROR_RET;
		}
	}while(level != 0);

	ret = tdr_xml_read_field_begin(super, vec_name);
	self->pre_read_uint32_field_once = TRUE;
	self->ui32 = count;

	return ret;
ERROR_RET:
	return ret;
}

tdr_error_code_t tdr_xml_read_vector_end(tdr_abstract_reader_t *super, const char *vec_name)
{
	return tdr_xml_read_field_end(super, vec_name);
}

tdr_error_code_t tdr_xml_read_field_begin(tdr_abstract_reader_t *super, const char *var_name)
{	
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	tdr_error_code_t ret;
	tdr_xml_reader_token_t token;	

	if(self->pre_read_uint32_field_once)
	{
		goto done;
	}
	token = tdr_xml_reader_get_token(self);
	if(token != tok_tag_begin)
	{
		if(self->error_code == E_TDR_NOERROR)
		{
			ret = E_TDR_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto ERROR_RET;
	}
	if(strcmp(self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name, var_name) != 0)
	{
		ret = E_TDR_MISMATCH;
		goto ERROR_RET;
	}

done:
	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}

tdr_error_code_t tdr_xml_read_field_end(tdr_abstract_reader_t *super, const char *var_name)
{
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);	
	tdr_error_code_t ret;
	tdr_xml_reader_token_t token;

	if(self->pre_read_uint32_field_once)
	{
		self->pre_read_uint32_field_once = FALSE;
		goto done;
	}
	token = tdr_xml_reader_get_token(self);
	if(token != tok_tag_end)
	{
		if(self->error_code == E_TDR_NOERROR)
		{
			ret = E_TDR_BAD_FILE;
		}
		else
		{
			ret = self->error_code;
		}
		goto ERROR_RET;
	}
	if(strcmp(self->scanner_context_stack[self->scanner_context_stack_num - 1].tag_name, var_name) != 0)
	{
		ret = E_TDR_MISMATCH;
		goto ERROR_RET;
	}

done:
	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}

tdr_error_code_t tdr_xml_read_vector_element_begin(tdr_abstract_reader_t *self, const char *var_name, uint32_t index)
{
	TDR_UNUSED(var_name);
	TDR_UNUSED(index);
	return tdr_xml_read_field_begin(self, "element");
}

tdr_error_code_t tdr_xml_read_vector_element_end(tdr_abstract_reader_t *self, const char *var_name, uint32_t index)
{
	TDR_UNUSED(var_name);
	TDR_UNUSED(index);
	return tdr_xml_read_field_end(self, "element");
}

tdr_error_code_t tdr_xml_read_double(tdr_abstract_reader_t *super, double *val)
{
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	tdr_error_code_t ret;
	
	errno = 0;
	*val = strtod(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin, NULL);
	if(errno != 0)
	{
		ret = E_TDR_ERRNO;
		goto ERROR_RET;
	}

	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}

tdr_error_code_t tdr_xml_read_int8(tdr_abstract_reader_t *super, int8_t *val)
{
	int64_t i64;
	tdr_error_code_t ret = tdr_xml_read_int64(super, &i64);
	*val = (int8_t)i64;
	if(*val != i64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
	return ret;
}

tdr_error_code_t tdr_xml_read_int16(tdr_abstract_reader_t *super, int16_t *val)
{
	int64_t i64;
	tdr_error_code_t ret = tdr_xml_read_int64(super, &i64);
	*val = (int16_t)i64;
	if(*val != i64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
	return ret;
}

tdr_error_code_t tdr_xml_read_int32(tdr_abstract_reader_t *super, int32_t *val)
{
	int64_t i64;
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = FALSE;
		ret = E_TDR_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	
	ret = tdr_xml_read_int64(super, &i64);
	*val = (int32_t)i64;
	if(*val != i64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xml_read_int64(tdr_abstract_reader_t *super, int64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);

	errno = 0;
	*val = strtoll(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TDR_ERRNO;
		goto ERROR_RET;
	}

	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}

tdr_error_code_t tdr_xml_read_uint8(tdr_abstract_reader_t *super, uint8_t *val)
{
	uint64_t ui64;
	tdr_error_code_t ret = tdr_xml_read_uint64(super, &ui64);
	*val = (uint8_t)ui64;
	if(*val != ui64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
	return ret;
}

tdr_error_code_t tdr_xml_read_uint16(tdr_abstract_reader_t *super, uint16_t *val)
{	
	uint64_t ui64;
	tdr_error_code_t ret;

	ret = tdr_xml_read_uint64(super, &ui64);

	*val = (uint16_t)ui64;
	if(*val != ui64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
	return ret;
}

tdr_error_code_t tdr_xml_read_uint32(tdr_abstract_reader_t *super, uint32_t *val)
{
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	uint64_t ui64;
	tdr_error_code_t ret;

	if(self->pre_read_uint32_field_once)
	{
		*val = self->ui32;
		ret = E_TDR_NOERROR;
	}
	else
	{
		ret = tdr_xml_read_uint64(super, &ui64);
		*val = (uint32_t)ui64;
		if(*val != ui64)
		{
			return E_TDR_INTEGER_OVERFLOW;
		}
	}	
	return ret;
}

tdr_error_code_t tdr_xml_read_uint64(tdr_abstract_reader_t *super, uint64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	errno = 0;
	*val = strtoull(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TDR_ERRNO;
		goto ERROR_RET;
	}

	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}


const char* tdr_xml_str2c(const char* curr, const char* limit, char *ch)
{
	char c;
	if(curr >= limit)
	{
		goto ERROR_RET;
	}
	c = *curr++;
	
	if(c == '&')
	{
		char c2;
		if(curr >= limit)
		{
			goto ERROR_RET;
		}
		c2 = *curr++;
		if(c2 == 'l')
		{
			//&lt
			*ch = '<';
			curr += 2;
			if(curr >= limit)
			{
				goto ERROR_RET;
			}
		}
		else if(c2 == 'g')
		{
			//&gt
			*ch = '>';
			curr += 2;
			if(curr >= limit)
			{
				goto ERROR_RET;
			}
		}
		else
		{
			char c3;
			if(curr >= limit)
			{
				goto ERROR_RET;
			}
			c3 = *curr++;
			if(c3 == 'm')
			{
				//&amp
				*ch = '&';
				curr += 2;
				if(curr >= limit)
				{
					goto ERROR_RET;
				}
			}
			else if(c3 == 'p')
			{
				//&apos
				*ch = '\'';
				curr += 3;
				if(curr >= limit)
				{
					goto ERROR_RET;
				}
			}
			else if(c3 == 'u')
			{
				//&auot
				*ch = '\"';
				curr += 3;
				if(curr >= limit)
				{
					goto ERROR_RET;
				}
			}
		}
	}
	else
	{
		*ch = c;
	}

	return curr;
ERROR_RET:
	return NULL;
}

tdr_error_code_t tdr_xml_read_char(tdr_abstract_reader_t *super, char *val)
{
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	const char* ret = tdr_xml_str2c(self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin
		, self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_limit, val);
	self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_cursor = ret;
	if(ret == NULL)
	{
		return E_TDR_OUT_OF_MEMORY;
	}
	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xml_read_bool(tdr_abstract_reader_t *super, bool *val)
{
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	uint32_t len = 0;
	tdr_error_code_t ret;
	const uint32_t str_len = 64;
	char str[str_len];
	
	const char* curr = self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin;
	const char* limit = self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_limit;
	while(curr < limit)
	{
		char c = 0;
		if(*curr == '<')
		{
			self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_cursor = curr - 1;
			break;
		}
		curr = tdr_xml_str2c(curr, limit, &c);
		if(curr == NULL)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto ERROR_RET;
		}

		if(len >= str_len)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto ERROR_RET;
		}
		str[len++] = c;		
	}
	str[len] = 0;
	if(strcmp(str, "0")==0){ 
		*val = false;
	}
	else if(strcmp(str, "1")== 0){ 
		*val = true;
	}
	else if(strcmp(str, "true")==0){ 
		*val = true;
	}
	else if(strcmp(str, "false")==0){ 
		*val = false;
	}
	else{
		ret = E_TDR_ERROR;
		goto ERROR_RET;
	}
	

	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}

tdr_error_code_t tdr_xml_read_string(tdr_abstract_reader_t *super, char *str, uint32_t str_len)
{
	tdr_xml_reader_t *self = TDR_CONTAINER_OF(super, tdr_xml_reader_t, super);
	uint32_t len = 0;
	tdr_error_code_t ret;
	const char* curr = self->scanner_context_stack[self->scanner_context_stack_num - 1].content_begin;
	const char* limit = self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_limit;
	while(curr < limit)
	{
		char c = 0;
		if(*curr == '<')
		{
			self->scanner_context_stack[self->scanner_context_stack_num - 1].yy_cursor = curr - 1;
			break;
		}
		curr = tdr_xml_str2c(curr, limit, &c);
		if(curr == NULL)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto ERROR_RET;
		}

		if(len >= str_len)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto ERROR_RET;
		}
		str[len++] = c;		
	}
	str[len] = 0;
	

	return E_TDR_NOERROR;
ERROR_RET:
	return ret;
}

const tdr_xml_reader_yyltype_t* tdr_xml_current_location(tdr_xml_reader_t *self)
{
	if(self->scanner_context_stack_num > 0)
	{
		return &self->scanner_context_stack[self->scanner_context_stack_num - 1].yylloc;
	}
	else
	{
		return NULL;
	}
}
