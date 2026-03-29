#ifndef POOL_H
#define POOL_H

#include "LL.h"

typedef struct Pool_Block_Header Pool_Block_Header, PL_BH;
typedef struct Pool_Unit_Header Pool_Unit_Header, PL_UH;
typedef struct Pool Pool;

struct Pool_Block_Header {
   DLL_Unit_Header;
   i4 size;
   i4 count;
};

struct Pool_Unit_Header {
   SLL_Unit_Header;
   Pool_Block_Header *block;
};

struct Pool {
   Pool_Block_Header *head_block;
   Pool_Unit_Header *head_free;
   Pool_Block_Header null_block;
   Pool_Unit_Header null_unit;
   i4 unit_size;
};

PREFIX void  Pool_init      (Pool *pool, u4 unit_size);
PREFIX void *Pool_alloc     (Pool *pool);
PREFIX void  Pool_free      (Pool *pool, PL_UH *unit);
PREFIX void  Pool_expand    (Pool *pool, PL_BH *block, u4 size);
PREFIX void  Pool_free_block(Pool *pool, PL_BH *block);

static inline u4 create_block_size(Pool *pool, u4 count) { return sizeof(PL_BH) + pool->unit_size * count; }

#endif