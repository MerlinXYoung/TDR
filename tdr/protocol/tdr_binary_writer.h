#ifndef _H_TDR_WRITER_WRITER
#define _H_TDR_WRITER_WRITER

#ifdef  __cplusplus
extern "C" {
#endif


#include "../tdr_platform.h"
#include "../tdr_abstract_writer.h"

typedef struct tdr_binary_writer_s
{
	tdr_abstract_writer_t super;

	char *addr;
	uint32_t size;
	uint32_t offset;
}tdr_binary_writer_t;

void tdr_binary_writer_init(tdr_binary_writer_t *self, char *addr, uint32_t size);

tdr_error_code_t tdr_binary_write_int8(tdr_abstract_writer_t *super, const int8_t *val);

tdr_error_code_t tdr_binary_write_int16(tdr_abstract_writer_t *super, const int16_t *val);

tdr_error_code_t tdr_binary_write_int32(tdr_abstract_writer_t *super, const int32_t *val);

tdr_error_code_t tdr_binary_write_int64(tdr_abstract_writer_t *super, const int64_t *val);

tdr_error_code_t tdr_binary_write_uint8(tdr_abstract_writer_t *super, const uint8_t *val);

tdr_error_code_t tdr_binary_write_uint16(tdr_abstract_writer_t *super, const uint16_t *val);

tdr_error_code_t tdr_binary_write_uint32(tdr_abstract_writer_t *super, const uint32_t *val);

tdr_error_code_t tdr_binary_write_uint64(tdr_abstract_writer_t *super, const uint64_t *val);

tdr_error_code_t tdr_binary_write_char(tdr_abstract_writer_t *super, const char *val);

tdr_error_code_t tdr_binary_write_bool(tdr_abstract_writer_t *super, const bool *val);

tdr_error_code_t tdr_binary_write_double(tdr_abstract_writer_t *super, const double *val);

tdr_error_code_t tdr_binary_write_string(tdr_abstract_writer_t *super, const char* str, uint32_t str_length);

#ifdef  __cplusplus
}
#endif

#endif
