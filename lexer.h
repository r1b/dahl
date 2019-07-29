#ifndef LEXER_H
#define LEXER_H

// XXX: These structures are sort of confusing so read the header

// I *think* we want:
// * An STAILQ for a linked-list (where we add entries to the TAIL)
// * A TAILQ for a linked-list that we may traverse backwards
// * An SLIST for a LIFO stack (where we add / remove entries to / from the HEAD)

// If this turns out to be correct I should write some macros to rename these bc
// holy shit that is weird.
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
void display_token_list(struct TokenList *);
struct TokenList *lex(FILE *source);
struct Token *lex_identifier(FILE *);
struct Token *lex_number(FILE *);
char *render_token_kind(enum TokenKind);
char *render_token_lexeme(unsigned char *);

#endif
