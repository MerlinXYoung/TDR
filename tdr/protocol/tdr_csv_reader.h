#ifndef _H_TDR_CSV_READER_H
#define _H_TDR_CSV_READER_H

#include "../tdr_abstract_reader.h"
#include "../tdr_error_code.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define TDR_CSV_FIELD_NUM 65535

typedef struct tdr_csv_reader_s
{
	tdr_abstract_reader_t super;
	char *top_line;
	size_t top_line_size;
	char* top_line_fields[TDR_CSV_FIELD_NUM];
	size_t	top_line_fields_num;

	char* cur_line_fields[TDR_CSV_FIELD_NUM];
	size_t	cur_line_fields_num;
	char *cur_line;
	size_t cur_line_size;

	bool pre_read_uint32;
	int32_t col;
	int32_t field_index[TDR_CSV_FIELD_NUM];
	char* field;
	char* field_end;
	bool read_enum_name_once;
	uint32_t pre_u32;
}tdr_csv_reader_t;



#define tdr_csv_reader_get_cur_col(self) ((self)->col)
#define tdr_csv_reader_get_cur_name(self) ((self)->super.name + 1)


tdr_error_code_t tdr_csv_reader_init(tdr_csv_reader_t *self, const char *top_line, size_t top_line_size);

void tdr_csv_reader_fini(tdr_csv_reader_t *self);

tdr_error_code_t tdr_csv_reader_store(tdr_csv_reader_t *self, const char *line, size_t line_size);

void tdr_csv_reader_close(tdr_csv_reader_t *self);


tdr_error_code_t tdr_csv_read_vector_begin(tdr_abstract_reader_t *super, const char* vec_name);

tdr_error_code_t tdr_csv_read_vector_end(tdr_abstract_reader_t *super, const char* vec_name);

tdr_error_code_t tdr_csv_read_vector_element_begin(tdr_abstract_reader_t *super, const char* var_name, uint32_t index);

tdr_error_code_t tdr_csv_read_vector_element_end(tdr_abstract_reader_t *super, const char* var_name, uint32_t index);

tdr_error_code_t tdr_csv_read_field_begin(tdr_abstract_reader_t *super, const char *var_name);

tdr_error_code_t tdr_csv_read_field_end(tdr_abstract_reader_t *super, const char *var_name);

tdr_error_code_t tdr_csv_read_enum_begin(tdr_abstract_reader_t *super, const char *enum_name);

tdr_error_code_t tdr_csv_read_int8(tdr_abstract_reader_t *super, int8_t *val);

tdr_error_code_t tdr_csv_read_int16(tdr_abstract_reader_t *super, int16_t *val);

tdr_error_code_t tdr_csv_read_int32(tdr_abstract_reader_t *super, int32_t *val);

tdr_error_code_t tdr_csv_read_int64(tdr_abstract_reader_t *super, int64_t *val);

tdr_error_code_t tdr_csv_read_uint8(tdr_abstract_reader_t *super, uint8_t *val);

tdr_error_code_t tdr_csv_read_uint16(tdr_abstract_reader_t *super, uint16_t *val);

tdr_error_code_t tdr_csv_read_uint32(tdr_abstract_reader_t *super, uint32_t *val);

tdr_error_code_t tdr_csv_read_uint64(tdr_abstract_reader_t *super, uint64_t *val);

tdr_error_code_t tdr_csv_read_bool(tdr_abstract_reader_t *super, bool *val);

tdr_error_code_t tdr_csv_read_double(tdr_abstract_reader_t *super, double *val);

tdr_error_code_t tdr_csv_read_char(tdr_abstract_reader_t *super, char *val);

tdr_error_code_t tdr_csv_read_string(tdr_abstract_reader_t *super, char *str, uint32_t str_len);

#endif //_H_TDR_CSV_READER_H

