#ifndef CHECK_LEXER_H
#define CHECK_LEXER_H

#include "../src/lexer.h"

#define MOCK_TOKEN(k, l)                                                       \
    {                                                                          \
        k, (unsigned char *)l, { NULL }                                        \
    }

void ck_token_eq(struct Token *, struct Token *);

#endif
