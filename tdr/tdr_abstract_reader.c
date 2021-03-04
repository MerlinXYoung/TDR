#include "tdr_abstract_reader.h"
#include "tdr_platform.h"
#include "tdr_error_code.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void tdr_abstract_reader_init(tdr_abstract_reader_t *self)
{
	bzero(self, sizeof(tdr_abstract_reader_t));
	self->name_ptr = self->name;
	// self->enable_name = FALSE;
	// self->name[0] = 0;
	// self->name_ptr = self->name;

	// self->read_struct_begin = NULL;
	// self->read_struct_end = NULL;
	// self->read_enum_begin = NULL;
	// self->read_enum_end = NULL;
	// self->read_union_begin = NULL;
	// self->read_union_end = NULL;
	// self->read_vector_begin = NULL;
	// self->read_vector_end = NULL;
	// self->read_field_begin = NULL;
	// self->read_field_end = NULL;
	// self->read_vector_element_begin = NULL;
	// self->read_vector_element_end = NULL;

	// self->read_int8 = NULL;
	// self->read_int16 = NULL;
	// self->read_int32 = NULL;
	// self->read_int64 = NULL;
	// self->read_uint8 = NULL;
	// self->read_uint16 = NULL;
	// self->read_uint32 = NULL;
	// self->read_uint64 = NULL;
	// self->read_char = NULL;
	// self->read_double = NULL;
	// self->read_string = NULL;
}

tdr_error_code_t tdr_read_struct_begin(tdr_abstract_reader_t *self, const char *struct_name)
{
	if(self->read_struct_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_struct_begin(self, struct_name);
}

tdr_error_code_t tdr_read_struct_end(tdr_abstract_reader_t *self, const char *struct_name)
{
	if(self->read_struct_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_struct_end(self, struct_name);
}


tdr_error_code_t tdr_read_union_begin(tdr_abstract_reader_t *self, const char *union_name)
{
	if(self->read_union_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_union_begin(self, union_name);
}

tdr_error_code_t tdr_read_union_end(tdr_abstract_reader_t *self, const char *union_name)
{
	if(self->read_union_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_union_end(self, union_name);
}


tdr_error_code_t tdr_read_enum_begin(tdr_abstract_reader_t *self, const char *enum_name)
{
	if(self->read_enum_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_enum_begin(self, enum_name);
}

tdr_error_code_t tdr_read_enum_end(tdr_abstract_reader_t *self, const char *enum_name)
{
	if(self->read_enum_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_enum_end(self, enum_name);
}

static tdr_error_code_t add_name(tdr_abstract_reader_t *self, const char *name)
{
	if(self->enable_name)
	{
		int len = snprintf(self->name_ptr, TDR_READER_NAME_LENGTH - (self->name_ptr - self->name)
			, ".%s", name);

		if(len < 0)
		{
			return E_TDR_ERROR;
		}

		self->name_ptr += len;
	}

	return E_TDR_NOERROR;
}

static tdr_error_code_t del_name(tdr_abstract_reader_t *self, const char *name)
{
	if(self->enable_name)
	{	
		char curr_name[TDR_READER_NAME_LENGTH];	
		int len = snprintf(curr_name, TDR_READER_NAME_LENGTH - (self->name_ptr - self->name)
			, ".%s", name);

		if(len < 0)
		{
			return E_TDR_ERROR;
		}
		self->name_ptr -= len;
	}
	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_read_vector_begin(tdr_abstract_reader_t *self, const char* vec_name)
{
	tdr_error_code_t ret;
	ret = add_name(self, vec_name);
	if(ret != E_TDR_NOERROR)
	{
		return ret;
	}
	if(self->read_vector_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_vector_begin(self, vec_name);
}

tdr_error_code_t tdr_read_vector_end(tdr_abstract_reader_t *self, const char* vec_name)
{
	tdr_error_code_t ret;
	ret = del_name(self, vec_name);
	if(ret != E_TDR_NOERROR)
	{
		return ret;
	}
	if(self->read_vector_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_vector_end(self, vec_name);
}



tdr_error_code_t tdr_read_field_begin(tdr_abstract_reader_t *self, const char *var_name)
{
	tdr_error_code_t ret;
	ret = add_name(self, var_name);
	if(ret != E_TDR_NOERROR)
	{
		return ret;
	}
	if(self->read_field_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_field_begin(self, var_name);
}

tdr_error_code_t tdr_read_field_end(tdr_abstract_reader_t *self, const char *var_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;

	if(self->read_field_end != NULL)
	{
		ret = self->read_field_end(self, var_name);
		if(ret != E_TDR_NOERROR)
		{
			return ret;
		}
	}	
	ret = del_name(self, var_name);
	return ret;
}

tdr_error_code_t tdr_read_vector_element_begin(tdr_abstract_reader_t *self, const char *var_name, uint32_t index)
{
	if(self->enable_name)
	{
		int len = snprintf(self->name_ptr, TDR_READER_NAME_LENGTH - (self->name_ptr - self->name)
				, "[%u]", index);

		if(len < 0)
		{
			return E_TDR_ERROR;
		}
		self->name_ptr += len;
	}
	
	if(self->read_vector_element_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_vector_element_begin(self, var_name, index);
}

tdr_error_code_t tdr_read_vector_element_end(tdr_abstract_reader_t *self, const char *var_name, uint32_t index)
{
	tdr_error_code_t ret = E_TDR_NOERROR;

	if(self->read_vector_element_end != NULL)
	{
		ret = self->read_vector_element_end(self, var_name, index);
	}	

	if(self->enable_name)
	{		
		char curr_name[TDR_READER_NAME_LENGTH];
		int len = snprintf(curr_name, TDR_READER_NAME_LENGTH, "[%u]", index);
		
		if(len < 0)
		{
			return E_TDR_ERROR;
		}
		self->name_ptr -= len;
	}
	return ret;
}

tdr_error_code_t tdr_read_int8(tdr_abstract_reader_t *self, int8_t *val)
{
	if(self->read_int8 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_int8(self, val);
}

tdr_error_code_t tdr_read_int16(tdr_abstract_reader_t *self, int16_t *val)
{
	if(self->read_int16 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_int16(self, val);
}

tdr_error_code_t tdr_read_int32(tdr_abstract_reader_t *self, int32_t *val)
{
	if(self->read_int32 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_int32(self, val);
}

tdr_error_code_t tdr_read_int64(tdr_abstract_reader_t *self, int64_t *val)
{
	if(self->read_int64 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_int64(self, val);
}

tdr_error_code_t tdr_read_uint8(tdr_abstract_reader_t *self, uint8_t *val)
{
	if(self->read_uint8 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_uint8(self, val);
}

tdr_error_code_t tdr_read_uint16(tdr_abstract_reader_t *self, uint16_t *val)
{
	if(self->read_uint16 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_uint16(self, val);
}

tdr_error_code_t tdr_read_uint32(tdr_abstract_reader_t *self, uint32_t *val)
{
	if(self->read_uint32 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_uint32(self, val);
}

tdr_error_code_t tdr_read_uint64(tdr_abstract_reader_t *self, uint64_t *val)
{
	if(self->read_uint64 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_uint64(self, val);
}

tdr_error_code_t tdr_read_char(tdr_abstract_reader_t *self, char *val)
{
	if(self->read_char == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_char(self, val);
}

tdr_error_code_t tdr_read_bool(tdr_abstract_reader_t *self, bool *val)
{
	if(self->read_bool == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_bool(self, val);
}

tdr_error_code_t tdr_read_double(tdr_abstract_reader_t *self, double *val)
{
	if(self->read_double == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_double(self, val);
}

tdr_error_code_t tdr_read_string(tdr_abstract_reader_t *self, char* str, uint32_t str_length)
{
	if(self->read_string == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->read_string(self, str, str_length);
}

