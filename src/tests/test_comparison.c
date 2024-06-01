#include <check.h>
#include <time.h>

#include "./../s21_decimal.h"

START_TEST(s21_comparision_test) {
  s21_decimal num = {0};
  s21_decimal num2 = {0};

  str_to_decimal("0", &num);
  str_to_decimal("-3", &num2);

  ck_assert_int_eq(s21_is_equal(num, num2), 0);
  ck_assert_int_eq(s21_is_not_equal(num, num2), 1);

  ck_assert_int_eq(s21_is_less(num, num2), 0);
  ck_assert_int_eq(s21_is_less_or_equal(num, num2), 0);

  ck_assert_int_eq(s21_is_greater(num, num2), 1);
  ck_assert_int_eq(s21_is_greater_or_equal(num, num2), 1);
}
END_TEST

int main() {
  Suite *suite = suite_create("TEST COMPARISON");
  TCase *test_case = tcase_create("My Test Case");
  tcase_add_test(test_case, s21_comparision_test);

  suite_add_tcase(suite, test_case);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);

  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}
