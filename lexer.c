#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

struct Token *lex_identifier(FILE *source)
{
    int cur_;
    unsigned char cur;
    bool done = false;
    unsigned char *identifier = NULL;
    size_t size = 0;
    struct Token *token;

    while (!done && ((cur_ = fgetc(source)) != EOF))
    {
        cur = (unsigned char)cur_;

        switch (cur)
        {
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '!':
        case '$':
        case '%':
        case '&':
        case '*':
        case '/':
        case ':':
        case '<':
        case '=':
        case '>':
        case '?':
        case '^':
        case '_':
        case '~':
        case '+':
        case '-':
        case '.':
        case '@':
            size += 1;

            REALLOC(identifier, size * sizeof(unsigned char));
            identifier[size - 1] = cur;
            break;
        default:
            ungetc(cur, source);
            done = true;
            break;
        }
    }

    size += 1;

    REALLOC(identifier, size * sizeof(unsigned char));
    identifier[size - 1] = '\0';

    MALLOC(token, sizeof(struct Token));
    token->kind = TOKEN_IDENTIFIER;
    token->lexeme = identifier;

    return token;
}

struct Token *lex_number(FILE *source)
{
    int cur_;
    unsigned char cur;
    bool done = false;
    unsigned char *digits = NULL;
    size_t size = 0;
    struct Token *token;

    while (!done && ((cur_ = fgetc(source)) != EOF))
    {
        cur = (unsigned char)cur_;

        switch (cur)
        {
        case '0' ... '9':
            if (cur == '0' && size == 0)
            {
                fprintf(stderr, "Numbers cannot begin with `0`");
                exit(1);
            }

            size += 1;
            REALLOC(digits, size * sizeof(unsigned char));
            digits[size - 1] = cur;
            break;
        default:
            ungetc(cur, source);
            done = true;
            break;
        }
    }

    size += 1;
    REALLOC(digits, size * sizeof(unsigned char));
    digits[size] = '\0';

    MALLOC(token, sizeof(struct Token));
    token->kind = TOKEN_NUMBER;
    token->lexeme = digits;

    return token;
}

struct TokenList *add_token(struct TokenList *token_list, struct Token *token)
{
    if (token_list->head == NULL)
    {
        token->prev = NULL;
        token->next = NULL;

        token_list->head = token;
        token_list->tail = token;

        return token_list;
    }

    token->prev = token_list->tail;
    token->next = NULL;

    token_list->tail->next = token;
    token_list->tail = token;

    return token_list;
}

struct TokenList *lex(FILE *source)
{
    int cur_;
    unsigned char cur;
    struct TokenList *token_list;

    MALLOC(token_list, sizeof(struct TokenList));
    token_list->head = NULL;
    token_list->tail = NULL;

    while ((cur_ = fgetc(source)) != EOF)
    {
        struct Token *token;
        cur = (unsigned char)cur_;

        switch (cur)
        {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            break;
        case '(':
            MALLOC(token, sizeof(struct Token));
            token->kind = TOKEN_PAREN_L;
            token->lexeme = NULL;

            token_list = add_token(token_list, token);
            break;
        case ')':
            MALLOC(token, sizeof(struct Token));
            token->kind = TOKEN_PAREN_R;
            token->lexeme = NULL;

            token_list = add_token(token_list, token);
            break;
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '+':
        case '-':
        case '!':
        case '$':
        case '%':
        case '&':
        case '*':
        case '/':
        case ':':
        case '<':
        case '=':
        case '>':
        case '?':
        case '^':
        case '_':
        case '~':
            ungetc(cur, source);
            token_list = add_token(token_list, lex_identifier(source));
            break;
        case '0' ... '9':
            ungetc(cur, source);
            token_list = add_token(token_list, lex_number(source));
            break;
        default:
            fprintf(stderr, "Unrecognized character: %c", cur);
            exit(1);
        }
    }

    return token_list;
}

char *render_token_kind(enum TokenKind token_kind)
{
    switch (token_kind)
    {
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_PAREN_L:
        return "PAREN_L";
    case TOKEN_PAREN_R:
        return "PAREN_R";
    }
}

char *render_token_lexeme(unsigned char *lexeme)
{
    if (lexeme == NULL)
    {
        return "NULL";
    }
    return (char *)lexeme;
}

void display_token_list(struct TokenList *token_list)
{
    struct Token *token = token_list->head;

    while (token != NULL)
    {
        fprintf(stdout,
                "<%s: %s>\n",
                render_token_kind(token->kind),
                render_token_lexeme(token->lexeme));
        token = token->next;
    }
}
