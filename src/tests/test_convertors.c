#include <check.h>
#include <time.h>

#include "./../s21_decimal.h"

START_TEST(s21_from_int_to_decimal_test) {
  int integer = 41241719;
  s21_decimal d = {0};
  s21_decimal expected = {0};
  s21_from_int_to_decimal(integer, &d);
  expected.bits[0] = 41241719;

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(d.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(s21_from_decimal_to_int_test) {
  s21_decimal d = {0};
  str_to_decimal("-123412", &d);
  int i = 0;
  s21_from_decimal_to_int(d, &i);
  ck_assert_int_eq(i, -123412);
}
END_TEST

int main() {
  Suite *suite = suite_create("TEST CONVERTORS");
  TCase *test_case = tcase_create("My Test Case");
  tcase_add_test(test_case, s21_from_int_to_decimal_test);
  tcase_add_test(test_case, s21_from_decimal_to_int_test);

  suite_add_tcase(suite, test_case);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);

  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}
