#ifndef _H_TDR_BINARY_READER
#define _H_TDR_BINARY_READER

#ifdef  __cplusplus
extern "C" {
#endif


#include "../tdr_platform.h"
#include "../tdr_abstract_reader.h"

typedef struct tdr_binary_reader_s
{
	tdr_abstract_reader_t super;

	const char *addr;
	uint32_t size;
	uint32_t offset;
}tdr_binary_reader_t;

void tdr_binary_reader_init(tdr_binary_reader_t *self, const char *addr, uint32_t size);

tdr_error_code_t tdr_binary_read_int8(tdr_abstract_reader_t *super, int8_t *val);

tdr_error_code_t tdr_binary_read_int16(tdr_abstract_reader_t *super, int16_t *val);

tdr_error_code_t tdr_binary_read_int32(tdr_abstract_reader_t *super, int32_t *val);

tdr_error_code_t tdr_binary_read_int64(tdr_abstract_reader_t *super, int64_t *val);

tdr_error_code_t tdr_binary_read_uint8(tdr_abstract_reader_t *super, uint8_t *val);

tdr_error_code_t tdr_binary_read_uint16(tdr_abstract_reader_t *super, uint16_t *val);

tdr_error_code_t tdr_binary_read_uint32(tdr_abstract_reader_t *super, uint32_t *val);

tdr_error_code_t tdr_binary_read_uint64(tdr_abstract_reader_t *super, uint64_t *val);

tdr_error_code_t tdr_binary_read_char(tdr_abstract_reader_t *super, char *val);

tdr_error_code_t tdr_binary_read_bool(tdr_abstract_reader_t *super, bool *val);

tdr_error_code_t tdr_binary_read_double(tdr_abstract_reader_t *super, double *val);

tdr_error_code_t tdr_binary_read_string(tdr_abstract_reader_t *super, char* str, uint32_t str_length);

#ifdef  __cplusplus
}
#endif


#endif
