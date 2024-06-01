#include <check.h>
#include <time.h>

#include "./../s21_decimal.h"

START_TEST(s21_sub_test) {
  s21_decimal num = {0};
  s21_decimal num2 = {0};
  s21_decimal res = {0};
  s21_decimal expected = {0};

  str_to_decimal("-1234", &num);
  str_to_decimal("132", &num2);
  s21_sub(num, num2, &res);

  str_to_decimal("-1366", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("1234", &num);
  str_to_decimal("1235", &num2);
  s21_sub(num, num2, &res);

  str_to_decimal("-1", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("1234", &num);
  str_to_decimal("-1235", &num2);
  s21_sub(num, num2, &res);

  str_to_decimal("2469", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("79228162514264337593543950335", &num);
  str_to_decimal("0.6", &num2);
  s21_sub(num, num2, &res);

  str_to_decimal("79228162514264337593543950334", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("79228162514264337593543950334", &num);
  str_to_decimal("0.5", &num2);
  s21_sub(num, num2, &res);
  str_to_decimal("79228162514264337593543950334", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(s21_add_test) {
  s21_decimal num = {0};
  s21_decimal num2 = {0};
  s21_decimal res = {0};
  s21_decimal expected = {0};

  str_to_decimal("1234", &num);
  str_to_decimal("132", &num2);
  s21_add(num, num2, &res);

  str_to_decimal("1366", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-1234", &num);
  str_to_decimal("1235", &num2);
  s21_add(num, num2, &res);

  str_to_decimal("1", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-1234", &num);
  str_to_decimal("-1235", &num2);
  s21_add(num, num2, &res);

  str_to_decimal("-2469", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-79228162514264337593543950335", &num);
  str_to_decimal("0.6", &num2);
  s21_add(num, num2, &res);

  str_to_decimal("-79228162514264337593543950334", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-79228162514264337593543950331", &num);
  str_to_decimal("-0.5", &num2);
  s21_add(num, num2, &res);

  str_to_decimal("-79228162514264337593543950332", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-79228162514264337593543950333", &num);
  str_to_decimal("0.5", &num2);
  s21_add(num, num2, &res);

  str_to_decimal("-79228162514264337593543950332", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(s21_mul_test) {
  s21_decimal num = {0};
  s21_decimal num2 = {0};
  s21_decimal res = {0};
  s21_decimal expected = {0};
  str_to_decimal("1", &num);
  str_to_decimal("0.0000000000000000000000000001", &num2);
  str_to_decimal("0.0000000000000000000000000001", &expected);
  s21_mul(num, num2, &res);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-11111", &num);
  str_to_decimal("-3", &num2);
  str_to_decimal("33333", &expected);
  s21_mul(num, num2, &res);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-1.11", &num);
  str_to_decimal("111", &num2);
  str_to_decimal("-123.21", &expected);
  s21_mul(num, num2, &res);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }
}
END_TEST

START_TEST(s21_div_test) {
  s21_decimal num = {0};
  s21_decimal num2 = {0};
  s21_decimal res = {0};
  s21_decimal expected = {0};
  str_to_decimal("0.0000000000000000000000000001", &num);
  str_to_decimal("0.0000000000000000000000000001", &num2);
  str_to_decimal("1", &expected);
  s21_div(num, num2, &res);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("33333", &num);
  str_to_decimal("-3", &num2);
  str_to_decimal("-11111", &expected);
  s21_div(num, num2, &res);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("-123.21", &num);
  str_to_decimal("111", &num2);
  str_to_decimal("-1.11", &expected);
  s21_div(num, num2, &res);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }

  str_to_decimal("1000000", &num);
  str_to_decimal("13", &num2);
  str_to_decimal("76923.076923076923076923076923", &expected);
  s21_div(num, num2, &res);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(res.bits[i], expected.bits[i]);
  }
}
END_TEST

int main() {
  Suite *suite = suite_create("TEST ARITHMETIC");
  TCase *test_case = tcase_create("My Test Case");
  tcase_add_test(test_case, s21_sub_test);
  tcase_add_test(test_case, s21_add_test);
  tcase_add_test(test_case, s21_mul_test);
  tcase_add_test(test_case, s21_div_test);

  suite_add_tcase(suite, test_case);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);

  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}
