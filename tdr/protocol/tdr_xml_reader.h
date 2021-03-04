#ifndef _H_TDR_XML_READER
#define _H_TDR_XML_READER

#ifdef  __cplusplus
extern "C" {
#endif


#include "../tdr_platform.h"
#include "../tdr_abstract_reader.h"
#include "../tdr_error_code.h"

typedef struct tdr_xml_reader_yyltype_s
{
	char file_name[TDR_MAX_PATH_LENGTH];
	int first_line;
	int first_column;
	int last_line;
	int last_column;
} tdr_xml_reader_yyltype_t;

typedef struct tdr_xml_reader_scanner_context_s tdr_xml_reader_scanner_context_t;
struct tdr_xml_reader_scanner_context_s
{
	int yy_state;
	const char *yy_last;
	const char *yy_cursor;
	const char *yy_limit;	
	const char *yy_marker;
	const char *yy_start;

	const char *yy_text;
	uint32_t yy_leng;

	uint32_t yylineno;
	uint32_t yycolumn;

	tdr_xml_reader_yyltype_t yylloc;

	char tag_name[TDR_MAX_LENGTH_OF_IDENTIFIER];
	const char *content_begin;

	void *filecontent_ptr;
};

#define TDR_XML_MAX_INCLUDE 1024
#define TDR_XML_MAX_DEEP 1024
typedef struct tdr_xml_reader_s
{
	tdr_abstract_reader_t super;

	tdr_xml_reader_scanner_context_t scanner_context_stack[TDR_XML_MAX_DEEP];
	size_t scanner_context_stack_num;

	const char *include[TDR_XML_MAX_INCLUDE];
	size_t include_num;

	int struct_deep;
	int ignore_int32_once;
	int pre_read_uint32_field_once;
	uint32_t ui32;
	tdr_error_code_t error_code;
}tdr_xml_reader_t;

void tdr_xml_reader_init(tdr_xml_reader_t *self);

tdr_error_code_t tdr_xml_add_include(tdr_xml_reader_t *self, const char *path);

tdr_error_code_t tdr_xml_reader_push_file(tdr_xml_reader_t *self, const char *file_name);

tdr_error_code_t tdr_xml_reader_push_buff(tdr_xml_reader_t *self, const char *xml_start, const char* xml_limit);

void tdr_xml_reader_pop_file(tdr_xml_reader_t *self);

void tdr_xml_reader_pop_buff(tdr_xml_reader_t *self);

const char* tdr_xml_str2c(const char* curr, const char* limit, char *ch);

const tdr_xml_reader_yyltype_t* tdr_xml_current_location(tdr_xml_reader_t *self);



tdr_error_code_t tdr_xml_read_struct_begin(tdr_abstract_reader_t *self, const char *struct_name);

tdr_error_code_t tdr_xml_read_struct_end(tdr_abstract_reader_t *self, const char *struct_name);

tdr_error_code_t tdr_xml_read_enum_begin(tdr_abstract_reader_t *self, const char *enum_name);

tdr_error_code_t tdr_xml_read_vector_begin(tdr_abstract_reader_t *self, const char *vec_name);

tdr_error_code_t tdr_xml_read_vector_end(tdr_abstract_reader_t *self, const char *vec_name);

tdr_error_code_t tdr_xml_read_field_begin(tdr_abstract_reader_t *self, const char *var_name);

tdr_error_code_t tdr_xml_read_field_end(tdr_abstract_reader_t *self, const char *var_name);

tdr_error_code_t tdr_xml_read_vector_element_begin(tdr_abstract_reader_t *self, const char *var_name, uint32_t index);

tdr_error_code_t tdr_xml_read_vector_element_end(tdr_abstract_reader_t *self, const char *var_name, uint32_t index);

tdr_error_code_t tdr_xml_read_char(tdr_abstract_reader_t *super, char *val);

tdr_error_code_t tdr_xml_read_bool(tdr_abstract_reader_t *super, bool *val);

tdr_error_code_t tdr_xml_read_double(tdr_abstract_reader_t *super, double *val);

tdr_error_code_t tdr_xml_read_int8(tdr_abstract_reader_t *super, int8_t *val);

tdr_error_code_t tdr_xml_read_int16(tdr_abstract_reader_t *super, int16_t *val);

tdr_error_code_t tdr_xml_read_int32(tdr_abstract_reader_t *super, int32_t *val);

tdr_error_code_t tdr_xml_read_int64(tdr_abstract_reader_t *super, int64_t *val);

tdr_error_code_t tdr_xml_read_uint8(tdr_abstract_reader_t *super, uint8_t *val);

tdr_error_code_t tdr_xml_read_uint16(tdr_abstract_reader_t *super, uint16_t *val);

tdr_error_code_t tdr_xml_read_uint32(tdr_abstract_reader_t *super, uint32_t *val);

tdr_error_code_t tdr_xml_read_uint64(tdr_abstract_reader_t *super, uint64_t *val);

tdr_error_code_t tdr_xml_read_string(tdr_abstract_reader_t *super, char *str, uint32_t str_len);

#ifdef  __cplusplus
}
#endif


#endif

