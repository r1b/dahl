#include <stdbool.h>

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
            if (size == 1 && digits[0] == '0')
            {
                fprintf(stderr, "Numbers cannot begin with `0`\n");
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

struct TokenList *lex(FILE *source)
{
    int cur_;
    unsigned char cur;
    struct TokenList *token_list;

    MALLOC(token_list, sizeof(struct TokenList));
    STAILQ_INIT(token_list);

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

            STAILQ_INSERT_TAIL(token_list, token, entries);

            break;
        case ')':
            MALLOC(token, sizeof(struct Token));
            token->kind = TOKEN_PAREN_R;
            token->lexeme = NULL;

            STAILQ_INSERT_TAIL(token_list, token, entries);
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
            token = lex_identifier(source);
            STAILQ_INSERT_TAIL(token_list, token, entries);
            break;
        case '0' ... '9':
            ungetc(cur, source);
            token = lex_number(source);
            STAILQ_INSERT_TAIL(token_list, token, entries);
            break;
        default:
            fprintf(stderr, "Unrecognized character: %c", cur);
            exit(1);
        }
    }

    return token_list;
}

void free_token(struct Token *token)
{
    free(token->lexeme);
    free(token);
}

void free_token_list(struct TokenList *token_list)
{
    struct Token *token, *token_;
    STAILQ_FOREACH_SAFE(token, token_list, entries, token_)
    {
        STAILQ_REMOVE(token_list, token, Token, entries);
        free_token(token);
    }
    free(token_list);
}
