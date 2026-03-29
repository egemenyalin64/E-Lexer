#define P0_BUILD

#include "utils.h"

#define INV_STRINT_RET (StrInt_Return){0, 0}

StrInt_Return str_to_int_le(String str, i4 base) {
   str.buffer += str.length;
   i4 counter = -str.length;

   i8 y = 0, coef = 1;
   u1 x;

   goto loop_start;
   loop: {
      y += x * coef;
      coef *= base;
   skip:
      counter++;
   loop_start:
      if (counter == 0) goto loop_end;
      u1 c = str.buffer[counter];
      if (c == '_') goto skip;
      x = char_to_int(c);
      if (x < base) goto loop;
   } loop_end:

   return (StrInt_Return){y, str.length + counter};
}

StrInt_Return str_to_int_be(String str, i4 base) {
   str.buffer += str.length;
   i4 counter = -str.length;

   i8 y = 0;
   u1 x;

   goto loop_start;
   loop: {
      y = y * base + x;
   skip:
      counter++;
   loop_start:
      if (counter == 0) goto loop_end;
      u1 c = str.buffer[counter];
      if (c == '_') goto skip;
      x = char_to_int(c);
      if (x < base) goto loop;
   } loop_end:

   return (StrInt_Return){y, str.length + counter};
}

StrInt_Return std_str_to_int(String str) {
   if (str.length < 4 || str.buffer[2] != ':') return INV_STRINT_RET;
   u1 endianness = str.buffer[0] & ~32;
   u1 base_c = char_to_int(str.buffer[1]);
   if (base_c == 255) return INV_STRINT_RET;

   StrInt_Return (*func)(String, u4);
   if (endianness == 'L') func = str_to_int_le;
   else if (endianness == 'B') func = str_to_int_be;
   else return INV_STRINT_RET;

   StrInt_Return ret = func((String){str.buffer + 3, str.length - 3}, base_c + 1);
   if (ret.length == 0) return INV_STRINT_RET;
   ret.length += 3;
   return ret;
}
