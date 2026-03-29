#define P0_BUILD

#include "pool.h"

void Pool_init(Pool *pool, u4 unit_size) {
   pool->unit_size = unit_size;

   pool->null_block.size = 0;
   pool->null_block.next = &pool->null_block;
   pool->null_block.prev = &pool->null_block;
   pool->head_block = &pool->null_block;

   pool->null_unit.next = &pool->null_unit;
   pool->null_unit.block = &pool->null_block;
   pool->head_free = &pool->null_unit;
}

void *Pool_alloc(Pool *pool) {
   PL_UH *unit = pool->head_free;
   PL_BH *block = unit->block;

   block->count++;
   pool->head_free = unit->next;

   return unit;
}

void Pool_free(Pool *pool, PL_UH *unit) {
   PL_BH *block = unit->block;

   block->count--;
   unit->next = pool->head_free;
   pool->head_free = unit;
}

void Pool_expand(Pool *pool, PL_BH *block, u4 size) {
   DLL_insert_before(block, &pool->null_block);
   block->size = size;
   block->count = 0;

   PL_UH *unit = ARITH(PL_UH, block, sizeof(PL_BH));

   while (size--) {
      unit->block = block;
      unit->next = pool->head_free;
      pool->head_free = unit;
      unit = ARITH(PL_UH, unit, pool->unit_size);
   }
}

void Pool_free_block(Pool *pool, PL_BH *block) {
   assert(block->count == 0);

   DLL_pop(block);

   PL_UH *unit = pool->head_free, *next;

   do {
      next = unit->next;
      if (next->block == block)
         unit->next = next->next;
      else
         unit = next;
   } while (unit != &pool->null_unit);

   if (pool->head_free->block == block)
      pool->head_free = pool->head_free->next;
}

