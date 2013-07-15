#include "hotprotocol/hp_compact_reader.h"
#include "hotpot/hp_error.h"
#include "hotpot/hp_number.h"
#include "hotprotocol/hp_abstract_reader.h"

#include <string.h>
#include <assert.h>

hpint32 ddekit_compact_encoding_reader_init(HP_COMPACT_READER *self, const void *addr, hpuint32 size)
{
	self->dpr.read_struct_begin = ddekit_compact_encoding_read_struct_begin;
	self->dpr.read_struct_end = ddekit_compact_encoding_read_struct_end;
	self->dpr.read_vector_begin = ddekit_compact_encoding_read_vector_begin;
	self->dpr.read_vector_end = ddekit_compact_encoding_read_vector_end;
	self->dpr.read_field_begin = ddekit_compact_encoding_read_field_begin;
	self->dpr.read_field_end = ddekit_compact_encoding_read_field_end;
	self->dpr.read_enum = ddekit_compact_encoding_read_enum;
	self->dpr.read_char = ddekit_compact_encoding_read_hpchar;
	self->dpr.read_double = ddekit_compact_encoding_read_hpdouble;
	self->dpr.read_int8 = ddekit_compact_encoding_read_hpint8;
	self->dpr.read_int16 = ddekit_compact_encoding_read_hpint16;
	self->dpr.read_int32 = ddekit_compact_encoding_read_hpint32;
	self->dpr.read_int64 = ddekit_compact_encoding_read_hpint64;
	self->dpr.read_uint8 = ddekit_compact_encoding_read_hpuint8;
	self->dpr.read_uint16 = ddekit_compact_encoding_read_hpuint16;
	self->dpr.read_uint32 = ddekit_compact_encoding_read_hpuint32;
	self->dpr.read_uint64 = ddekit_compact_encoding_read_hpuint64;



	self->addr = (const char*)addr;
	self->size = size;
	self->offset = 0;

	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_reader_fini(HP_COMPACT_READER *self)
{
	self->dpr.read_struct_begin = NULL;
	self->dpr.read_struct_end = NULL;
	self->dpr.read_vector_begin = NULL;
	self->dpr.read_vector_end = NULL;
	self->dpr.read_field_begin = NULL;
	self->dpr.read_field_end = NULL;
	self->dpr.read_enum = NULL;
	self->dpr.read_char = NULL;
	self->dpr.read_double = NULL;
	self->dpr.read_int8 = NULL;
	self->dpr.read_int16 = NULL;
	self->dpr.read_int32 = NULL;
	self->dpr.read_int64 = NULL;
	self->dpr.read_uint8 = NULL;
	self->dpr.read_uint16 = NULL;
	self->dpr.read_uint32 = NULL;
	self->dpr.read_uint64 = NULL;


	self->addr = NULL;
	self->size = 0;
	self->offset = 0;
	return E_HP_NOERROR;
}

#define COMPACT_encoding_READER_CAPACITY(self) (self->size - self->offset)

#define COMPACT_encoding_READER_PTR(self) (self->addr + self->offset)

hpint32 ddekit_compact_encoding_read_struct_begin(HPAbstractReader *super, const char *struct_name)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_read_struct_end(HPAbstractReader *super, const char *struct_name)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_read_vector_begin(HPAbstractReader *super)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_read_vector_end(HPAbstractReader *super)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_read_field_begin(HPAbstractReader *super, const char *var_name, hpuint32 len)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_read_field_end(HPAbstractReader *super, const char *var_name, hpuint32 len)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	return E_HP_NOERROR;
}

hpint32 ddekit_compact_encoding_read_enum(HPAbstractReader *super, hpint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint32_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode32(hp_little_to_host32(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}

hpint32 ddekit_compact_encoding_read_hpchar(HPAbstractReader *super, char *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(char))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(char*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(char);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 ddekit_compact_encoding_read_hpdouble(HPAbstractReader *super, double *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(double))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(double*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(double);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 ddekit_compact_encoding_read_hpint8(HPAbstractReader *super, hpint8 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(hpint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(hpint8*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(hpint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}


hpint32 ddekit_compact_encoding_read_hpint16(HPAbstractReader *super, hpint16 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	hpuint16 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint16_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode16(hp_little_to_host16(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}


hpint32 ddekit_compact_encoding_read_hpint32(HPAbstractReader *super, hpint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint32_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode32(hp_little_to_host32(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}


hpint32 ddekit_compact_encoding_read_hpint64(HPAbstractReader *super, hpint64 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	hpuint64 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint64_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_zigzag_decode64(hp_little_to_host64(res));
	self->offset +=buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}



hpint32 ddekit_compact_encoding_read_hpuint8(HPAbstractReader *super, hpuint8 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	if(COMPACT_encoding_READER_CAPACITY(self) < sizeof(hpuint8))
	{
		goto not_enough_bytebuff_size;
	}
	*val = *(hpuint8*)COMPACT_encoding_READER_PTR(self);
	self->offset += sizeof(hpuint8);

	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return E_HP_NOT_ENOUGH_BYTEBUFF_SIZE;
}



hpint32 ddekit_compact_encoding_read_hpuint16(HPAbstractReader *super, hpuint16 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	hpuint16 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint16_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_little_to_host16(res);
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}


hpint32 ddekit_compact_encoding_read_hpuint32(HPAbstractReader *super, hpuint32 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	hpuint32 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint32_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_little_to_host32(res);
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}



hpint32 ddekit_compact_encoding_read_hpuint64(HPAbstractReader *super, hpuint64 *val)
{
	HP_COMPACT_READER *self = HP_CONTAINER_OF(super, HP_COMPACT_READER, dpr);
	hpuint64 res;
	hpuint32 buff_size = COMPACT_encoding_READER_CAPACITY(self);
	int ret = hp_varint64_decode(COMPACT_encoding_READER_PTR(self), &buff_size, &res);
	if(ret != E_HP_NOERROR)
	{
		goto not_enough_bytebuff_size;
	}
	*val = hp_little_to_host64(res);
	self->offset += buff_size;
	return E_HP_NOERROR;
not_enough_bytebuff_size:
	return ret;
}
