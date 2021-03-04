#include "tdr_mybind_reader.h"
#include "../tdr_util.h"
#include "../tdr_abstract_reader.h"
#include "../tdr_error_code.h"


#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>


void tdr_mybind_reader_init(tdr_mybind_reader_t *self, MYSQL_BIND *bind_vec, uint32_t bind_vec_num)
{
	tdr_abstract_reader_init(&self->super);

	self->super.read_int8 = tdr_mybind_read_int8;
	self->super.read_int16 = tdr_mybind_read_int16;
	self->super.read_int32 = tdr_mybind_read_int32;
	self->super.read_int64 = tdr_mybind_read_int64;

	self->super.read_uint8 = tdr_mybind_read_uint8;
	self->super.read_uint16 = tdr_mybind_read_uint16;
	self->super.read_uint32 = tdr_mybind_read_uint32;
	self->super.read_uint64 = tdr_mybind_read_uint64;

	self->super.read_double = tdr_mybind_read_double;
	self->super.read_char = tdr_mybind_read_char;
	self->super.read_bool = tdr_mybind_read_bool;
	self->super.read_string = tdr_mybind_read_string;

	self->bind_vec = bind_vec;
	self->bind_vec_num = bind_vec_num;
	self->idx = 0;
}


tdr_error_code_t tdr_mybind_read_int8(tdr_abstract_reader_t *super, int8_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_TINY;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_int16(tdr_abstract_reader_t *super, int16_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_SHORT;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_int32(tdr_abstract_reader_t *super, int32_t *val)
{	
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONG;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_int64(tdr_abstract_reader_t *super, int64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONGLONG;
	self->bind_vec[self->idx].buffer = (void*)val;

	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_uint8(tdr_abstract_reader_t *super, uint8_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}
	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_TINY;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_uint16(tdr_abstract_reader_t *super, uint16_t *val)
{	
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_SHORT;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_uint32(tdr_abstract_reader_t *super, uint32_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONG;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_uint64(tdr_abstract_reader_t *super, uint64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_LONGLONG;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}


tdr_error_code_t tdr_mybind_read_double(tdr_abstract_reader_t *super, double *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_DOUBLE;
	self->bind_vec[self->idx].buffer = (void*)val;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_char(tdr_abstract_reader_t *super, char *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_STRING;
	self->bind_vec[self->idx].buffer = (void*)val;
	self->bind_vec[self->idx].buffer_length = 1;

	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_bool(tdr_abstract_reader_t *super, bool *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_TINY;
	self->bind_vec[self->idx].buffer = (void*)val;
	self->bind_vec[self->idx].buffer_length = 1;

	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}

tdr_error_code_t tdr_mybind_read_string(tdr_abstract_reader_t *super, char *str, uint32_t str_len)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_reader_t *self = TDR_CONTAINER_OF(super, tdr_mybind_reader_t, super);
	TDR_UNUSED(str_len);

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}


	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_STRING;
	self->bind_vec[self->idx].buffer = (void*)str;	
	self->bind_vec[self->idx].buffer_length = strlen(str) + 1;
	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}
