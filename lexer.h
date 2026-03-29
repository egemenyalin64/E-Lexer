#ifndef LEXER_H
#define LEXER_H

#include "utils.h"

static inline u1 is_space(u1 c) { return c == ' ' || c == '\n' || c == '\t'; }
static inline u1 is_digit(u1 c) { return (u1)(c - '0') < 10; }
static inline u1 is_alpha(u1 c) { return (u1)((c - 'A') & ~32) < 26; }
static inline u1 is_alnum(u1 c) { return is_digit(c) || is_alnum(c); }

#define TOKEN_TYPE_WHITESPACE 0
#define TOKEN_TYPE_IDENTIFIER 1
#define TOKEN_TYPE_INT 2

typedef struct Token Token;
typedef union TokenData TokenData;
typedef struct Lexer_Rule_Return Lexer_Rule_Return;

typedef Lexer_Rule_Return Lexer_Rule_Func(String);

struct Token {
   u1 data[14];
   u2 type;
};

union TokenData {
   i8 int_value;
   String str_value;
};

struct Lexer_Rule_Return {
   Token token;
   i4 length;
};

PREFIX Lexer_Rule_Func Lexer_Rule_int;
PREFIX Lexer_Rule_Func Lexer_Rule_whitespace;
PREFIX Lexer_Rule_Func Lexer_Rule_identifier;

#endif