#include "check_lexer.h"
#include "../src/lexer.h"
#include "../src/utils.h"
#include <check.h>

void ck_token_eq(struct Token *token_a, struct Token *token_b) {
    ck_assert_int_eq(token_a->kind, token_b->kind);
    ck_assert_pstr_eq((const char *)token_a->lexeme,
                      (const char *)token_b->lexeme);
}

START_TEST(test_lex_simple_expression) {
    char expr[] = "(+ 1 1)";
    struct Token expected_tokens[] = {MOCK_TOKEN(TOKEN_PAREN_L, NULL),
                                      MOCK_TOKEN(TOKEN_IDENTIFIER, "+"),
                                      MOCK_TOKEN(TOKEN_NUMBER, "1"),
                                      MOCK_TOKEN(TOKEN_NUMBER, "1"),
                                      MOCK_TOKEN(TOKEN_PAREN_R, NULL)};

    FILE *fd = fmemopen(expr, strlen(expr), "r");
    struct TokenList *token_list = lex(fd);

    struct Token *actual_token;
    unsigned int i = 0;
    STAILQ_FOREACH(actual_token, token_list, entries) {
        ck_token_eq(actual_token, &expected_tokens[i]);
        i += 1;
    }

    free_token_list(token_list);
    fclose(fd);
}
END_TEST

Suite *lexer_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Lexer");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_lex_simple_expression);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
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
