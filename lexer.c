#define P0_BUILD

#include "lexer.h"

#define INV_LEXER_RULE_RET (Lexer_Rule_Return){{{0}, 0}, 0}

Lexer_Rule_Return Lexer_Rule_int(String str) {
   StrInt_Return value = std_str_to_int(str);
   Lexer_Rule_Return ret;
   ret.token.type = TOKEN_TYPE_INT;
   ret.length = value.length;
   ARITH(TokenData, &ret.token.data, 0)->int_value = value.value;

   return ret;
}

Lexer_Rule_Return Lexer_Rule_whitespace(String str) {
   if (!str.length) return INV_LEXER_RULE_RET;

   str.buffer += str.length;
   i4 counter = -str.length;

   loop: {
      u1 c = str.buffer[counter];
      if (is_space(c) && ++counter) goto loop;
   }

   return (Lexer_Rule_Return){{{0}, TOKEN_TYPE_WHITESPACE}, str.length + counter};
}

Lexer_Rule_Return Lexer_Rule_identifier(String str) {
   if (str.length < 2 || str.buffer[0] != '@') return INV_LEXER_RULE_RET;

   str.buffer += str.length;
   i4 counter = 1 - str.length;

   loop: {
      u1 c = str.buffer[counter];
      if ((is_alnum(c) || c == '_') && ++counter) goto loop;
   }

   counter = str.length + counter;
   if (counter == 1) return INV_LEXER_RULE_RET;

   Lexer_Rule_Return ret;
   ret.token.type = TOKEN_TYPE_IDENTIFIER;
   ret.length = counter;
   ARITH(TokenData, &ret.token.data, 0)->str_value = (String){str.buffer - str.length + 1, counter - 1};

   return ret;
}

void read_str(u1 *src, u1 *dest, i4 dest_size) {
   i4 src_i = 0;
   i4 dest_i = 0;
   while (dest_i != dest_size) {
      u1 c = src[src_i];
      if (c == '\\') {
         src_i++;
         c = src[src_i];
         switch (c) {
         case '0': c = '\0'; break;
         case 'n': c = '\n'; break;
         case 't': c = '\t'; break;
         }
      }

      dest[dest_i] = c;
      dest_i++, src_i++;
   }
}

Lexer_Rule_Return Lexer_Rule_string(String str) {
   if (str.length < 2 || str.buffer[0] != '"') return INV_LEXER_RULE_RET;

   str.buffer += str.length;
   i4 counter = 1 - str.length;
   i4 escape_count = 0;

   do {
      u1 c = str.buffer[counter++];
      if (c == '\\') counter++, escape_count++;
      else if (c == '"') goto success;
   } while (counter < 0);

   return INV_LEXER_RULE_RET;

success:
   Lexer_Rule_Return ret;
   ret.token.type = TOKEN_TYPE_STRING;
   ret.length = str.length + counter;

   i4 str_len = ret.length - 2 - escape_count;
   ARITH(TokenData, &ret.token.data, 0)->str_value.length = str_len;

   u1 *str_start = str.buffer - str.length + 1;

   if (!escape_count) {
      ARITH(TokenData, &ret.token.data, 0)->str_value.buffer = str_start;
      ret.token.data[12] = 0;
      return ret;
   }

   ret.token.data[12] = TOKEN_HEAP_ALLOCATED;
   ARITH(TokenData, &ret.token.data, 0)->str_value.buffer = malloc(str_len);
   read_str(str_start, ARITH(TokenData, &ret.token.data, 0)->str_value.buffer, str_len);

   return ret;
}
