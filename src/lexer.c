#include <stdbool.h>

#include "lexer.h"
#include "utils.h"

inline struct Token *create_token(enum TokenKind kind, unsigned char *lexeme) {
    struct Token *token;
    MALLOC(token, sizeof(struct Token));

    token->kind = kind;
    token->lexeme = lexeme;

    return token;
}

void free_token(struct Token *token) {
    free(token->lexeme);
    free(token);
}

char *render_token_list(struct TokenList *token_list) {
    char *rendered_tokens = "";
    struct Token *token;

    STAILQ_FOREACH(token, token_list, entries) {
        switch (token->kind) {
            case TOKEN_IDENTIFIER:
                asprintf(&rendered_tokens,
                         "%sIDENTIFIER %s\n",
                         rendered_tokens,
                         token->lexeme);
                break;
            case TOKEN_NUMBER:
                asprintf(&rendered_tokens,
                         "%sNUMBER %s\n",
                         rendered_tokens,
                         token->lexeme);
                break;
            case TOKEN_PAREN_L:
                asprintf(&rendered_tokens, "%sPAREN-L\n", rendered_tokens);
                break;
            case TOKEN_PAREN_R:
                asprintf(&rendered_tokens, "%sPAREN-R\n", rendered_tokens);
                break;
        }
    }

    return rendered_tokens;
}

void free_token_list(struct TokenList *token_list) {
    struct Token *token, *token_;
    STAILQ_FOREACH_SAFE(token, token_list, entries, token_) {
        STAILQ_REMOVE(token_list, token, Token, entries);
        free_token(token);
    }
    free(token_list);
}

struct Token *lex_identifier(FILE *source) {
    int cur_;
    unsigned char cur;
    bool done = false;
    unsigned char *identifier = NULL;
    size_t size = 0;

    while (!done && ((cur_ = fgetc(source)) != EOF)) {
        cur = (unsigned char)cur_;

        switch (cur) {
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

    return create_token(TOKEN_IDENTIFIER, identifier);
}

struct Token *lex_number(FILE *source) {
    int cur_;
    unsigned char cur;
    bool done = false;
    unsigned char *digits = NULL;
    size_t size = 0;

    while (!done && ((cur_ = fgetc(source)) != EOF)) {
        cur = (unsigned char)cur_;

        switch (cur) {
            case '0' ... '9':
                if (size == 1 && digits[0] == '0') {
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

    return create_token(TOKEN_NUMBER, digits);
}

struct TokenList *lex(FILE *source) {
    int cur_;
    unsigned char cur;
    struct TokenList *token_list;

    MALLOC(token_list, sizeof(struct TokenList));
    STAILQ_INIT(token_list);

    while ((cur_ = fgetc(source)) != EOF) {
        cur = (unsigned char)cur_;

        switch (cur) {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break;
            case '(': {
                struct Token *token = create_token(TOKEN_PAREN_L, NULL);
                STAILQ_INSERT_TAIL(token_list, token, entries);
            } break;
            case ')': {
                struct Token *token = create_token(TOKEN_PAREN_R, NULL);
                STAILQ_INSERT_TAIL(token_list, token, entries);
            } break;
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
            case '~': {
                ungetc(cur, source);
                struct Token *token = lex_identifier(source);
                STAILQ_INSERT_TAIL(token_list, token, entries);
            } break;
            case '0' ... '9': {
                ungetc(cur, source);
                struct Token *token = lex_number(source);
                STAILQ_INSERT_TAIL(token_list, token, entries);
            } break;
            default:
                fprintf(stderr, "Unrecognized character: %c", cur);
                exit(1);
        }
    }

    return token_list;
}
