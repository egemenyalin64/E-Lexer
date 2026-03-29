#define P0_BUILD

#include "lexer.h"

#define INV_LEXER_RULE_RET (Lexer_Rule_Return){{{0}, 0}, 0}

Lexer_Rule_Return Lexer_Rule_int(String str) {
   StrInt_Return value = std_str_to_int(str);
   Lexer_Rule_Return ret;
   ret.token.type = TOKEN_TYPE_INT;
   ret.length = value.length;
   ((TokenData*)&ret.token.data)->int_value = value.value;

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
   ((TokenData*)&ret.token.data)->str_value = (String){str.buffer - str.length + 1, counter - 1};

   return ret;
}
