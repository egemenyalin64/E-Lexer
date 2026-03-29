#ifndef LL_H
#define LL_H

#include "basics.h"

typedef struct SLL_Unit_Header SLL_Unit_Header, SLL_UH;
typedef struct DLL_Unit_Header DLL_Unit_Header, DLL_UH;

struct SLL_Unit_Header {
   SLL_Unit_Header *next;
};

struct DLL_Unit_Header {
   DLL_Unit_Header *next;
   DLL_Unit_Header *prev;
};

static inline void DLL_insert(DLL_UH *unit, DLL_UH *prev, DLL_UH *next) {
   unit->prev = prev, unit->next = next;
   prev->next = unit, next->prev = unit;
}

static inline void DLL_insert_after(DLL_UH *unit, DLL_UH *prev) { DLL_insert(unit, prev, prev->next); }
static inline void DLL_insert_before(DLL_UH *unit, DLL_UH *next) { DLL_insert(unit, next->prev, next); }

static inline void DLL_pop(DLL_UH *unit) {
   DLL_UH *prev = unit->prev, *next = unit->next;
   prev->next = next, next->prev = prev;
}

static inline void SLL_insert_after(SLL_UH *unit, SLL_UH *prev) {
   unit->next = prev->next;
   prev->next = unit;
}

#endif