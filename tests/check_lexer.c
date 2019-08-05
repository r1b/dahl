#include "check_lexer.h"
#include "../src/lexer.h"
#include "../src/utils.h"

START_TEST(test_lex_simple_expression) {
    char expr[] = "(+ 1 1)";
    FILE *fd = fmemopen(expr, strlen(expr), "r");
    struct TokenList *token_list = lex(fd);

    char expected_tokens[] = "PAREN-L\n"
                             "IDENTIFIER +\n"
                             "NUMBER 1\n"
                             "NUMBER 1\n"
                             "PAREN-R\n";
    char *actual_tokens = render_token_list(token_list);

    ck_assert_str_eq(expected_tokens, actual_tokens);

    free_token_list(token_list);
    free(actual_tokens);
    fclose(fd);
}
END_TEST

Suite *make_lexer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("lexer");

    tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_lex_simple_expression);
    suite_add_tcase(s, tc_core);

    return s;
}
