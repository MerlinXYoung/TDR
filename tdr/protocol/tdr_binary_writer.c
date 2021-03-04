#include "tdr_binary_writer.h"
#include "../tdr_platform.h"
#include "../tdr_util.h"
#include "../tdr_abstract_writer.h"
#include "../tdr_error_code.h"

#include <string.h>
#include <assert.h>


void tdr_binary_writer_init(tdr_binary_writer_t *self, char *addr, uint32_t size)
{
	tdr_abstract_writer_init(&self->super);

	self->super.write_bool = tdr_binary_write_bool;
	self->super.write_char = tdr_binary_write_char;
	self->super.write_double = tdr_binary_write_double;
	self->super.write_int8 = tdr_binary_write_int8;
	self->super.write_int16 = tdr_binary_write_int16;
	self->super.write_int32 = tdr_binary_write_int32;
	self->super.write_int64 = tdr_binary_write_int64;
	self->super.write_uint8 = tdr_binary_write_uint8;
	self->super.write_uint16 = tdr_binary_write_uint16;
	self->super.write_uint32 = tdr_binary_write_uint32;
	self->super.write_uint64 = tdr_binary_write_uint64;
	self->super.write_string = tdr_binary_write_string;

	self->addr = addr;
	self->size = size;
	self->offset = 0;
}

#define WRITER_CAPACITY(self) (self->size - self->offset)
#define WRITER_PTR(self) (self->addr + self->offset)

tdr_error_code_t tdr_binary_write_int8(tdr_abstract_writer_t *super, const int8_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(int8_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(int8_t*)WRITER_PTR(self) = *val;
	self->offset += sizeof(int8_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_int16(tdr_abstract_writer_t *super, const int16_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(int16_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(int16_t*)WRITER_PTR(self) = *val;
	tdr_host16_to_little(*(int16_t*)WRITER_PTR(self));
	self->offset += sizeof(int16_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_int32(tdr_abstract_writer_t *super, const int32_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(int32_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(int32_t*)WRITER_PTR(self) = *val;
	tdr_host32_to_little(*(int32_t*)WRITER_PTR(self));
	self->offset += sizeof(int32_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_int64(tdr_abstract_writer_t *super, const int64_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(int64_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(int64_t*)WRITER_PTR(self) = *val;
	tdr_host64_to_little(*(int64_t*)WRITER_PTR(self));
	self->offset += sizeof(int64_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}


tdr_error_code_t tdr_binary_write_uint8(tdr_abstract_writer_t *super, const uint8_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(uint8_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(uint8_t*)WRITER_PTR(self) = *val;
	self->offset += sizeof(uint8_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_uint16(tdr_abstract_writer_t *super, const uint16_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(uint16_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(uint16_t*)WRITER_PTR(self) = *val;
	tdr_host16_to_little(*(uint16_t*)WRITER_PTR(self));
	self->offset += sizeof(uint16_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_uint32(tdr_abstract_writer_t *super, const uint32_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(uint32_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(uint32_t*)WRITER_PTR(self) = *val;
	tdr_host32_to_little(*(uint32_t*)WRITER_PTR(self));
	self->offset += sizeof(uint32_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_uint64(tdr_abstract_writer_t *super, const uint64_t *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(uint64_t))
	{
		goto not_enough_bytebuff_size;
	}
	*(uint64_t*)WRITER_PTR(self) = *val;
	tdr_host64_to_little(*(uint64_t*)WRITER_PTR(self));
	self->offset += sizeof(uint64_t);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;

}

tdr_error_code_t tdr_binary_write_char(tdr_abstract_writer_t *super, const char *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*(char*)WRITER_PTR(self) = *val;
	self->offset += sizeof(char);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_bool(tdr_abstract_writer_t *super, const bool *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*(char*)WRITER_PTR(self) = *val;
	self->offset += sizeof(char);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_double(tdr_abstract_writer_t *super, const double *val)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	if(WRITER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*(double*)WRITER_PTR(self) = *val;
	self->offset += sizeof(double);

	return E_TDR_NOERROR;
not_enough_bytebuff_size:
	return E_TDR_OUT_OF_MEMORY;
}

tdr_error_code_t tdr_binary_write_string(tdr_abstract_writer_t *super, const char* str, uint32_t str_length)
{
	tdr_binary_writer_t *self = TDR_CONTAINER_OF(super, tdr_binary_writer_t, super);
	uint32_t str_len = 0;
	tdr_error_code_t ret= E_TDR_NOERROR;
	TDR_UNUSED(str_length);

	for(; self->offset < self->size; )
	{
		char c = (self->addr[self->offset++] = str[str_len++]);

		if(c == 0)
		{
			goto done;
		}
	}

	ret = E_TDR_OUT_OF_MEMORY;
done:
	return ret;
}
