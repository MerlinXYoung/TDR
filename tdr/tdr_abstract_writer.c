#include "tdr_abstract_writer.h"
#include "tdr_error_code.h"
#include <string.h>

void tdr_abstract_writer_init(tdr_abstract_writer_t *self)
{
	bzero(self, sizeof(tdr_abstract_writer_t));
	// self->write_struct_begin = NULL;
	// self->write_struct_end = NULL;
	// self->write_union_begin = NULL;
	// self->write_union_end = NULL;
	// self->write_enum_begin = NULL;
	// self->write_enum_end = NULL;
	// self->write_vector_begin = NULL;
	// self->write_vector_end = NULL;
	// self->write_field_begin = NULL;
	// self->write_field_end = NULL;
	// self->write_vector_element_begin = NULL;
	// self->write_vector_element_end = NULL;

	// self->write_int8 = NULL;
	// self->write_int16 = NULL;
	// self->write_int32 = NULL;
	// self->write_int64 = NULL;
	// self->write_uint8 = NULL;
	// self->write_uint16 = NULL;
	// self->write_uint32 = NULL;
	// self->write_uint64 = NULL;
	// self->write_char = NULL;
	// self->write_double = NULL;
	// self->write_string = NULL;

}

tdr_error_code_t tdr_write_struct_begin(tdr_abstract_writer_t *self, const char *struct_name)
{
	if(self->write_struct_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_struct_begin(self, struct_name);
}

tdr_error_code_t tdr_write_struct_end(tdr_abstract_writer_t *self, const char *struct_name)
{
	if(self->write_struct_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_struct_end(self, struct_name);
}

tdr_error_code_t tdr_write_union_begin(tdr_abstract_writer_t *self, const char *union_name)
{
	if(self->write_union_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_union_begin(self, union_name);
}

tdr_error_code_t tdr_write_union_end(tdr_abstract_writer_t *self, const char *union_name)
{
	if(self->write_union_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_union_end(self, union_name);
}


tdr_error_code_t tdr_write_enum_begin(tdr_abstract_writer_t *self, const char *enum_name)
{
	if(self->write_enum_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_enum_begin(self, enum_name);
}

tdr_error_code_t tdr_write_enum_end(tdr_abstract_writer_t *self, const char *enum_name)
{
	if(self->write_enum_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_enum_end(self, enum_name);
}

tdr_error_code_t tdr_write_vector_begin(tdr_abstract_writer_t *self, const char* vec_name)
{
	if(self->write_vector_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_vector_begin(self, vec_name);
}

tdr_error_code_t tdr_write_vector_end(tdr_abstract_writer_t *self, const char* vec_name)
{
	if(self->write_vector_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_vector_end(self, vec_name);
}

tdr_error_code_t tdr_write_field_begin(tdr_abstract_writer_t *self, const char *var_name)
{
	if(self->write_field_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_field_begin(self, var_name);
}

tdr_error_code_t tdr_write_vector_element_begin(tdr_abstract_writer_t *self, const char *var_name, uint32_t index)
{
	if(self->write_vector_element_begin == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_vector_element_begin(self, var_name, index);
}

tdr_error_code_t tdr_write_vector_element_end(tdr_abstract_writer_t *self, const char *var_name, uint32_t index)
{
	if(self->write_vector_element_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_vector_element_end(self, var_name, index);
}

tdr_error_code_t tdr_write_field_end(tdr_abstract_writer_t *self, const char *var_name)
{
	if(self->write_field_end == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_field_end(self, var_name);
}

tdr_error_code_t tdr_write_int8(tdr_abstract_writer_t *self, const int8_t *val)
{
	if(self->write_int8 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_int8(self, val);
}

tdr_error_code_t tdr_write_int16(tdr_abstract_writer_t *self, const int16_t *val)
{
	if(self->write_int16 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_int16(self, val);
}

tdr_error_code_t tdr_write_int32(tdr_abstract_writer_t *self, const int32_t *val)
{
	if(self->write_int32 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_int32(self, val);
}

tdr_error_code_t tdr_write_int64(tdr_abstract_writer_t *self, const int64_t *val)
{
	if(self->write_int64 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_int64(self, val);
}

tdr_error_code_t tdr_write_uint8(tdr_abstract_writer_t *self, const uint8_t *val)
{
	if(self->write_uint8 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_uint8(self, val);
}

tdr_error_code_t tdr_write_uint16(tdr_abstract_writer_t *self, const uint16_t *val)
{
	if(self->write_uint16 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_uint16(self, val);
}

tdr_error_code_t tdr_write_uint32(tdr_abstract_writer_t *self, const uint32_t *val)
{
	if(self->write_uint32 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_uint32(self, val);
}

tdr_error_code_t tdr_write_uint64(tdr_abstract_writer_t *self, const uint64_t *val)
{
	if(self->write_uint64 == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_uint64(self, val);
}

tdr_error_code_t tdr_write_char(tdr_abstract_writer_t *self, const char *val)
{
	if(self->write_char == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_char(self, val);
}

tdr_error_code_t tdr_write_double(tdr_abstract_writer_t *self, const double *val)
{
	if(self->write_double == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_double(self, val);
}

tdr_error_code_t tdr_write_string(tdr_abstract_writer_t *self, const char* str, uint32_t str_length)
{
	if(self->write_string == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_string(self, str, str_length);
}

tdr_error_code_t tdr_write_bool(tdr_abstract_writer_t *self, const bool *val)
{
	if(self->write_bool == NULL)
	{
		return E_TDR_NOERROR;
	}
	return self->write_bool(self, val);
}