#include "parse/check.h"
#include "symbols.h"
#include <string.h>

#include <assert.h>
#include <stdint.h>
void check_identifier_defined(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier)
{
	if(symbols_search(symbols, prefix, identifier) == NULL)
	{
		scanner_error_halt(yylloc, E_LS_IDENTIFIER_NOT_DEFINED, prefix, identifier);
	}
}

void check_identifier_not_defined(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier)
{
	const symbol_t *symbol = symbols_search(symbols, prefix, identifier);
	if(symbol != NULL)
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_REDEFINITION, prefix, identifier);
		scanner_error_halt(&symbol->yylloc, E_LS_IDENTIFIER_REDEFINITION, prefix, identifier);
	}
}

void check_identifier_is_type(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier)
{
	const symbol_t *symbol = symbols_search(symbols, prefix, identifier);
	assert(symbols != NULL);

	if((symbol->type != EN_HST_TYPEDEF) && (symbol->type != EN_HST_ENUM) 
		&& (symbol->type != EN_HST_STRUCT) && (symbol->type != EN_HST_UNION))
	{
		scanner_error_halt(yylloc, E_LS_IDENTIFIER_NOT_STRING, prefix, identifier, "enum struct union");
	}
}

void check_identifier_is_value(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier)
{
	const symbol_t * symbol = symbols_search(symbols, prefix, identifier);

	if(symbol->type != EN_HST_VALUE)
	{
		scanner_error_halt(yylloc, E_LS_IDENTIFIER_NOT_STRING, prefix, identifier, "char double string integer");
	}
}

void check_identifier_not_defined_as_value(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier)
{
	const symbol_t * symbol = symbols_search(symbols, prefix, identifier);

	if((symbol) && (symbol->type == EN_HST_VALUE))
	{
		scanner_error(yylloc, E_LS_IDENTIFIER_REDEFINITION, "", identifier);
		scanner_error_halt(&symbol->yylloc, E_LS_IDENTIFIER_REDEFINITION, "", identifier);
	}
}

void check_identifier_is_non_zero_positive_integer(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier)
{
	const syn_value_t *val = NULL;
	const symbol_t *symbol = symbols_search(symbols, prefix, identifier);
	assert(symbols != NULL);

	assert(symbol->type == EN_HST_VALUE);

	val = symbols_get_real_value(symbols, &symbol->body.val);
	assert(val != NULL);

	switch(val->type)
	{
	case E_SNVT_INT64:
	case E_SNVT_HEX_INT64:
		if(val->val.i64 <= 0)
		{
			scanner_error_halt(yylloc, E_LS_IDENTIFIER_NOT_INTEGER, prefix, identifier);
		}
	case E_SNVT_UINT64:
	case E_SNVT_HEX_UINT64:
		if(val->val.ui64 <= 0)
		{
			scanner_error_halt(yylloc, E_LS_IDENTIFIER_NOT_INTEGER, prefix, identifier);
		}
		break;
	default:
		scanner_error_halt(yylloc, E_LS_IDENTIFIER_NOT_STRING, prefix, identifier, "integer");
	}
}

void check_string_length_defined(const YYLTYPE *yylloc, const syn_simple_type_t *symbol_type)
{
	if(symbol_type->st != E_ST_STRING)
	{
		goto done;
	}

	if(symbol_type->string_length[0] == 0)
	{
		scanner_error_halt(yylloc, E_LS_STRING_LENGTH_NOT_DEFINED);
	}
done:
	return;
}

//���string�����Ƿ���
void check_string_length(const YYLTYPE *yylloc, const syn_simple_type_t *symbol_type, int defined)
{
	if(symbol_type->st != E_ST_STRING)
	{
		goto done;
	}

	if(defined)
	{
		if(symbol_type->string_length[0] == 0)
		{
			scanner_error_halt(yylloc, E_LS_CAN_NOT_DEFINE_STRING_LENGTH_HERE);
		}
	}
	else
	{
		if(symbol_type->string_length[0] != 0)
		{
			scanner_error_halt(yylloc, E_LS_CAN_NOT_DEFINE_STRING_LENGTH_HERE);
		}
	}
	
done:
	return;
}

//��������Ƿ�Ϊ����
void check_integer_type(const symbols_t *symbols, const YYLTYPE *yylloc, const syn_simple_type_t *simple_type)
{
	const syn_simple_type_t *real_type = symbols_get_real_type(symbols, simple_type);
	assert(real_type != NULL);
	
	switch(real_type->st)
	{
	case E_ST_INT8:
	case E_ST_INT16:
	case E_ST_INT32:
	case E_ST_INT64:
	case E_ST_UINT8:
	case E_ST_UINT16:
	case E_ST_UINT32:
	case E_ST_UINT64:
		break;
	case E_ST_REFER:
		{
			const symbol_t* enum_type = symbols_search(symbols, "", real_type->st_refer);
			assert(enum_type != NULL);
			if(enum_type->type != EN_HST_ENUM)
			{
				scanner_error_halt(yylloc, E_LS_NOT_INTEGER_TYPE);
			}
			break;
		}
	default:
		scanner_error_halt(yylloc, E_LS_NOT_INTEGER_TYPE);
	}
}

void check_integer_value(const symbols_t *symbols, const YYLTYPE *yylloc, const syn_value_t *val)
{
	const syn_value_t *real_val;
	real_val = symbols_get_real_value(symbols, val);
	assert(real_val != NULL);

	switch(real_val->type)
	{
	case E_SNVT_INT64:
	case E_SNVT_UINT64:
	case E_SNVT_HEX_INT64:
	case E_SNVT_HEX_UINT64:
		break;
	default:
		scanner_error_halt(yylloc, E_LS_NOT_INTEGER_VALUE);
	}
}

void check_identifier_refer_to_a_field_with_integer_type(const symbols_t *symbols, const YYLTYPE *yylloc, const char *prefix, const char *identifier)
{
	const symbol_t* symbol = symbols_search(symbols, prefix, identifier);
	assert(symbol != NULL);
	assert(symbol->type == EN_HST_FIELD);
	if(symbol->body.field.type.type != E_SNT_SIMPLE)
	{
		scanner_error_halt(yylloc, E_LS_NOT_INTEGER_TYPE);
	}

	check_integer_type(symbols, yylloc, &symbol->body.field.type.st);
}

void check_arguments(const symbols_t *symbols, const YYLTYPE *yylloc, const syn_type_t *type, const syn_arguments_t *arguments)
{
	const syn_simple_type_t *field_type = NULL;

	const syn_simple_type_t *par_type = NULL;
	const syn_simple_type_t *arg_type = NULL;

	//��ȡ��������
	if(type->type == E_SNT_CONTAINER)
	{
		assert(type->ct.ct == E_CT_VECTOR);
		field_type = &type->ct.vector_type;
	}
	else
	{
		assert(type->type == E_SNT_SIMPLE);
		field_type = &type->st;
	}
	field_type = symbols_get_real_type(symbols, field_type);


	//��ȡʵ�ʲ�������
	if(arguments->arg_list_num == 1)
	{
		const symbol_t* arg_field_symbol = NULL;
		arg_field_symbol = symbols_search(symbols, symbols->struct_name, arguments->arg_list[0]);
		assert(arg_field_symbol != NULL);
		assert(arg_field_symbol->type == EN_HST_FIELD);
		if(arg_field_symbol->body.field.type.type != E_SNT_SIMPLE)
		{
			scanner_error_halt(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
		}
		arg_type = symbols_get_real_type(symbols, &arg_field_symbol->body.field.type.st);
		assert(arg_type != NULL);
	}
	
	//��ȡ��ʽ������
	if(field_type->st == E_ST_REFER)
	{
		const symbol_t* union_symbol = symbols_search(symbols, "", field_type->st_refer);
		assert(union_symbol != NULL);
		if(union_symbol->type == EN_HST_UNION)
		{
			assert(union_symbol->body.union_symbol.para.par_list_num == 1);
			par_type = symbols_get_real_type(symbols, &union_symbol->body.union_symbol.para.par_list[0].type);
			assert(par_type != NULL);
		}
	}

	if((par_type == NULL) && (arg_type == NULL))
	{
		//okey~
	}
	else if((par_type != NULL) && (arg_type != NULL))
	{
		if((par_type->st != E_ST_REFER) || (arg_type->st != E_ST_REFER))
		{
			scanner_error_halt(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
		}

		if(strcmp(par_type->st_refer, arg_type->st_refer) != 0)
		{
			scanner_error_halt(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
		}
	}
	else
	{
		scanner_error_halt(yylloc, E_LS_ARGUMENT_TYPE_MISMATCH);
	}
}

//����ַ��������Ƿ񳬹�����
void check_strlen_too_long(const YYLTYPE *yylloc, const char *str, const char *suffix, uint32_t limit)
{
	if(strlen(str) + strlen(suffix) + 1 > limit)
	{
		scanner_error_halt(yylloc, E_LS_IDENTIFIER_LENGTH_ERROR, limit);
	}
}

#define CHECK_VALUE_TYPE(vt, v_min, v_max)\
switch(vt)\
{\
case E_SNVT_INT64:\
	if((i64 < v_min) || (i64 > v_max))\
	{\
		scanner_error_halt(yylloc, E_LS_NUMBER_ERROR_RANGE);\
	}\
	break;\
case E_SNVT_UINT64:\
	if((ui64 < v_min) || (ui64 > v_max))\
	{\
		scanner_error_halt(yylloc, E_LS_NUMBER_ERROR_RANGE);\
	}\
	break;\
default:\
	scanner_error_halt(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);\
}

void check_value_type(const symbols_t *symbols, const YYLTYPE *yylloc, const syn_simple_type_t *type, const syn_value_t *val)
{
	const syn_simple_type_t *real_type_ptr = NULL;
	const syn_value_t *real_val_ptr = NULL;
	syn_simple_type_type_t st;

	syn_value_type_t vt;
	int64_t i64 = 0;
	uint64_t ui64 = 0;

	real_type_ptr = symbols_get_real_type(symbols, type);
	st = real_type_ptr->st;


	real_val_ptr = symbols_get_real_value(symbols, val);
	vt = real_val_ptr->type;
	switch(vt)
	{
	case E_SNVT_INT64:
		i64 = real_val_ptr->val.i64;
		break;
	case E_SNVT_HEX_INT64:
		i64 = real_val_ptr->val.i64;
		vt = E_SNVT_INT64;
		break;
	case E_SNVT_UINT64:
		ui64 = real_val_ptr->val.ui64;
		break;
	case E_SNVT_HEX_UINT64:
		ui64 = real_val_ptr->val.ui64;
		vt = E_SNVT_UINT64;
		break;
	default:
		break;
	}

	switch(st)
	{
	case E_ST_INT8:
		CHECK_VALUE_TYPE(vt, TDR_INT8_MIN, TDR_INT8_MAX);
		break;
	case E_ST_INT16:
		CHECK_VALUE_TYPE(vt, TDR_INT16_MIN, TDR_INT16_MAX);
		break;
	case E_ST_INT32:
		CHECK_VALUE_TYPE(vt, TDR_INT32_MIN, TDR_INT32_MAX);
		break;
	case E_ST_INT64:
		CHECK_VALUE_TYPE(vt, TDR_INT64_MIN, TDR_INT64_MAX);
		break;
	case E_ST_UINT8:
		CHECK_VALUE_TYPE(vt, 0, TDR_UINT8_MAX);
		break;
	case E_ST_UINT16:
		CHECK_VALUE_TYPE(vt, 0, TDR_UINT16_MAX);
		break;
	case E_ST_UINT32:
		CHECK_VALUE_TYPE(vt, 0, TDR_UINT32_MAX);
		break;
	case E_ST_UINT64:
		CHECK_VALUE_TYPE(vt, 0, TDR_UINT64_MAX);
		break;
	case E_ST_CHAR:
		if(vt != E_SNVT_CHAR)
		{
			scanner_error_halt(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;
	case E_ST_DOUBLE:
		if(vt != E_SNVT_DOUBLE)
		{
			scanner_error_halt(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;
	case E_ST_STRING:
		if(vt != E_SNVT_STRING)
		{
			scanner_error_halt(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;
	case E_ST_BOOL:
		if(vt != E_SNVT_BOOL)
		{
			scanner_error_halt(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		}
		break;		
	default:
		scanner_error_halt(yylloc, E_LS_CONSTANT_TYPES_DO_NOT_MATCH);
		break;
	}
}

void check_simpletype_is_enum(const symbols_t *symbols, const YYLTYPE *yylloc, const syn_simple_type_t *simple_type)
{
	const symbol_t* enum_symbol;
	simple_type = symbols_get_real_type(symbols, simple_type);
	assert(simple_type != NULL);
	if(simple_type->st != E_ST_REFER)
	{
		scanner_error_halt(yylloc, E_LS_NOT_ENUM_TYPE);
	}

	enum_symbol = symbols_search(symbols, "", simple_type->st_refer);

	if(enum_symbol->type != EN_HST_ENUM)
	{
		scanner_error_halt(yylloc, E_LS_NOT_ENUM_TYPE);
	}
}

void check_enumdef_is_unique(const YYLTYPE *yylloc, const syn_enum_t *pn_enum, uint32_t index)
{
	uint32_t i;

	for(i = 0;i < pn_enum->enum_def_list_num; ++i)
	{
		if(pn_enum->enum_def_list[i].val.val.i64 == pn_enum->enum_def_list[index].val.val.i64)
		{
			scanner_error_halt(yylloc, E_LS_ENUM_HAVE_EQUAL_ELEMENTS, pn_enum->enum_def_list[i].identifier, pn_enum->enum_def_list[index].identifier);
		}
	}
}

void check_str_equal(const YYLTYPE *yylloc, const char *src, const char* dst)
{
	if(strcmp(src, dst) != 0)
	{
		scanner_error_halt(yylloc, E_LS_IDENTIFIER_NOT_STRING, src, dst);
	}
}
