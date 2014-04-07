#include "generate/tlibc_writer.h"
#include "generate/tlibc_writer_header.h"
#include "generator.h"
#include "version.h"
#include "symbols.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>


static TD_ERROR_CODE on_document_begin(GENERATOR *super, const YYLTYPE *yylloc, const char *file_name)
{
	char header[MAX_PACKAGE_NAME_LENGTH];	
	TLIBC_UNUSED(yylloc);

	generator_open(super, file_name, TLIBC_WRITER_SUFFIX);

	generator_printline(super, 0, "/**");
    generator_printline(super, 0, " * Autogenerated by TData Compiler (%s)", TDATA_VERSION);
    generator_printline(super, 0, " *");
    generator_printline(super, 0, " * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING");
    generator_printline(super, 0, " *  @generated");
    generator_printline(super, 0, " */");
	generator_printline(super, 0, "");

	
	//包含header的头文件
	strncpy_notdir(header, file_name, MAX_PACKAGE_NAME_LENGTH - 1);
	generator_replace_extension(header, MAX_PACKAGE_NAME_LENGTH, TLIBC_WRITER_HEADER_SUFFIX);
	generator_printline(super, 0, "#include \"%s\"", header);

	generator_printline(super, 0, "#include <stdint.h>");

	generator_printline(super, 0, "");
	generator_printline(super, 0, "");
	return E_TD_NOERROR;
}

static TD_ERROR_CODE on_document_end(GENERATOR *super, const YYLTYPE *yylloc, const char *file_name)
{	
	TLIBC_UNUSED(file_name);
	TLIBC_UNUSED(yylloc);

	generator_printline(super, 0, "");

	generator_close(super);
	return E_TD_NOERROR;
}

static TD_ERROR_CODE _on_import(TLIBC_WRITER_GENERATOR *self, const ST_Import *de_import)
{
	char name[MAX_PACKAGE_NAME_LENGTH];	
	strncpy_notdir(name, de_import->package_name, MAX_PACKAGE_NAME_LENGTH - 1);
	generator_replace_extension(name, MAX_PACKAGE_NAME_LENGTH, TLIBC_WRITER_HEADER_SUFFIX);
	generator_printline(&self->super, 0, "#include \"%s\"", name);
	generator_printline(&self->super, 0, "");

	return E_TD_NOERROR;
}

static TD_ERROR_CODE _on_enum(TLIBC_WRITER_GENERATOR *self, const ST_ENUM *de_enum)
{
	uint32_t i;
	generator_printline(&self->super, 0, "");
	generator_printline(&self->super, 0, "TLIBC_ERROR_CODE tlibc_write_%s(TLIBC_ABSTRACT_WRITER *self, const enum %s *data)", de_enum->name, de_enum->name);
	generator_printline(&self->super, 0, "{");
	generator_printline(&self->super, 0, "\tTLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;");
	generator_printline(&self->super, 0, "\tif((ret = tlibc_write_enum_begin(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_enum->name);
	generator_printline(&self->super, 0, "");
	generator_print(&self->super, 0, "\tret = tlibc_write_");
	generator_print_type_name(&self->super, &g_enum_type);
	generator_print(&self->super, 0, "(self, (const ");
	generator_print_ctype(&self->super, &g_enum_type);
	generator_printline(&self->super, 0, "*)data);");
	
	
	generator_printline(&self->super, 0, "\tif(ret == E_TLIBC_PLEASE_READ_ENUM_NAME)");
	generator_printline(&self->super, 0, "\t{");

	generator_printline(&self->super, 0, "\t\tswitch(*data)");
	generator_printline(&self->super, 0, "\t\t{");
	for(i = 0; i < de_enum->enum_def_list_num; ++i)
	{
		generator_printline(&self->super, 0, "\t\tcase %s:", de_enum->enum_def_list[i].identifier);
		generator_printline(&self->super, 0, "\t\t\tif((ret = tlibc_write_string(self, \"%s\", %u)) == E_TLIBC_NOERROR) break;", de_enum->enum_def_list[i].identifier, strlen(de_enum->enum_def_list[i].identifier));
		generator_printline(&self->super, 0, "\t\t\tbreak;");
	}
	generator_printline(&self->super, 0, "\t\tdefault:");
	generator_printline(&self->super, 0, "\t\t\tret = E_TLIBC_NOT_FOUND;");
	generator_printline(&self->super, 0, "\t\t\tgoto done;");
	generator_printline(&self->super, 0, "\t\t}");
	generator_printline(&self->super, 0, "\t}");
	generator_printline(&self->super, 0, "\telse if(ret != E_TLIBC_NOERROR)");
	generator_printline(&self->super, 0, "\t{");
	generator_printline(&self->super, 0, "\t\tgoto done;");
	generator_printline(&self->super, 0, "\t}");


	generator_printline(&self->super, 0, "");
	generator_printline(&self->super, 0, "");
	generator_printline(&self->super, 0, "\tif((ret = tlibc_write_enum_end(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_enum->name);		
	generator_printline(&self->super, 0, "done:");
	generator_printline(&self->super, 0, "\treturn ret;");	
	generator_printline(&self->super, 0, "}");


	return E_TD_NOERROR;
}

static TD_ERROR_CODE _on_struct(TLIBC_WRITER_GENERATOR *self, const ST_STRUCT *de_struct)
{
	uint32_t i;
	generator_printline(&self->super, 0, "");

	generator_printline(&self->super, 0, "TLIBC_ERROR_CODE tlibc_write_%s(TLIBC_ABSTRACT_WRITER *self, const struct %s *data)", de_struct->name, de_struct->name);

	generator_printline(&self->super, 0, "{");
	generator_printline(&self->super, 0, "\tTLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;");
	generator_printline(&self->super, 0, "\tif((ret = tlibc_write_struct_begin(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_struct->name);
	for(i = 0; i < de_struct->field_list.field_list_num; ++i)
	{
		generator_printline(&self->super, 0, "");
		//condition
		if(de_struct->field_list.field_list[i].condition.oper != E_EO_NON)
		{
			const char *op = NULL;
			switch(de_struct->field_list.field_list[i].condition.oper)
			{
			case E_EO_AND:
				op = "&";		
				break;
			case E_EO_EQUAL:		
				op = "==";
				break;
			case E_EO_UNEQUAL:
				op = "!=";
				break;
			default:
				assert(0);
			}
			generator_print(&self->super, 0, "\tif(data->%s %s ", de_struct->field_list.field_list[i].condition.op0, op);
			generator_print_value(&self->super, &de_struct->field_list.field_list[i].condition.op1);
			generator_printline(&self->super, 0, ")");
		}
		else
		{
			generator_printline(&self->super, 0, "\t");
		}
		generator_printline(&self->super, 0, "\t{");

		
		if(de_struct->field_list.field_list[i].type.type == E_SNT_CONTAINER)
		{
			if(de_struct->field_list.field_list[i].type.ct.ct == E_CT_VECTOR)
			{
				const ST_SIMPLE_TYPE *vector_type = symbols_get_real_type(self->super.symbols, &de_struct->field_list.field_list[i].type.ct.vector_type);
				generator_printline(&self->super, 0, "\t\tuint32_t i;");				

				generator_printline(&self->super, 0, "\t\tif((ret = tlibc_write_vector_begin(self)) != E_TLIBC_NOERROR) goto done;");

				generator_printline(&self->super, 0, "");
				generator_printline(&self->super, 2, "{");
				generator_printline(&self->super, 3, "if((ret = tlibc_write_field_begin(self, \"%s_num\")) == E_TLIBC_NOERROR)", de_struct->field_list.field_list[i].identifier);
				generator_printline(&self->super, 3, "{");
				generator_print(&self->super, 4, "if((ret = tlibc_write_");
				generator_print_type_name(&self->super, &g_vec_num_type);
				generator_printline(&self->super, 0, "(self, &data->"VEC_NUM_TYPE_STYLE")) != E_TLIBC_NOERROR) goto done;", de_struct->field_list.field_list[i].identifier);
				generator_printline(&self->super, 3, "}");
				generator_printline(&self->super, 3, "else if (ret != E_TLIBC_IGNORE)");
				generator_printline(&self->super, 4, "goto done;");
				generator_printline(&self->super, 3, "if((ret = tlibc_write_field_end(self, \"%s_num\")) != E_TLIBC_NOERROR) goto done;", de_struct->field_list.field_list[i].identifier);
				generator_printline(&self->super, 2, "}");
				generator_printline(&self->super, 0, "");

				generator_printline(&self->super, 2, "for(i = 0; i < %s; ++i)", de_struct->field_list.field_list[i].type.ct.vector_length);
				generator_printline(&self->super, 2, "{");				
				generator_printline(&self->super, 3, "if(i == data->"VEC_NUM_TYPE_STYLE") break;", de_struct->field_list.field_list[i].identifier);
				generator_printline(&self->super, 0, "");
				generator_printline(&self->super, 3, "if((ret = tlibc_write_vector_element_begin(self, \"%s\", i)) == E_TLIBC_NOERROR)", de_struct->field_list.field_list[i].identifier);
				generator_printline(&self->super, 3, "{");
				if(vector_type->st == E_ST_STRING)
				{
					generator_printline(&self->super, 4, "if((ret = tlibc_write_string(self, data->%s, %s)) != E_TLIBC_NOERROR) goto done;", de_struct->field_list.field_list[i].identifier, vector_type->string_length);
				}
				else
				{
					generator_print(&self->super, 4, "if((ret = tlibc_write_");
					generator_print_type_name(&self->super, vector_type);
					generator_print(&self->super, 0, "(self, &data->%s[i]", de_struct->field_list.field_list[i].identifier);
					if(de_struct->field_list.field_list[i].args.arg_list_num > 0)
					{
						generator_printline(&self->super, 0, ", data->%s", de_struct->field_list.field_list[i].args.arg_list[0]);
					}
					generator_printline(&self->super, 0, ")) != E_TLIBC_NOERROR) goto done;");
				}
				generator_printline(&self->super, 3, "}");
				generator_printline(&self->super, 3, "else if(ret != E_TLIBC_IGNORE)");
				generator_printline(&self->super, 4, "goto done;");

				generator_printline(&self->super, 3, "if((ret = tlibc_write_vector_element_end(self, \"%s\", i)) != E_TLIBC_NOERROR) goto done;", de_struct->field_list.field_list[i].identifier);
				generator_printline(&self->super, 2, "}");
				generator_printline(&self->super, 0, "");
				generator_printline(&self->super, 2, "if((ret = tlibc_write_vector_end(self)) != E_TLIBC_NOERROR) goto done;");				
			}
		}
		else if(de_struct->field_list.field_list[i].type.type == E_SNT_SIMPLE)
		{
			const ST_SIMPLE_TYPE *st = symbols_get_real_type(self->super.symbols, &de_struct->field_list.field_list[i].type.st);

			generator_printline(&self->super, 2, "if((ret = tlibc_write_field_begin(self, \"%s\")) == E_TLIBC_NOERROR)", de_struct->field_list.field_list[i].identifier);
			generator_printline(&self->super, 2, "{");
			if(st->st == E_ST_STRING)
			{
				generator_printline(&self->super, 3, "if((ret = tlibc_write_string(self, data->%s, %s)) != E_TLIBC_NOERROR) goto done;", de_struct->field_list.field_list[i].identifier, st->string_length);
			}
			else
			{
				generator_print(&self->super, 3, "if((ret = tlibc_write_");
				generator_print_type_name(&self->super, st);
				generator_print(&self->super, 0, "(self, &data->%s", de_struct->field_list.field_list[i].identifier);
				if(de_struct->field_list.field_list[i].args.arg_list_num > 0)
				{
					generator_print(&self->super, 0, ", data->%s", de_struct->field_list.field_list[i].args.arg_list[0]);
				}
				generator_printline(&self->super, 0, ")) != E_TLIBC_NOERROR) goto done;");
			}
			generator_printline(&self->super, 2, "}");
			generator_printline(&self->super, 2, "else if (ret != E_TLIBC_IGNORE)");
			generator_printline(&self->super, 3, "goto done;");
			generator_printline(&self->super, 2, "if((ret = tlibc_write_field_end(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_struct->field_list.field_list[i].identifier);
		}

		generator_printline(&self->super, 1, "}");
	}
	generator_printline(&self->super, 0, "");
	generator_printline(&self->super, 1, "if((ret = tlibc_write_struct_end(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_struct->name);

	generator_printline(&self->super, 0, "");
	generator_printline(&self->super, 0, "done:");
	generator_printline(&self->super, 1, "return ret;");
	generator_printline(&self->super, 0, "}");

	return E_TD_NOERROR;
}

static TD_ERROR_CODE _on_union(TLIBC_WRITER_GENERATOR *self, const ST_UNION *de_union)
{
	uint32_t i;

	generator_printline(&self->super, 0, "");

	generator_printline(&self->super, 0, "TLIBC_ERROR_CODE tlibc_write_%s(TLIBC_ABSTRACT_WRITER *self, const union %s *data, enum %s selector)", de_union->name, de_union->name, de_union->parameters.par_list[0].type.st_refer);
	generator_printline(&self->super, 0, "{");
	generator_printline(&self->super, 0, "\tTLIBC_ERROR_CODE ret = E_TLIBC_NOERROR;");
	generator_printline(&self->super, 0, "\tif((ret = tlibc_write_union_begin(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_union->name);
	generator_printline(&self->super, 0, "\tswitch(selector)");
	generator_printline(&self->super, 0, "\t{");
	for(i = 0; i < de_union->union_field_list.union_field_list_num; ++i)
	{
		const ST_SIMPLE_TYPE *st = symbols_get_real_type(self->super.symbols, &de_union->union_field_list.union_field_list[i].simple_type);

		generator_printline(&self->super, 1, "case %s:", de_union->union_field_list.union_field_list[i].key);
		generator_printline(&self->super, 2, "if((ret = tlibc_write_field_begin(self, \"%s\")) == E_TLIBC_NOERROR)", de_union->union_field_list.union_field_list[i].name);
		generator_printline(&self->super, 2, "{");
		if(st->st == E_ST_STRING)
		{
			generator_printline(&self->super, 2, "if((ret = tlibc_write_string(self, data->%s, %s)) != E_TLIBC_NOERROR) goto done;", de_union->union_field_list.union_field_list[i].name, st->string_length);
		}
		else
		{
			generator_print(&self->super, 2, "if((ret = tlibc_write_");
			generator_print_type_name(&self->super, st);
			generator_print(&self->super, 0, "(self, &data->%s)) != E_TLIBC_NOERROR) goto done;", de_union->union_field_list.union_field_list[i].name);
		}
		generator_printline(&self->super, 2, "}");
		generator_printline(&self->super, 2, "else if(ret != E_TLIBC_IGNORE)");
		generator_printline(&self->super, 3, "goto done;");

		generator_printline(&self->super, 2, "if((ret = tlibc_write_field_end(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_union->union_field_list.union_field_list[i].name);
		generator_printline(&self->super, 2, "break;");		
	}
	generator_printline(&self->super, 1, "default:");
	generator_printline(&self->super, 2, "break;");		

	generator_printline(&self->super, 1, "}");
	generator_printline(&self->super, 1, "if((ret = tlibc_write_union_end(self, \"%s\")) != E_TLIBC_NOERROR) goto done;", de_union->name);
	generator_printline(&self->super, 0, "");
	generator_printline(&self->super, 0, "done:");
	generator_printline(&self->super, 1, "return ret;");
	generator_printline(&self->super, 0, "}");

	return E_TD_NOERROR;
}

static TD_ERROR_CODE on_definition(GENERATOR *super, const YYLTYPE *yylloc, const ST_DEFINITION *definition)
{
	TLIBC_WRITER_GENERATOR *self = TLIBC_CONTAINER_OF(super, TLIBC_WRITER_GENERATOR, super);
	TLIBC_UNUSED(yylloc);
	switch(definition->type)
	{
		case E_DT_IMPORT:
			return _on_import(self, &definition->definition.de_import);				
		case E_DT_CONST:
			return E_TD_NOERROR;
		case E_DT_ENUM:
			return _on_enum(self, &definition->definition.de_enum);
		case E_DT_STRUCT:
			return _on_struct(self, &definition->definition.de_struct);
		case E_DT_UNION:
			return _on_union(self, &definition->definition.de_union);
		case E_DT_TYPEDEF:
			return E_TD_NOERROR;
		case E_DT_UNIX_COMMENT:
			return E_TD_NOERROR;
		default:
			return E_TD_ERROR;
	}
}

void tlibc_writer_generator_init(TLIBC_WRITER_GENERATOR *self, const SYMBOLS *symbols)
{
	generator_init(&self->super, symbols);

	self->super.on_document_begin = on_document_begin;
	self->super.on_document_end = on_document_end;
	self->super.on_definition = on_definition;
}
