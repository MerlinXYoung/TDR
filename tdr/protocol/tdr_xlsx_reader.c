#include "tdr_xlsx_reader.h"
#include "../tdr_util.h"
#include "../tdr_abstract_reader.h"
#include "../tdr_error_code.h"
#include "tdr_xml_reader.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>

const char* tdr_xlsx_reader_workbook_rels_search_file(tdr_xlsx_reader_t *self, const char* rid);
const char* tdr_xlsx_reader_workbook_search_rid(tdr_xlsx_reader_t *self, const char* sheet);
void tdr_xlsx_reader_load_sharedstring(tdr_xlsx_reader_t *self);
tdr_error_code_t tdr_xlsx_reader_loadsheet(tdr_xlsx_reader_t *self, uint32_t bindinfo_row);

tdr_error_code_t tdr_xlsx_reader_init(tdr_xlsx_reader_t *self, const char *file_name)
{
	tdr_error_code_t ret = E_TDR_NOERROR;

	ret = tdr_unzip_init(&self->unzip, file_name);
	if(ret != E_TDR_NOERROR)
	{
		goto ERROR_RET;
	}

	//load workbook
	ret = tdr_unzip_locate(&self->unzip, "xl/workbook.xml");
	if(ret != E_TDR_NOERROR)
	{
		goto fini_unzip;
	}
	ret = tdr_unzip_open_current_file(&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto fini_unzip;
	}
	self->workbook_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->workbook_buff = (char*)malloc(self->workbook_buff_size);
	ret = tdr_read_current_file(&self->unzip, self->workbook_buff, &self->workbook_buff_size);
	if(ret != E_TDR_NOERROR)
	{
		tdr_unzip_close_current_file (&self->unzip);
		goto free_workbook;
	}
	ret = tdr_unzip_close_current_file (&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto free_workbook;
	}

	//load workbook rels
	ret = tdr_unzip_locate(&self->unzip, "xl/_rels/workbook.xml.rels");
	if(ret != E_TDR_NOERROR)
	{
		goto free_workbook;
	}
	ret = tdr_unzip_open_current_file(&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto free_workbook;
	}
	self->workbook_rels_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->workbook_rels_buff = (char*)malloc(self->workbook_rels_buff_size);
	ret = tdr_read_current_file(&self->unzip, self->workbook_rels_buff, &self->workbook_rels_buff_size);
	if(ret != E_TDR_NOERROR)
	{
		tdr_unzip_close_current_file (&self->unzip);
		goto free_workbook_rels;
	}
	ret = tdr_unzip_close_current_file (&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto free_sharedstring;
	}


	//load shared string
	ret = tdr_unzip_locate(&self->unzip, "xl/sharedStrings.xml");
	if(ret != E_TDR_NOERROR)
	{
		goto free_workbook_rels;
	}
	ret = tdr_unzip_open_current_file(&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto free_workbook_rels;
	}
	self->sharedstring_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->sharedstring_buff = (char*)malloc(self->sharedstring_buff_size);	
	ret = tdr_read_current_file(&self->unzip, self->sharedstring_buff, &self->sharedstring_buff_size);
	if(ret != E_TDR_NOERROR)
	{
		tdr_unzip_close_current_file (&self->unzip);
		goto free_sharedstring;
	}
	ret = tdr_unzip_close_current_file (&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto free_sharedstring;
	}
	tdr_xlsx_reader_load_sharedstring(self);

	tdr_abstract_reader_init(&self->super);

	self->last_col = -1;
	self->last_col_str[0] = 0;

	self->super.read_vector_begin = tdr_xlsx_read_vector_begin;
	self->super.read_vector_element_begin = tdr_xlsx_read_vector_element_begin;
	self->super.read_field_begin = tdr_xlsx_read_field_begin;	
	self->super.read_enum_begin = tdr_xlsx_read_enum_begin;

	self->super.read_int8 = tdr_xlsx_read_int8;
	self->super.read_int16 = tdr_xlsx_read_int16;
	self->super.read_int32 = tdr_xlsx_read_int32;
	self->super.read_int64 = tdr_xlsx_read_int64;

	self->super.read_uint8 = tdr_xlsx_read_uint8;
	self->super.read_uint16 = tdr_xlsx_read_uint16;
	self->super.read_uint32 = tdr_xlsx_read_uint32;
	self->super.read_uint64 = tdr_xlsx_read_uint64;

	self->super.read_double = tdr_xlsx_read_double;
	self->super.read_char = tdr_xlsx_read_char;
	self->super.read_string = tdr_xlsx_read_string;
	self->super.enable_name = TRUE;
	self->use_cache = TRUE;
	self->hash_cache = NULL;
	self->row_index = 0;
	self->pre_read_uint32 = FALSE;
	
	tdr_hash_init(&self->name2index, self->hash_bucket, TDR_XLSX_HASH_BUCKET);
	

	return E_TDR_NOERROR;
free_sharedstring:
	free(self->sharedstring_buff);
free_workbook_rels:
	free(self->workbook_rels_buff);
free_workbook:
	free(self->workbook_buff);
fini_unzip:
	tdr_unzip_fini(&self->unzip);
ERROR_RET:
	return E_TDR_ERROR;
}

#define XL_PREFIX "xl/"
 tdr_error_code_t tdr_xlsx_reader_open_sheet(tdr_xlsx_reader_t *self, const char* sheet, uint32_t bindinfo_row)
{
	size_t i ;
	tdr_error_code_t ret = E_TDR_NOERROR;
	const char *rid;
	const char *file;
	char sheet_file[TDR_MAX_PATH_LENGTH] = {XL_PREFIX};

	rid = tdr_xlsx_reader_workbook_search_rid(self, sheet);
	if(rid == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto ERROR_RET;
	}

	file = tdr_xlsx_reader_workbook_rels_search_file(self, rid);
	if(file == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto ERROR_RET;
	}
	strncpy(sheet_file + strlen(XL_PREFIX), file, TDR_MAX_PATH_LENGTH - strlen(XL_PREFIX) - 1);

	ret = tdr_unzip_locate(&self->unzip, sheet_file);
	if(ret != E_TDR_NOERROR)
	{
		goto ERROR_RET;
	}

	ret = tdr_unzip_open_current_file(&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto ERROR_RET;
	}
	self->sheet_buff_size = self->unzip.cur_file_info.uncompressed_size;
	self->sheet_buff = (char*)malloc(self->sheet_buff_size);	
	ret = tdr_read_current_file(&self->unzip, self->sheet_buff, &self->sheet_buff_size);
	if(ret != E_TDR_NOERROR)
	{
		tdr_unzip_close_current_file (&self->unzip);
		goto free_sheet;
	}
	ret = tdr_unzip_close_current_file (&self->unzip);
	if(ret != E_TDR_NOERROR)
	{
		goto free_sheet;
	}
	if(tdr_xlsx_reader_loadsheet(self, bindinfo_row) != E_TDR_NOERROR)
	{
		goto free_sheet;
	}	

	for(i = 0; i < self->cell_col_size; ++i)
	{
		if(!self->bindinfo_row[i].empty)
		{
			tdr_hash_insert(&self->name2index, self->bindinfo_row[i].val_begin
				, self->bindinfo_row[i].val_end - self->bindinfo_row[i].val_begin, &self->bindinfo_row[i].name2index);
		}
		if(self->use_cache)
		{
			self->hash_cache[i] = NULL;
		}
	}

	return E_TDR_NOERROR;
free_sheet:
	free(self->sheet_buff);
ERROR_RET:
	return E_TDR_ERROR;
}

uint32_t tdr_xlsx_reader_num_rows(tdr_xlsx_reader_t *self)
{
	return self->cell_row_size;
}

void tdr_xlsx_reader_row_seek(tdr_xlsx_reader_t *self, uint32_t offset)
{
	self->super.name_ptr = self->super.name;
	self->curr_cell = NULL;
	self->read_enum_name_once = FALSE;
	self->row_index = 0;
	self->curr_row = self->cell_matrix + (offset - self->cell_min_pos.row) * self->cell_col_size;	
}

void tdr_xlsx_reader_close_sheet(tdr_xlsx_reader_t *self)
{
	free(self->sheet_buff);
	tdr_hash_clear(&self->name2index);
}

void tdr_xlsx_reader_fini(tdr_xlsx_reader_t *self)
{
	if(self->hash_cache)
	{
		free(self->hash_cache);
	}

	if(self->cell_matrix)
	{
		free(self->cell_matrix);
	}

	if(self->workbook_buff)
	{
		free(self->workbook_buff);
	}	

	if(self->workbook_rels_buff)
	{
		free(self->workbook_rels_buff);
	}
	
	if(self->sharedstring_buff)
	{
		free(self->sharedstring_buff);
	}
	
	if(self->sharedstring_begin_list)
	{
		free(self->sharedstring_begin_list);
	}

	if(self->sharedstring_end_list)
	{
		free(self->sharedstring_end_list);
	}
	
	tdr_unzip_fini(&self->unzip);
}

//可以把这个函数放到叶子节点， 以支持多层结构体。
static void tdr_xlsx_locate(tdr_xlsx_reader_t *self)
{
	tdr_xlsx_cell_s *cell;
	tdr_hash_head_t *head;

	self->curr_cell = NULL;
	if((self->use_cache) && (self->hash_cache) && (self->hash_cache[self->row_index]))
	{
		cell = self->hash_cache[self->row_index];
		self->super.enable_name = FALSE;
	}
	else
	{	
		if(self->super.name_ptr <= self->super.name)
		{
			goto done;
		}
		head = tdr_hash_find(&self->name2index, self->super.name + 1, self->super.name_ptr - self->super.name - 1);
		if(head == NULL)
		{
			goto done;
		}
		cell = TDR_CONTAINER_OF(head, tdr_xlsx_cell_s, name2index);
		if(self->use_cache)
		{
			self->hash_cache[self->row_index] = cell;
		}
	}
	++self->row_index;
	self->curr_cell = self->curr_row + (cell - self->bindinfo_row);
	self->last_col = self->curr_cell - self->curr_row;

done:	
	return;
}

tdr_error_code_t tdr_xlsx_read_vector_begin(tdr_abstract_reader_t *super, const char* vec_name)
{
	const char *ch;
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	TDR_UNUSED(vec_name);
	self->pre_read_uint32 = TRUE;
	
	tdr_xlsx_locate(self);
	if(self->curr_cell == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		self->pre_u32 = 0;
		goto done;
	}
	self->pre_u32 = 1;
	for(ch = self->curr_cell->val_start; ch < self->curr_cell->val_limit; ++ch)
	{
		if(*ch == ';')
		{
			++(self->pre_u32);
		}
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_vector_element_begin(tdr_abstract_reader_t *super, const char* var_name, uint32_t index)
{
	size_t i;
	const char *ch;
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	TDR_UNUSED(var_name);
	if(self->curr_cell == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		goto done;
	}
	i = 0;
	self->curr_cell->val_begin = self->curr_cell->val_start;
	self->curr_cell->val_end = self->curr_cell->val_limit;
	for(ch = self->curr_cell->val_start; ch < self->curr_cell->val_limit; ++ch)
	{
		if(*ch == ';')
		{
			++i;			
			if(i == index)
			{
				self->curr_cell->val_begin = ch + 1;
			}
			if(i > index)
			{
				self->curr_cell->val_end = ch;
			}			
		}
	}
	

done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_field_begin(tdr_abstract_reader_t *super, const char *var_name)
{
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);

	if(self->pre_read_uint32)
	{
		return E_TDR_NOERROR;
	}

	TDR_UNUSED(var_name);
	tdr_xlsx_locate(self);

	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xlsx_read_enum_begin(tdr_abstract_reader_t *super, const char *enum_name)
{
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	TDR_UNUSED(enum_name);

	self->read_enum_name_once = TRUE;

	return E_TDR_NOERROR;
}

tdr_error_code_t tdr_xlsx_read_int8(tdr_abstract_reader_t *super, int8_t *val)
{
	int64_t i64;
	tdr_error_code_t ret = tdr_xlsx_read_int64(super, &i64);
	if(ret != E_TDR_NOERROR)
	{
		goto done;
	}

	*val = (int8_t)i64;
	if(*val != i64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_int16(tdr_abstract_reader_t *super, int16_t *val)
{
	int64_t i64;
	tdr_error_code_t ret = tdr_xlsx_read_int64(super, &i64);
	if(ret != E_TDR_NOERROR)
	{
		goto done;
	}

	*val = (int16_t)i64;
	if(*val != i64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_int32(tdr_abstract_reader_t *super, int32_t *val)
{
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	int64_t i64;
	tdr_error_code_t ret;
	if(self->read_enum_name_once)
	{
		self->read_enum_name_once = FALSE;
		ret = E_TDR_PLEASE_READ_ENUM_NAME;
		goto done;
	}
	ret = tdr_xlsx_read_int64(super, &i64);
	if(ret != E_TDR_NOERROR)
	{
		goto done;
	}

	*val = (int32_t)i64;
	if(*val != i64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_int64(tdr_abstract_reader_t *super, int64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;
		goto done;
	}

	errno = 0;
	*val = strtoll(self->curr_cell->val_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TDR_ERRNO;
		goto done;
	}

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_uint8(tdr_abstract_reader_t *super, uint8_t *val)
{
	uint64_t ui64;
	tdr_error_code_t ret = tdr_xlsx_read_uint64(super, &ui64);
	if(ret != E_TDR_NOERROR)
	{
		goto done;
	}

	*val = (uint8_t)ui64;
	if(*val != ui64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_uint16(tdr_abstract_reader_t *super, uint16_t *val)
{
	uint64_t ui64;
	tdr_error_code_t ret = tdr_xlsx_read_uint64(super, &ui64);
	if(ret != E_TDR_NOERROR)
	{
		goto done;
	}
	*val = (uint16_t)ui64;
	if(*val != ui64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_uint32(tdr_abstract_reader_t *super, uint32_t *val)
{
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	tdr_error_code_t ret = E_TDR_NOERROR;
	uint64_t ui64;
	if(self->pre_read_uint32)
	{
		self->pre_read_uint32 = FALSE;
		*val = self->pre_u32;
		goto done;
	}

	ret = tdr_xlsx_read_uint64(super, &ui64);
	if(ret != E_TDR_NOERROR)
	{
		goto done;
	}
	*val = (uint32_t)ui64;
	if(*val != ui64)
	{
		return E_TDR_INTEGER_OVERFLOW;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_uint64(tdr_abstract_reader_t *super, uint64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;
		goto done;
	}

	errno = 0;
	*val = strtoull(self->curr_cell->val_begin, NULL, 10);
	if(errno != 0)
	{
		ret = E_TDR_ERRNO;
		goto done;
	}

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_double(tdr_abstract_reader_t *super, double *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	if(self->curr_cell == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;		
		goto done;
	}

	errno = 0;
	*val = strtod(self->curr_cell->val_begin, NULL);
	if(errno != 0)
	{
		ret = E_TDR_ERRNO;
		goto done;
	}

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_char(tdr_abstract_reader_t *super, char *val)
{
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	const char* curr;
	tdr_error_code_t ret = E_TDR_NOERROR;
	if(self->curr_cell == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		*val = 0;
		goto done;
	}

	curr = tdr_xml_str2c(self->curr_cell->val_begin
		, self->curr_cell->val_end, val);
	if(curr == NULL)
	{
		return E_TDR_OUT_OF_MEMORY;
	}
done:
	return ret;
}

tdr_error_code_t tdr_xlsx_read_string(tdr_abstract_reader_t *super, char *str, uint32_t str_len)
{
	tdr_xlsx_reader_t *self = TDR_CONTAINER_OF(super, tdr_xlsx_reader_t, super);
	uint32_t len = 0;
	const char* curr;
	const char* limit;
	tdr_error_code_t ret = E_TDR_NOERROR;
	if(self->curr_cell == NULL)
	{
		ret = E_TDR_NOT_FOUND;
		goto done;
	}
	if(self->curr_cell->empty)
	{
		if(str_len > 0)
		{
			*str = 0;
		}
		goto done;
	}
	curr = self->curr_cell->val_begin;
	limit = self->curr_cell->val_end;
	while(curr < limit)
	{
		char c;
		curr = tdr_xml_str2c(curr, limit, &c);
		if(curr == NULL)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto done;
		}

		if(c == '<')
		{
			break;
		}
		if(len >= str_len)
		{
			ret = E_TDR_OUT_OF_MEMORY;
			goto done;
		}
		str[len++] = c;		
	}
	str[len] = 0;
done:
	return ret;
}

size_t tdr_xlsx_current_col(tdr_xlsx_reader_t *self)
{
	return self->last_col + 1;
}

size_t tdr_xlsx_str2num(const char* str)
{
	size_t len;
	size_t i;
	uint32_t num = 0;
	if(str == NULL)
	{
		return 0;
	}
	len = strlen(str);

	for(i = 0; i < len; ++i)
	{
		num = num * 26 + str[i] - 'A' + 1;
	}
	return num;
}

const char* tdr_xlsx_num2str(int num, char *str, size_t str_max_len)
{             
	int r = 0;
	size_t str_len = str_max_len;
	if(str_len <= 0)
	{
		return NULL;
	}
	--str_len;
	str[str_len] = 0;

	while(num != 0)  
	{
		char ch;
		r = num % 26;
		if(r == 0)
		{
			ch = 'Z';
		}
		else
		{
			ch = (char)('A' + r - 1);
		}
		if(str_len <= 0)
		{
			return NULL;
		}
		--str_len;
		str[str_len] = ch;		
		if(ch == 'Z')
		{
			num = num / 26 - 1;
		}
		else
		{
			num /= 26 ;
		}
	}
	return str + str_len;
}