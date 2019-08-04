#include <check.h>
#include "../src/lexer.h"
#include "../src/utils.h"

START_TEST(test_lex_simple_expression)
{
    char expr[] = "(+ 1 1)";
    int expected_kinds[] = {
        TOKEN_PAREN_L,
        TOKEN_IDENTIFIER,
        TOKEN_NUMBER,
        TOKEN_NUMBER,
        TOKEN_PAREN_R,
    };

    FILE *fd = fmemopen(expr, strlen(expr), "r");
    struct TokenList *token_list = lex(fd);

    struct Token *token;
    unsigned int i = 0;
    STAILQ_FOREACH(token, token_list, entries)
    {
        ck_assert_int_eq(token->kind, expected_kinds[i]);
        i += 1;
    }

    free_token_list(token_list);
    fclose(fd);
}
END_TEST

Suite *lexer_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Lexer");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_lex_simple_expression);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = lexer_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
