#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>
#include <sys/queue.h>

enum TokenKind { TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_PAREN_L, TOKEN_PAREN_R };

struct Token {
    enum TokenKind kind;
    unsigned char *lexeme;
    STAILQ_ENTRY(Token) entries;
};

STAILQ_HEAD(TokenList, Token);

struct Token *create_token(enum TokenKind kind, unsigned char *lexeme);
void free_token(struct Token *);
void free_token_list(struct TokenList *);

struct TokenList *lex(FILE *source);
struct Token *lex_identifier(FILE *);
struct Token *lex_number(FILE *);

#endif
