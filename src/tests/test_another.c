#include <check.h>
#include <time.h>

#include "./../s21_decimal.h"

START_TEST(s21_negate_test) {
  s21_decimal value1 = {0};
  s21_decimal result1 = {0};
  setSign(result1.bits, 1);
  s21_negate(value1, &result1);

  s21_decimal value2 = {0};
  s21_decimal result2 = {0};
  setSign(result2.bits, 0);
  s21_negate(value2, &result2);

  ck_assert_int_ne(getSign(value1.bits), getSign(result1.bits));
  ck_assert_int_ne(getSign(value2.bits), getSign(result2.bits));
}
END_TEST

START_TEST(s21_truncate_test) {
  s21_decimal value1 = {0};
  s21_decimal result1 = {0};
  s21_decimal expected = {0};
  str_to_decimal("79228162514264.337593543950335", &value1);
  s21_truncate(value1, &result1);
  str_to_decimal("79228162514264", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result1.bits[i], expected.bits[i]);
  }

  s21_decimal value2 = {0};
  s21_decimal result2 = {0};
  s21_decimal expected2 = {0};
  str_to_decimal("-79228162514264.337593543950335", &value2);
  s21_truncate(value2, &result2);
  str_to_decimal("-79228162514264", &expected2);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result2.bits[i], expected2.bits[i]);
  }
}
END_TEST

START_TEST(s21_floor_test) {
  s21_decimal value1 = {0};
  s21_decimal result1 = {0};
  s21_decimal expected = {0};
  str_to_decimal("-10.00000004", &value1);
  s21_floor(value1, &result1);
  str_to_decimal("-11", &expected);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result1.bits[i], expected.bits[i]);
  }

  s21_decimal value2 = {0};
  s21_decimal result2 = {0};
  s21_decimal expected2 = {0};
  str_to_decimal("10.00000004", &value2);
  s21_floor(value2, &result2);
  str_to_decimal("10", &expected2);

  for (int i = 0; i < 4; ++i) {
    ck_assert_int_eq(result2.bits[i], expected2.bits[i]);
  }
}
END_TEST

START_TEST(s21_round_test) {
  char from_nums[16][100] = {
      "10",  "10.4",  "10.5",  "10.6",  "11",  "11.4",  "11.5",  "11.6",
      "-10", "-10.4", "-10.5", "-10.6", "-11", "-11.4", "-11.5", "-11.6"};

  char result[16][100] = {"10",  "10",  "10",  "11",  "11",  "11",
                          "12",  "12",  "-10", "-10", "-10", "-11",
                          "-11", "-11", "-12", "-12"};

  for (int i = 0; i < 16; ++i) {
    s21_decimal num = {0};
    s21_decimal res = {0};
    s21_decimal expected = {0};

    str_to_decimal(from_nums[i], &num);
    s21_round(num, &res);

    str_to_decimal(result[i], &expected);

    for (int i = 0; i < 4; ++i) {
      ck_assert_int_eq(res.bits[i], expected.bits[i]);
    }
  }
}
END_TEST

int main() {
  Suite *suite = suite_create("TEST ANOTHER");
  TCase *test_case = tcase_create("My Test Case");
  tcase_add_test(test_case, s21_negate_test);
  tcase_add_test(test_case, s21_truncate_test);
  tcase_add_test(test_case, s21_floor_test);
  tcase_add_test(test_case, s21_round_test);

  suite_add_tcase(suite, test_case);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);

  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed == 0 ? 0 : 1;
}
