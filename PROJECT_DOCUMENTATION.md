# P0 Project Documentation

## Overview
P0 is a C library project consisting of memory management, utility functions, and lexical analysis components.

## Module Descriptions

### basics.h
Foundation module providing:
- **Type Definitions**: Portable integer types (i1, i2, i4, i8 for signed; u1, u2, u4, u8 for unsigned)
- **Macros**: ARITH for pointer arithmetic, PREFIX for DLL export/import
- **String Structure**: Simple buffer-based string with length tracking

### LL.h (Linked Lists)
Linked list utilities:
- **SLL_Unit_Header**: Singly-linked list node with next pointer
- **DLL_Unit_Header**: Doubly-linked list node with next and prev pointers
- **Functions**:
  - `DLL_insert()`: Insert unit between prev and next nodes
  - `DLL_insert_after()`: Insert after a specific node
  - `DLL_insert_before()`: Insert before a specific node
  - `DLL_pop()`: Remove unit from list

### pool.h & pool.c (Memory Pool)
Memory pool allocator:
- **Pool**: Manages free and allocated units across multiple blocks
- **Functions**:
  - `Pool_init()`: Initialize pool with unit size
  - `Pool_alloc()`: Allocate a unit from free list
  - `Pool_free()`: Return unit to free list
  - `Pool_expand()`: Add new block with pre-allocated units
  - `Pool_free_block()`: Remove empty block and consolidate free list

### utils.h & utils.c (String-to-Integer Conversion)
Number parsing utilities:
- **str_to_int_le()**: Little-endian string-to-integer conversion
- **str_to_int_be()**: Big-endian string-to-integer conversion
- **std_str_to_int()**: Standard format parser (L/B:base:digits)
- **char_to_int()**: Character digit conversion (supports 0-9, A-Z)

### lexer.h & lexer.c (Lexical Analysis)
Tokenization system:
- **Token Structure**: 14-byte data field with 2-byte type identifier
- **Token Types**: WHITESPACE, IDENTIFIER, INT
- **Rules**:
  - `Lexer_Rule_int()`: Parse formatted integers
  - `Lexer_Rule_whitespace()`: Match whitespace sequences
  - `Lexer_Rule_identifer()`: Match @-prefixed identifiers (alphanumeric + underscore)

## Key Design Patterns
- Intrusive data structures (headers embedded in user structs)
- DLL-based memory management with sentinel nodes
- Zero-cost abstractions using inline functions
- Custom goto-based parsing loops for efficiency