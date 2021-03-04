#ifndef _H_TDR_BIND_READER
#define _H_TDR_BIND_READER

#ifdef  __cplusplus
extern "C" {
#endif


#include "../tdr_platform.h"
#include "../tdr_abstract_reader.h"
#include "../tdr_error_code.h"

#include "mysql.h"
typedef struct tdr_mybind_reader_s
{
	tdr_abstract_reader_t		super;
	MYSQL_BIND					*bind_vec;
	uint32_t					bind_vec_num;
	uint32_t					idx;
}tdr_mybind_reader_t;

void tdr_mybind_reader_init(tdr_mybind_reader_t *self, MYSQL_BIND *bind_vec, uint32_t bind_vec_num);

tdr_error_code_t tdr_mybind_read_int8(tdr_abstract_reader_t *super, int8_t *val);

tdr_error_code_t tdr_mybind_read_int16(tdr_abstract_reader_t *super, int16_t *val);

tdr_error_code_t tdr_mybind_read_int32(tdr_abstract_reader_t *super, int32_t *val);

tdr_error_code_t tdr_mybind_read_int64(tdr_abstract_reader_t *super, int64_t *val);

tdr_error_code_t tdr_mybind_read_uint8(tdr_abstract_reader_t *super, uint8_t *val);

tdr_error_code_t tdr_mybind_read_uint16(tdr_abstract_reader_t *super, uint16_t *val);

tdr_error_code_t tdr_mybind_read_uint32(tdr_abstract_reader_t *super, uint32_t *val);

tdr_error_code_t tdr_mybind_read_uint64(tdr_abstract_reader_t *super, uint64_t *val);

tdr_error_code_t tdr_mybind_read_double(tdr_abstract_reader_t *super, double *val);

tdr_error_code_t tdr_mybind_read_char(tdr_abstract_reader_t *super, char *val);

tdr_error_code_t tdr_mybind_read_bool(tdr_abstract_reader_t *super, bool *val);

tdr_error_code_t tdr_mybind_read_string(tdr_abstract_reader_t *super, char *str, uint32_t str_len);

#ifdef  __cplusplus
}
#endif

#endif

