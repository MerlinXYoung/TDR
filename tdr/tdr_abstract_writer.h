#ifndef _H_TDR_ABSTRACT_WRITER
#define _H_TDR_ABSTRACT_WRITER

#ifdef  __cplusplus
extern "C" {
#endif

#include "tdr_platform.h"
#include "tdr_error_code.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct tdr_abstract_writer_s tdr_abstract_writer_t;
struct tdr_abstract_writer_s
{
	tdr_error_code_t (*write_struct_begin)(tdr_abstract_writer_t *self, const char *struct_name);
	tdr_error_code_t (*write_struct_end)(tdr_abstract_writer_t *self, const char *struct_name);
	tdr_error_code_t (*write_union_begin)(tdr_abstract_writer_t *self, const char *union_name);
	tdr_error_code_t (*write_union_end)(tdr_abstract_writer_t *self, const char *union_name);
	tdr_error_code_t (*write_enum_begin)(tdr_abstract_writer_t *self, const char *enum_name);
	tdr_error_code_t (*write_enum_end)(tdr_abstract_writer_t *self, const char *enum_name);
	tdr_error_code_t (*write_vector_begin)(tdr_abstract_writer_t *self, const char* vec_name);
	tdr_error_code_t (*write_vector_end)(tdr_abstract_writer_t *self, const char* vec_name);
	tdr_error_code_t (*write_field_begin)(tdr_abstract_writer_t *self, const char *var_name);
	tdr_error_code_t (*write_field_end)(tdr_abstract_writer_t *self, const char *var_name);	
	tdr_error_code_t (*write_vector_element_begin)(tdr_abstract_writer_t *self, const char *var_name, uint32_t index);
	tdr_error_code_t (*write_vector_element_end)(tdr_abstract_writer_t *self, const char *var_name, uint32_t index);

	tdr_error_code_t (*write_int8)(tdr_abstract_writer_t *self, const int8_t *val);
	tdr_error_code_t (*write_int16)(tdr_abstract_writer_t *self, const int16_t *val);
	tdr_error_code_t (*write_int32)(tdr_abstract_writer_t *self, const int32_t *val);
	tdr_error_code_t (*write_int64)(tdr_abstract_writer_t *self, const int64_t *val);
	tdr_error_code_t (*write_uint8)(tdr_abstract_writer_t *self, const uint8_t *val);
	tdr_error_code_t (*write_uint16)(tdr_abstract_writer_t *self, const uint16_t *val);
	tdr_error_code_t (*write_uint32)(tdr_abstract_writer_t *self, const uint32_t *val);
	tdr_error_code_t (*write_uint64)(tdr_abstract_writer_t *self, const uint64_t *val);
	tdr_error_code_t (*write_char)(tdr_abstract_writer_t *self, const char *val);
	tdr_error_code_t (*write_bool)(tdr_abstract_writer_t *self, const bool *val);
	tdr_error_code_t (*write_double)(tdr_abstract_writer_t *self, const double *val);
	tdr_error_code_t (*write_string)(tdr_abstract_writer_t *self, const char* str, uint32_t str_length);
};

void tdr_abstract_writer_init(tdr_abstract_writer_t *self);

tdr_error_code_t tdr_write_struct_begin(tdr_abstract_writer_t *self, const char *struct_name);
tdr_error_code_t tdr_write_struct_end(tdr_abstract_writer_t *self, const char *struct_name);
tdr_error_code_t tdr_write_union_begin(tdr_abstract_writer_t *self, const char *union_name);
tdr_error_code_t tdr_write_union_end(tdr_abstract_writer_t *self, const char *union_name);
tdr_error_code_t tdr_write_enum_begin(tdr_abstract_writer_t *self, const char *enum_name);
tdr_error_code_t tdr_write_enum_end(tdr_abstract_writer_t *self, const char *enum_name);
tdr_error_code_t tdr_write_vector_begin(tdr_abstract_writer_t *self, const char* vec_name);
tdr_error_code_t tdr_write_vector_end(tdr_abstract_writer_t *self, const char* vec_name);
tdr_error_code_t tdr_write_field_begin(tdr_abstract_writer_t *self, const char *var_name);
tdr_error_code_t tdr_write_field_end(tdr_abstract_writer_t *self, const char *var_name);
tdr_error_code_t tdr_write_vector_element_begin(tdr_abstract_writer_t *self, const char *var_name, uint32_t index);
tdr_error_code_t tdr_write_vector_element_end(tdr_abstract_writer_t *self, const char *var_name, uint32_t index);

tdr_error_code_t tdr_write_int8(tdr_abstract_writer_t *self, const int8_t *val);
tdr_error_code_t tdr_write_int16(tdr_abstract_writer_t *self, const int16_t *val);
tdr_error_code_t tdr_write_int32(tdr_abstract_writer_t *self, const int32_t *val);
tdr_error_code_t tdr_write_int64(tdr_abstract_writer_t *self, const int64_t *val);
tdr_error_code_t tdr_write_uint8(tdr_abstract_writer_t *self, const uint8_t *val);
tdr_error_code_t tdr_write_uint16(tdr_abstract_writer_t *self, const uint16_t *val);
tdr_error_code_t tdr_write_uint32(tdr_abstract_writer_t *self, const uint32_t *val);
tdr_error_code_t tdr_write_uint64(tdr_abstract_writer_t *self, const uint64_t *val);
tdr_error_code_t tdr_write_char(tdr_abstract_writer_t *self, const char *val);	
tdr_error_code_t tdr_write_bool(tdr_abstract_writer_t *self, const bool *val);
tdr_error_code_t tdr_write_double(tdr_abstract_writer_t *self, const double *val);
tdr_error_code_t tdr_write_string(tdr_abstract_writer_t *self, const char *str, uint32_t str_length);

#ifdef  __cplusplus
}
#endif

#endif

