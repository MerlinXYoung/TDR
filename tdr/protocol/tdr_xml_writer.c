#include "tdr_xml_writer.h"
#include "../tdr_error_code.h"
#include "../tdr_util.h"
#include "../tdr_abstract_writer.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>


void tdr_xml_writer_init(tdr_xml_writer_t *self, char *start, char*limit)
{
	tdr_abstract_writer_init(&self->super);


	self->cur = start;
	self->start = start;
	self->limit = limit;
	self->count = 0;

	self->super.write_struct_begin = tdr_xml_write_struct_begin;
	self->super.write_struct_end = tdr_xml_write_struct_end;
	self->super.write_union_begin = tdr_xml_write_struct_begin;
	self->super.write_union_end = tdr_xml_write_struct_end;
	self->super.write_enum_begin = tdr_xml_write_enum_begin;

	self->super.write_vector_begin = tdr_xml_write_vector_begin;
	self->super.write_vector_end = tdr_xml_write_vector_end;
	self->super.write_field_begin = tdr_xml_write_field_begin;
	self->super.write_field_end = tdr_xml_write_field_end;
	self->super.write_vector_element_begin = tdr_xml_write_vector_element_begin;
	self->super.write_vector_element_end = tdr_xml_write_vector_element_end;
	
	self->super.write_int8 = tdr_xml_write_int8;
	self->super.write_int16 = tdr_xml_write_int16;
	self->super.write_int32 = tdr_xml_write_int32;
	self->super.write_int64 = tdr_xml_write_int64;

	self->super.write_uint8 = tdr_xml_write_uint8;
	self->super.write_uint16 = tdr_xml_write_uint16;
	self->super.write_uint32 = tdr_xml_write_uint32;
	self->super.write_uint64 = tdr_xml_write_uint64;


	self->super.write_double = tdr_xml_write_double;
	self->super.write_string = tdr_xml_write_string;
	self->super.write_char = tdr_xml_write_char;


	self->skip_uint32_field_once = FALSE;
	self->ignore_int32_once = FALSE;
}

tdr_error_code_t tdr_xml_write_struct_begin(tdr_abstract_writer_t *super, const char *struct_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);	
	size_t slen, len;

	if(self->count == 0)
	{
		slen = strlen(struct_name);
		len = slen + 1 + 1;
		if((size_t)(self->limit - self->cur) < len)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto done;
		}
		*(self->cur++) = '<';
		memcpy(self->cur, struct_name, slen);
		self->cur += slen;
		*(self->cur++) = '>';
	}
	
done:
	return ret;
}

tdr_error_code_t tdr_xml_write_struct_end(tdr_abstract_writer_t *super, const char *struct_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);	
	size_t slen, len;
	if(self->count == 0)
	{
		slen = strlen(struct_name);
		len = slen + 3;
		if((size_t)(self->limit - self->cur) < len)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto done;
		}
		*(self->cur++) = '<';
		*(self->cur++) = '/';
		memcpy(self->cur, struct_name, slen);
		self->cur += slen;
		*(self->cur++) = '>';
	}
done:
	return ret;
}

tdr_error_code_t tdr_xml_write_enum_begin(tdr_abstract_writer_t *super, const char *enum_name)
{
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	TDR_UNUSED(enum_name);
	self->ignore_int32_once = TRUE;
	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xml_write_vector_begin(tdr_abstract_writer_t *super, const char* vec_name)
{
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);	
	tdr_error_code_t ret = tdr_xml_write_field_begin(super, vec_name);
	self->skip_uint32_field_once = TRUE;
	return ret;
}

tdr_error_code_t tdr_xml_write_vector_end(tdr_abstract_writer_t *super, const char* vec_name)
{
	return tdr_xml_write_field_end(super, vec_name);
}

tdr_error_code_t tdr_xml_write_field_begin(tdr_abstract_writer_t *super, const char *var_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	size_t len, slen;
	if(self->skip_uint32_field_once)
	{
		goto done;
	}
	++self->count;

	slen = strlen(var_name);
	len = 1 + slen + 1;
	
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}
	
	*(self->cur++) = '<';
	memcpy(self->cur, var_name, slen);
	self->cur+=slen;
	*(self->cur++) = '>';
done:
	return ret;
}

tdr_error_code_t tdr_xml_write_field_end(tdr_abstract_writer_t *super, const char *var_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);	
	size_t len, slen;
	if(self->skip_uint32_field_once)
	{
		self->skip_uint32_field_once = FALSE;
		goto done;
	}

	slen = strlen(var_name);
	len = 1 + 1 + slen + 1;
	
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}
	
	--(self->count);
	*(self->cur++) = '<';
	*(self->cur++) = '/';
	memcpy(self->cur, var_name, slen);
	self->cur += slen;
	*(self->cur++) = '>';

done:
	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xml_write_vector_element_begin(tdr_abstract_writer_t *super, const char *var_name, uint32_t index)
{	
	TDR_UNUSED(var_name);
	TDR_UNUSED(index);
	return tdr_xml_write_field_begin(super, "element");
}

tdr_error_code_t tdr_xml_write_vector_element_end(tdr_abstract_writer_t *super, const char *var_name, uint32_t index)
{
	TDR_UNUSED(var_name);
	TDR_UNUSED(index);
	return tdr_xml_write_field_end(super, "element");
}
#define TDR_XML_VALUE_LEN 128

tdr_error_code_t tdr_xml_write_double(tdr_abstract_writer_t *super, const double *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	char str[TDR_XML_VALUE_LEN];
	size_t len;
	len = snprintf(str, TDR_XML_VALUE_LEN, "%lf", *val);
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(self->cur, str, len);
	self->cur += len;
done:
	return ret;
}

tdr_error_code_t tdr_xml_write_int8(tdr_abstract_writer_t *super, const int8_t *val)
{
	int64_t v = *val;
	return tdr_xml_write_int64(super, &v);
}

tdr_error_code_t tdr_xml_write_int16(tdr_abstract_writer_t *super, const int16_t *val)
{
	int64_t v = *val;
	return tdr_xml_write_int64(super, &v);
}

tdr_error_code_t tdr_xml_write_int32(tdr_abstract_writer_t *super, const int32_t *val)
{
	int64_t v;
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	if(self->ignore_int32_once)
	{
		self->ignore_int32_once = FALSE;
		ret = E_TDR_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	v = *val;
	ret = tdr_xml_write_int64(super, &v);
done:
	return ret;
}

tdr_error_code_t tdr_xml_write_int64(tdr_abstract_writer_t *super, const int64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	char str[TDR_XML_VALUE_LEN];
	size_t len;
	len = snprintf(str, TDR_XML_VALUE_LEN, "%"PRIi64, *val);
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(self->cur, str, len);
	self->cur += len;
done:
	return ret;
}


tdr_error_code_t tdr_xml_write_uint8(tdr_abstract_writer_t *super, const uint8_t *val)
{
	uint64_t v = *val;
	return tdr_xml_write_uint64(super, &v);
}

tdr_error_code_t tdr_xml_write_uint16(tdr_abstract_writer_t *super, const uint16_t *val)
{
	uint64_t v = *val;
	
	return tdr_xml_write_uint64(super, &v);
}

tdr_error_code_t tdr_xml_write_uint32(tdr_abstract_writer_t *super, const uint32_t *val)
{
	uint64_t v = *val;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	if(self->skip_uint32_field_once)
	{
		goto done;
	}
	return tdr_xml_write_uint64(super, &v);
done:
	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xml_write_uint64(tdr_abstract_writer_t *super, const uint64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	char str[TDR_XML_VALUE_LEN];
	size_t len;
	len = snprintf(str, TDR_XML_VALUE_LEN, "%"PRIu64, *val);
	if((size_t)(self->limit - self->cur) < len)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}
	memcpy(self->cur, str, len);
	self->cur += len;
done:
	return ret;
}

static char* write_char(char *cur, char *limit, char c)
{
	const char *str = NULL;
	size_t len = 0;
	switch (c)
	{
	case '<':
		str = "&lt";
		len = 3;
		break;
	case '>':
		str = "&gt";
		len = 3;
		break;
	case '&':
		str = "&amp";
		len = 4;
		break;
	case '\'':
		str = "&apos";
		len = 6;
		break;
	case '\"':
		str = "&quot";
		len = 5;
		break;
	default:
		str = &c;
		len = 1;
		break;
	}

	if((size_t)(limit - cur) < len)
	{
		return NULL;
	}
	memcpy(cur, str, len);
	return cur + len;
}

tdr_error_code_t tdr_xml_write_string(tdr_abstract_writer_t *super, const char* str, uint32_t str_length)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	const char *i;	
	TDR_UNUSED(str_length);
	for(i = str; *i ; ++i)
	{
		char *next = write_char(self->cur, self->limit, *i);
		if(next == NULL)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto done;
		}
		self->cur = next;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xml_write_char(tdr_abstract_writer_t *super, const char *val)
{
	tdr_xml_writer_t *self = TDR_CONTAINER_OF(super, tdr_xml_writer_t, super);
	char *next = write_char(self->cur, self->limit, *val);
	if(next == NULL)
	{
		return E_TDR_OUT_OF_MEMORY;
	}

	self->cur = next;
	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xml_write_bool(tdr_abstract_writer_t *super, const bool *val){
	return tdr_xml_write_string(super, *val?"True":"False", 0);
}
