#ifndef UTILS_H
#define UTILS_H

#include "basics.h"

static inline u1 char_to_int(u1 c) {
   c -= '0';
   if (c < 10) return c;
   c = (c - ('A' - '0')) & ~32;
   if (c < 26) return c + 10;
   return 255;
}

typedef struct StrInt_Return StrInt_Return;

struct StrInt_Return {
   i8 value;
   i4 length;
};

PREFIX StrInt_Return str_to_int_le(String str, i4 base);
PREFIX StrInt_Return str_to_int_be(String str, i4 base);
PREFIX StrInt_Return std_str_to_int(String str);

#endif