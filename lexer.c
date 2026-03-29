#define P0_BUILD

#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

#define INV_LEXER_RULE_RET (Lexer_Rule_Return){{{0}, 0}, 0}

Lexer_Rule_Return Lexer_Rule_int(String str) {
   StrInt_Return value = std_str_to_int(str);
   Lexer_Rule_Return ret;
   ret.length = value.length;
   ARITH(TokenData, &ret.token.data, 0)->int_value = value.value;
   ret.token.type = TOKEN_TYPE_INT;
   ret.token.data[12] = 0;

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
   ret.length = counter;
   ARITH(TokenData, &ret.token.data, 0)->str_value = (String){str.buffer - str.length + 1, counter - 1};
   ret.token.type = TOKEN_TYPE_IDENTIFIER;
   ret.token.data[12] = 0;

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

Token_Unit *Lexer_run(String input, Lexer *lexer, Pool *token_pool) {
   assert(token_pool->unit_size == sizeof(Token_Unit));

   SLL_UH first_unit = {0};
   Token_Unit *head = &first_unit;
   i4 i = 0;

   while (i < input.length) {
      String rest = {input.buffer + i, input.length - i};
      i++;
      for (i4 j = 0; j < lexer->rule_count; j++) {
         Lexer_Rule_Return rule_ret = lexer->rules[j](rest);
         if (rule_ret.length) {
            Token_Unit *unit = Pool_alloc(token_pool);
            if (unit == &token_pool->null_unit) return 0;
            unit->token = rule_ret.token;
            SLL_insert_after(unit, head);
            head = unit;
            i += rule_ret.length - 1;
            break;
         }
      }
   }

   return first_unit.next;
}

void Lexer_free_tokens(Token_Unit *token, Pool *token_pool) {
   while (token) {
      if (token->token.data[12] & TOKEN_HEAP_ALLOCATED)
         free(ARITH(TokenData, &token->token.data, 0)->ptr_value);
      Token_Unit *next = token->next;
      Pool_free(token_pool, token);
      token = next;
   }
}

void print_token(Token *token) {
   switch (token->type) {
   case TOKEN_TYPE_WHITESPACE:
      putchar(' ');
      break;
   case TOKEN_TYPE_IDENTIFIER:
      printf("@%.*s", ARITH(TokenData, &token->data, 0)->str_value.length, ARITH(TokenData, &token->data, 0)->str_value.buffer);
      break;
   case TOKEN_TYPE_INT:
      printf("BF:%llX", ARITH(TokenData, &token->data, 0)->int_value);
      break;
   case TOKEN_TYPE_STRING:
      printf("\"%.*s\"", ARITH(TokenData, &token->data, 0)->str_value.length, ARITH(TokenData, &token->data, 0)->str_value.buffer);
      break;
   }
}
