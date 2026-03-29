#ifndef LEXER_H
#define LEXER_H

#include "utils.h"
#include "pool.h"

static inline u1 is_space(u1 c) { return c == ' ' || c == '\n' || c == '\t'; }
static inline u1 is_digit(u1 c) { return (u1)(c - '0') < 10; }
static inline u1 is_alpha(u1 c) { return (u1)((c - 'A') & ~32) < 26; }
static inline u1 is_alnum(u1 c) { return is_digit(c) || is_alpha(c); }

#define TOKEN_TYPE_WHITESPACE 0
#define TOKEN_TYPE_IDENTIFIER 1
#define TOKEN_TYPE_INT 2
#define TOKEN_TYPE_STRING 3

#define TOKEN_HEAP_ALLOCATED 1

typedef struct Token Token;
typedef struct Token_Unit Token_Unit;
typedef union TokenData TokenData;
typedef struct Lexer_Rule_Return Lexer_Rule_Return;
typedef struct Lexer Lexer;

typedef Lexer_Rule_Return Lexer_Rule_Func(String);

struct Token {
   u1 data[14];
   u2 type;
};

union TokenData {
   i8 int_value;
   String str_value;
   void *ptr_value;
};

struct Lexer_Rule_Return {
   Token token;
   i4 length;
};

struct Lexer {
   Lexer_Rule_Func **rules;
   i4 rule_count;
};

struct Token_Unit {
   PL_UH;
   Token token;
};

PREFIX Lexer_Rule_Func Lexer_Rule_int;
PREFIX Lexer_Rule_Func Lexer_Rule_whitespace;
PREFIX Lexer_Rule_Func Lexer_Rule_identifier;
PREFIX Lexer_Rule_Func Lexer_Rule_string;

PREFIX void read_str(u1 *src, u1 *dest, i4 dest_size);

PREFIX Token_Unit *Lexer_run(String input, Lexer *lexer, Pool *token_pool);
PREFIX void Lexer_free_tokens(Token_Unit *token, Pool *token_pool);
PREFIX void print_token(Token *token);

#endif