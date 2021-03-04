#ifndef _H_TDR_ABSTRACT_READR
#define _H_TDR_ABSTRACT_READR

#ifdef  __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include "tdr_error_code.h"

#define TDR_READER_NAME_LENGTH 65536

typedef struct tdr_abstract_reader_s tdr_abstract_reader_t;
struct tdr_abstract_reader_s
{
	int enable_name;
	char name[TDR_READER_NAME_LENGTH];
	char *name_ptr;

	tdr_error_code_t (*read_struct_begin)(tdr_abstract_reader_t *self, const char *struct_name);
	tdr_error_code_t (*read_struct_end)(tdr_abstract_reader_t *self, const char *struct_name);
	tdr_error_code_t (*read_union_begin)(tdr_abstract_reader_t *self, const char *union_name);
	tdr_error_code_t (*read_union_end)(tdr_abstract_reader_t *self, const char *union_name);
	tdr_error_code_t (*read_enum_begin)(tdr_abstract_reader_t *self, const char *enum_name);
	tdr_error_code_t (*read_enum_end)(tdr_abstract_reader_t *self, const char *enum_name);
	tdr_error_code_t (*read_vector_begin)(tdr_abstract_reader_t *self, const char* vec_name);
	tdr_error_code_t (*read_vector_end)(tdr_abstract_reader_t *self, const char* vec_name);
	tdr_error_code_t (*read_vector_element_begin)(tdr_abstract_reader_t *self, const char *var_name, uint32_t index);
	tdr_error_code_t (*read_vector_element_end)(tdr_abstract_reader_t *self, const char *var_name, uint32_t index);
	tdr_error_code_t (*read_field_begin)(tdr_abstract_reader_t *self, const char *var_name);
	tdr_error_code_t (*read_field_end)(tdr_abstract_reader_t *self, const char *var_name);

	tdr_error_code_t (*read_int8)(tdr_abstract_reader_t *self, int8_t *val);
	tdr_error_code_t (*read_int16)(tdr_abstract_reader_t *self, int16_t *val);
	tdr_error_code_t (*read_int32)(tdr_abstract_reader_t *self, int32_t *val);
	tdr_error_code_t (*read_int64)(tdr_abstract_reader_t *self, int64_t *val);
	tdr_error_code_t (*read_uint8)(tdr_abstract_reader_t *self, uint8_t *val);
	tdr_error_code_t (*read_uint16)(tdr_abstract_reader_t *self, uint16_t *val);
	tdr_error_code_t (*read_uint32)(tdr_abstract_reader_t *self, uint32_t *val);
	tdr_error_code_t (*read_uint64)(tdr_abstract_reader_t *self, uint64_t *val);
	tdr_error_code_t (*read_char)(tdr_abstract_reader_t *self, char *val);
	tdr_error_code_t (*read_bool)(tdr_abstract_reader_t *self, bool *val);
	tdr_error_code_t (*read_double)(tdr_abstract_reader_t *self, double *val);
	tdr_error_code_t (*read_string)(tdr_abstract_reader_t *self, char* str, uint32_t str_length);
};

void tdr_abstract_reader_init(tdr_abstract_reader_t *self);

tdr_error_code_t tdr_read_struct_begin(tdr_abstract_reader_t *self, const char *struct_name);
tdr_error_code_t tdr_read_struct_end(tdr_abstract_reader_t *self, const char *struct_name);
tdr_error_code_t tdr_read_union_begin(tdr_abstract_reader_t *self, const char *union_name);
tdr_error_code_t tdr_read_union_end(tdr_abstract_reader_t *self, const char *union_name);
tdr_error_code_t tdr_read_enum_begin(tdr_abstract_reader_t *self, const char *enum_name);
tdr_error_code_t tdr_read_enum_end(tdr_abstract_reader_t *self, const char *enum_name);
tdr_error_code_t tdr_read_vector_begin(tdr_abstract_reader_t *self, const char* vec_name);
tdr_error_code_t tdr_read_vector_end(tdr_abstract_reader_t *self, const char* vec_name);
tdr_error_code_t tdr_read_field_begin(tdr_abstract_reader_t *self, const char *var_name);
tdr_error_code_t tdr_read_field_end(tdr_abstract_reader_t *self, const char *var_name);
tdr_error_code_t tdr_read_vector_element_begin(tdr_abstract_reader_t *self, const char *var_name, uint32_t index);
tdr_error_code_t tdr_read_vector_element_end(tdr_abstract_reader_t *self, const char *var_name, uint32_t index);

tdr_error_code_t tdr_read_int8(tdr_abstract_reader_t *self, int8_t *val);
tdr_error_code_t tdr_read_int16(tdr_abstract_reader_t *self, int16_t *val);
tdr_error_code_t tdr_read_int32(tdr_abstract_reader_t *self, int32_t *val);
tdr_error_code_t tdr_read_int64(tdr_abstract_reader_t *self, int64_t *val);
tdr_error_code_t tdr_read_uint8(tdr_abstract_reader_t *self, uint8_t *val);
tdr_error_code_t tdr_read_uint16(tdr_abstract_reader_t *self, uint16_t *val);
tdr_error_code_t tdr_read_uint32(tdr_abstract_reader_t *self, uint32_t *val);
tdr_error_code_t tdr_read_uint64(tdr_abstract_reader_t *self, uint64_t *val);
tdr_error_code_t tdr_read_char(tdr_abstract_reader_t *self, char *val);
tdr_error_code_t tdr_read_bool(tdr_abstract_reader_t *self, bool *val);
tdr_error_code_t tdr_read_double(tdr_abstract_reader_t *self, double *val);
tdr_error_code_t tdr_read_string(tdr_abstract_reader_t *self, char* str, uint32_t str_length);

#ifdef  __cplusplus
}
#endif

#endif
