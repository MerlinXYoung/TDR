#ifndef _H_TDR_XML_WRITER
#define _H_TDR_XML_WRITER

#ifdef  __cplusplus
extern "C" {
#endif


#include "../tdr_platform.h"
#include "../tdr_abstract_writer.h"
#include "../tdr_error_code.h"

#include <stdio.h>


typedef struct tdr_xml_writer_s
{
	tdr_abstract_writer_t super;
	uint32_t count;
	char *start;
	char *cur;
	char *limit;

	int skip_uint32_field_once;

	int ignore_int32_once;
}tdr_xml_writer_t;

void tdr_xml_writer_init(tdr_xml_writer_t *self, char *start, char*limit);


tdr_error_code_t tdr_xml_write_vector_begin(tdr_abstract_writer_t *super, const char* vec_name);

tdr_error_code_t tdr_xml_write_vector_end(tdr_abstract_writer_t *super, const char* vec_name);

tdr_error_code_t tdr_xml_write_field_begin(tdr_abstract_writer_t *super, const char *var_name);

tdr_error_code_t tdr_xml_write_field_end(tdr_abstract_writer_t *super, const char *var_name);

tdr_error_code_t tdr_xml_write_vector_element_begin(tdr_abstract_writer_t *super, const char *var_name, uint32_t index);

tdr_error_code_t tdr_xml_write_vector_element_end(tdr_abstract_writer_t *super, const char *var_name, uint32_t index);

tdr_error_code_t tdr_xml_write_int8(tdr_abstract_writer_t *super, const int8_t *val);

tdr_error_code_t tdr_xml_write_int16(tdr_abstract_writer_t *super, const int16_t *val);

tdr_error_code_t tdr_xml_write_int32(tdr_abstract_writer_t *super, const int32_t *val);

tdr_error_code_t tdr_xml_write_int64(tdr_abstract_writer_t *super, const int64_t *val);

tdr_error_code_t tdr_xml_write_uint8(tdr_abstract_writer_t *super, const uint8_t *val);

tdr_error_code_t tdr_xml_write_uint16(tdr_abstract_writer_t *super, const uint16_t *val);

tdr_error_code_t tdr_xml_write_uint32(tdr_abstract_writer_t *super, const uint32_t *val);

tdr_error_code_t tdr_xml_write_uint64(tdr_abstract_writer_t *super, const uint64_t *val);

tdr_error_code_t tdr_xml_write_string(tdr_abstract_writer_t *super, const char* str, uint32_t str_length);

tdr_error_code_t tdr_xml_write_double(tdr_abstract_writer_t *super, const double *val);

tdr_error_code_t tdr_xml_write_char(tdr_abstract_writer_t *super, const char *val);

tdr_error_code_t tdr_xml_write_bool(tdr_abstract_writer_t *super, const bool *val);

tdr_error_code_t tdr_xml_write_struct_begin(tdr_abstract_writer_t *super, const char *struct_name);

tdr_error_code_t tdr_xml_write_struct_end(tdr_abstract_writer_t *super, const char *struct_name);

tdr_error_code_t tdr_xml_write_enum_begin(tdr_abstract_writer_t *super, const char *enum_name);


#ifdef  __cplusplus
}
#endif

#endif
