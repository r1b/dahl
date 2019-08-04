#ifndef LEXER_H
#define LEXER_H

#include <sys/queue.h>
#include <stdio.h>

enum TokenKind
{
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_PAREN_L,
    TOKEN_PAREN_R
};

struct Token
{
    enum TokenKind kind;
    unsigned char *lexeme;
    // clang-format off
    STAILQ_ENTRY(Token) entries;
    //clang-format on
};

STAILQ_HEAD(TokenList, Token);

struct TokenList *add_token(struct TokenList *, struct Token *);
void free_token(struct Token*);
void free_token_list(struct TokenList*);

struct TokenList *lex(FILE *source);
struct Token *lex_identifier(FILE *);
struct Token *lex_number(FILE *);

#endif
