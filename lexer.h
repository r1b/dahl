#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

enum TokenKind {IDENTIFIER, NUMBER, PAREN_L, PAREN_R};

struct Token {
    enum TokenKind kind;
    unsigned char* lexeme;
    struct Token* next;
    struct Token* prev;
};

struct TokenList {
    struct Token* head;
    struct Token* tail;
};

struct TokenList* add_token(struct TokenList*, struct Token*);
void display_token_list(struct TokenList*);
struct TokenList* lex(FILE* source);
struct Token* lex_identifier(FILE*);
struct Token* lex_number(FILE*);
char* render_token_kind(enum TokenKind);
char* render_token_lexeme(unsigned char*);

#endif
