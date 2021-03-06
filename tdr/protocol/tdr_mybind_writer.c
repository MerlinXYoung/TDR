#include "tdr_mybind_writer.h"
#include "../tdr_platform.h"
#include "../tdr_util.h"
#include "../tdr_abstract_writer.h"
#include "../tdr_error_code.h"

#include <string.h>
#include <assert.h>
#include <stdint.h>


void tdr_mybind_writer_init(tdr_mybind_writer_t *self, MYSQL_BIND *bind_vec, uint32_t bind_vec_num)
{
	tdr_abstract_writer_init(&self->super);

	self->super.write_int8 = tdr_mybind_write_int8;
	self->super.write_int16 = tdr_mybind_write_int16;
	self->super.write_int32 = tdr_mybind_write_int32;
	self->super.write_int64 = tdr_mybind_write_int64;
	self->super.write_uint8 = tdr_mybind_write_uint8;
	self->super.write_uint16 = tdr_mybind_write_uint16;
	self->super.write_uint32 = tdr_mybind_write_uint32;
	self->super.write_uint64 = tdr_mybind_write_uint64;
	self->super.write_double = tdr_mybind_write_double;
	self->super.write_char = tdr_mybind_write_char;
	self->super.write_bool = tdr_mybind_write_bool;
	self->super.write_string = tdr_mybind_write_string;
	

	self->bind_vec = bind_vec;
	self->bind_vec_num = bind_vec_num;
	self->idx = 0;
}

tdr_error_code_t tdr_mybind_write_int8(tdr_abstract_writer_t *super, const int8_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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

tdr_error_code_t tdr_mybind_write_int16(tdr_abstract_writer_t *super, const int16_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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

tdr_error_code_t tdr_mybind_write_int32(tdr_abstract_writer_t *super, const int32_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);

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

tdr_error_code_t tdr_mybind_write_int64(tdr_abstract_writer_t *super, const int64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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


tdr_error_code_t tdr_mybind_write_uint8(tdr_abstract_writer_t *super, const uint8_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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

tdr_error_code_t tdr_mybind_write_uint16(tdr_abstract_writer_t *super, const uint16_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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

tdr_error_code_t tdr_mybind_write_uint32(tdr_abstract_writer_t *super, const uint32_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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

tdr_error_code_t tdr_mybind_write_uint64(tdr_abstract_writer_t *super, const uint64_t *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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


tdr_error_code_t tdr_mybind_write_double(tdr_abstract_writer_t *super, const double *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
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

tdr_error_code_t tdr_mybind_write_char(tdr_abstract_writer_t *super, const char *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);

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

tdr_error_code_t tdr_mybind_write_bool(tdr_abstract_writer_t *super, const bool *val)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);

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


tdr_error_code_t tdr_mybind_write_string(tdr_abstract_writer_t *super, const char* str, uint32_t str_length)
{
	tdr_error_code_t ret = E_TDR_NOERROR;
	tdr_mybind_writer_t *self = TDR_CONTAINER_OF(super, tdr_mybind_writer_t, super);
	TDR_UNUSED(str_length);

	if(self->idx >= self->bind_vec_num)
	{
		ret = E_TDR_OUT_OF_MEMORY;
		goto done;
	}

	self->bind_vec[self->idx].buffer_type = MYSQL_TYPE_STRING;
	self->bind_vec[self->idx].buffer = (void*)str;
	self->bind_vec[self->idx].buffer_length = str_length;

	++(self->idx);

	return E_TDR_NOERROR;
done:
	return ret;
}
