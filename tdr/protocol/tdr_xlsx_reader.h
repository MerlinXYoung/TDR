#ifndef _H_TDR_XLSX_READER
#define _H_TDR_XLSX_READER

#ifdef  __cplusplus
extern "C" {
#endif


#include "../tdr_platform.h"
#include "../tdr_abstract_reader.h"
#include "../tdr_error_code.h"
#include "../tdr_unzip.h"
#include "../tdr_hash.h"

typedef struct tdr_xlsx_reader_scanner_s
{
	int state;
	char *cursor;
	char *limit;
	char *marker;
}tdr_xlsx_reader_scanner_t;

typedef struct _tdr_xlsx_pos
{
	uint32_t row;
	uint32_t col;
}tdr_xlsx_pos;

typedef struct _tdr_xlsx_cell_s
{
	tdr_hash_head_t name2index;
	int empty;
	const char* xpos;

	const char* val_start;
	const char* val_limit;

	const char* val_begin;
	const char* val_end;
}tdr_xlsx_cell_s;

#define TDR_XLSX_HASH_BUCKET 65536
#define TDR_XLSX_MAX_COL_STR 1024

typedef struct _tdr_xlsx_reader_t
{
	tdr_abstract_reader_t super;

	tdr_unzip_s unzip;

	char* workbook_rels_buff;
	uint32_t workbook_rels_buff_size;

	char* workbook_buff;
	uint32_t workbook_buff_size;

	char* sharedstring_buff;
	uint32_t sharedstring_buff_size;

	char* sheet_buff;
	uint32_t sheet_buff_size;

	tdr_xlsx_reader_scanner_t scanner;

	char** sharedstring_begin_list;
	char** sharedstring_end_list;
	uint32_t sharedstring_list_num;

	tdr_xlsx_pos cell_min_pos;
	tdr_xlsx_pos cell_max_pos;
	uint32_t cell_row_size;
	uint32_t cell_col_size;
	tdr_xlsx_cell_s *cell_matrix;
	tdr_xlsx_cell_s *bindinfo_row;

	tdr_xlsx_cell_s *curr_row;
	tdr_xlsx_cell_s *curr_cell;	

	


	int32_t last_col;
	char last_col_str[TDR_XLSX_MAX_COL_STR];

	int read_enum_name_once;
	tdr_hash_bucket_t hash_bucket[TDR_XLSX_HASH_BUCKET];
	tdr_hash_t name2index;

	int pre_read_uint32;
	uint32_t pre_u32;


	//如果不存在动态长度的数据， 可以通过这个开关来优化读取速度。
	//可以通过TData生成数据的标号来更好的解决这个问题。
	int use_cache;
	size_t row_index;
	tdr_xlsx_cell_s **hash_cache;
}tdr_xlsx_reader_t;

tdr_error_code_t tdr_xlsx_reader_init(tdr_xlsx_reader_t *self, const char *file_name);

tdr_error_code_t tdr_xlsx_reader_open_sheet(tdr_xlsx_reader_t *self, const char* sheet, uint32_t bindinfo_row);

uint32_t tdr_xlsx_reader_num_rows(tdr_xlsx_reader_t *self);

void tdr_xlsx_reader_row_seek(tdr_xlsx_reader_t *self, uint32_t offset);

void tdr_xlsx_reader_close_sheet(tdr_xlsx_reader_t *self);

void tdr_xlsx_reader_fini(tdr_xlsx_reader_t *self);

size_t tdr_xlsx_str2num(const char* str);

const char* tdr_xlsx_num2str(int num, char *str, size_t str_max_len);

size_t tdr_xlsx_current_col(tdr_xlsx_reader_t *self);


tdr_error_code_t tdr_xlsx_read_vector_begin(tdr_abstract_reader_t *super, const char* vec_name);

tdr_error_code_t tdr_xlsx_read_vector_element_begin(tdr_abstract_reader_t *super, const char* var_name, uint32_t index);

tdr_error_code_t tdr_xlsx_read_field_begin(tdr_abstract_reader_t *super, const char *var_name);

tdr_error_code_t tdr_xlsx_read_enum_begin(tdr_abstract_reader_t *super, const char *enum_name);

tdr_error_code_t tdr_xlsx_read_char(tdr_abstract_reader_t *super, char *val);

tdr_error_code_t tdr_xlsx_read_double(tdr_abstract_reader_t *super, double *val);

tdr_error_code_t tdr_xlsx_read_int8(tdr_abstract_reader_t *super, int8_t *val);

tdr_error_code_t tdr_xlsx_read_int16(tdr_abstract_reader_t *super, int16_t *val);

tdr_error_code_t tdr_xlsx_read_int32(tdr_abstract_reader_t *super, int32_t *val);

tdr_error_code_t tdr_xlsx_read_int64(tdr_abstract_reader_t *super, int64_t *val);

tdr_error_code_t tdr_xlsx_read_uint8(tdr_abstract_reader_t *super, uint8_t *val);

tdr_error_code_t tdr_xlsx_read_uint16(tdr_abstract_reader_t *super, uint16_t *val);

tdr_error_code_t tdr_xlsx_read_uint32(tdr_abstract_reader_t *super, uint32_t *val);

tdr_error_code_t tdr_xlsx_read_uint64(tdr_abstract_reader_t *super, uint64_t *val);

tdr_error_code_t tdr_xlsx_read_string(tdr_abstract_reader_t *super, char *str, uint32_t str_len);

#ifdef  __cplusplus
}
#endif

#endif
