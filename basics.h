#ifndef BASICS_H
#define BASICS_H

#include <assert.h>

typedef char i1;
typedef short i2;
typedef int i4;
typedef long long i8;

typedef unsigned char u1;
typedef unsigned short u2;
typedef unsigned int u4;
typedef unsigned long long u8;

#define ARITH(T, x, o) ((T*)((u1*)(x) + o))

#ifdef P0_BUILD
   #define PREFIX __declspec(dllexport)
#else
   #define PREFIX __declspec(dllimport)
#endif

typedef struct String String;

struct String {
   u1 *buffer;
   i4 length;
};

#endif