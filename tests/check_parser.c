#include "check_parser.h"
#include "../src/lexer.h"
#include "../src/parser.h"
#include "check_lexer.h"

/*
FIXME: Not sure how to do a serious test here..
seems like we need to walk the parse tree

 A cute way to do this would be to make an
 expression_repr that prints the parse tree
 and just compare that..
 */

START_TEST(test_parse_simple_expression) {
    char expr[] = "(+ 1 1)";
    FILE *fd = fmemopen(expr, strlen(expr), "r");
    struct TokenList *token_list = lex(fd);
    struct Expression *expression = parse(token_list);
    ck_assert_int_eq(expression->kind, EXPR_PROCEDURE_CALL);
}
END_TEST

Suite *make_parser_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("parser");

    tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_parse_simple_expression);
    suite_add_tcase(s, tc_core);

    return s;
}
