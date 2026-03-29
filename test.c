#include "lexer.h"
#include <stdlib.h>
#include <stdio.h>

// ..\..\LLVM\bin\clang.exe lexer.c pool.c utils.c test.c -o program.exe -O -Wno-pointer-sign -Wno-microsoft-anon-tag -Wno-incompatible-pointer-types -Wno-compare-distinct-pointer-types
int main() {
   u1 cstr[] = " @id ";

   Lexer lexer = {
      .rules = (Lexer_Rule_Func*[]){
         Lexer_Rule_whitespace,
         Lexer_Rule_identifier,
         Lexer_Rule_string,
         Lexer_Rule_int
      },
      .rule_count = 4
   };

   String str = {cstr, sizeof(cstr) - 1};

   Pool token_pool;
   Pool_init(&token_pool, sizeof(Token_Unit));

   void *pool_block_0 = malloc(create_block_size(&token_pool, 16));
   Pool_expand(&token_pool, pool_block_0, 16);

   Token_Unit *first_token = Lexer_run(str, &lexer, &token_pool);

   fputs("Recreation: !", stdout);
   for (Token_Unit *token = first_token; token; token = token->next)
      print_token(&token->token);
   putchar('!');

   puts("\nBlocks:");
   Pool_print_blocks(&token_pool);

   Lexer_free_tokens(first_token, &token_pool);
   Pool_free_block(&token_pool, pool_block_0);
   free(pool_block_0);
   return 0;
}