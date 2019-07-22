#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "utils.h"

struct Token* lex_identifier(FILE* source) {
    int cur;
    unsigned char* identifier = NULL;
    unsigned long size = 0;
    struct Token* token;

    while((cur = fgetc(source)) != EOF) {
        // TODO: Support <peculiar identifier>
        switch ((unsigned char) cur) {
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
                REALLOC(identifier, ++size * sizeof(unsigned char));
                identifier[size - 1] = (unsigned char)cur;
                continue;
            default:
                ungetc(cur, source);
                break;
        }
    }

    MALLOC(token, sizeof(struct Token));

    token->kind = IDENTIFIER;

    REALLOC(identifier, (size + 1) * sizeof(unsigned char));
    identifier[size] = '\0';
    token->lexeme = identifier;

    return token;

}

struct Token* lex_number(FILE* source) {
    int cur;
    unsigned char* digits = NULL;
    unsigned long size = 0;
    struct Token* token;

    while((cur = fgetc(source)) != EOF) {
        switch ((unsigned char) cur) {
            case '0' ... '9':
                if ((unsigned char)cur == '0' && size == 0) {
                    fprintf(stderr, "Numbers cannot begin with `0`");
                    exit(1);
                }

                REALLOC(digits, ++size * sizeof(unsigned char));
                digits[size - 1] = (unsigned char)cur;
                continue;
            default:
                ungetc(cur, source);
                break;
        }
    }

    MALLOC(token, sizeof(struct Token));

    token->kind = NUMBER;

    REALLOC(digits, (size + 1) * sizeof(unsigned char));
    digits[size] = '\0';
    token->lexeme = digits;

    return token;
}

struct TokenList* add_token(struct TokenList* token_list, struct Token* token) {
    if (token_list->head == NULL) {
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

struct TokenList* lex(FILE* source) {
    int cur;
    struct TokenList* token_list;

    MALLOC(token_list, sizeof(struct TokenList));
    token_list->head = NULL;
    token_list->tail = NULL;

    while((cur = fgetc(source)) != EOF) {
        struct Token* token;

        switch ((unsigned char) cur) {
            // FIXME: Special case \r\n to preserve line info
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                continue;
            case '(':
                MALLOC(token, sizeof(struct Token));
                token->kind = PAREN_L;
                token->lexeme = NULL;
                token_list = add_token(token_list, token);
                continue;
            case ')':
                MALLOC(token, sizeof(struct Token));
                token->kind = PAREN_R;
                token->lexeme = NULL;
                token_list = add_token(token_list, token);
                continue;
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
                continue;
            case '0' ... '9':
                ungetc(cur, source);
                token_list = add_token(token_list, lex_number(source));
                continue;
            default:
                fprintf(stderr, "Unrecognized character: %c", (unsigned char) cur);
                exit(1);
        }
    }

    return token_list;
}

char* render_token_kind(enum TokenKind token_kind) {
    switch (token_kind) {
        case IDENTIFIER:
            return "IDENTIFIER";
        case NUMBER:
            return "NUMBER";
        case PAREN_L:
            return "PAREN_L";
        case PAREN_R:
            return "PAREN_R";
        default:
            fprintf(stderr, "Unrecognized token kind: %d", token_kind);
            exit(1);
    }
}

char* render_token_lexeme(unsigned char* lexeme) {
    if (lexeme == NULL) {
        return "NULL";
    }
    return (char*)lexeme;
}

void display_token_list(struct TokenList* token_list) {
    struct Token* token;
    token = token_list->head;

    while (token != NULL) {
        fprintf(stdout,
                "<%s: %s>\n",
                render_token_kind(token->kind),
                render_token_lexeme(token->lexeme));
        token = token->next;
    }

    fprintf(stdout, "\n");
}

