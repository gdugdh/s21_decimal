#include "./../s21_decimal.h"

void s21_print(s21_decimal* val_print) {
  s21_decimal val = {0};
  s21_assign(val_print, &val);

  int exp = d_exp(&val);

  if (exp > 28) {
    // putc(d_sign(&val) ? '-' : '+', stdout);
    // puts("inf");
    puts("s21_decimal error: exp > 28");
    return;
  }

  char buffer[100] = {'\0'};
  int R = 0;
  int i = 0;

  if (!exp) buffer[i++] = '0';

  while (exp--) {
    s21_idiv10(&val, &R);
    buffer[i++] = R + '0';
  }
  buffer[i++] = '.';

  if (!s21_neq_zero(&val)) buffer[i++] = '0';

  while (s21_neq_zero(&val)) {
    s21_idiv10(&val, &R);
    buffer[i++] = R + '0';
  }

  if (d_sign(&val)) putc('-', stdout);
  while (i) putc(buffer[--i], stdout);
  putc('\n', stdout);
}

int mult_test() {
  s21_decimal num = {0};
  s21_decimal num2 = {0};
  s21_decimal res = {0};
  str_to_decimal("1", &num);
  str_to_decimal("0.0000000000000000000000000001", &num2);
  printf("\n\n\nMULT RESULT: %d\n", s21_mul(num, num2, &res));

  printf("exp: %2d | ", d_exp(&num));
  s21_print(&num);
  printf("exp: %2d | ", d_exp(&num2));
  s21_print(&num2);
  printf("exp: %2d | ", d_exp(&res));
  s21_print(&res);

  str_to_decimal("11111", &num);
  str_to_decimal("-3", &num2);
  printf("\n\n\nMULT RESULT: %d\n", s21_mul(num, num2, &res));

  printf("exp: %2d | ", d_exp(&num));
  s21_print(&num);
  printf("exp: %2d | ", d_exp(&num2));
  s21_print(&num2);
  printf("exp: %2d | ", d_exp(&res));
  s21_print(&res);
  return 0;
}

int div_test() {
  s21_decimal num = {0};
  s21_decimal num2 = {0};
  s21_decimal res = {0};
  str_to_decimal("-123.21", &num);
  str_to_decimal("111", &num2);
  printf("div result: %d\n", s21_div(num, num2, &res));
  s21_print(&num);
  s21_print(&num2);
  s21_print(&res);

  // str_to_decimal("1.23", &num);
  // str_to_decimal("1.23", &num2);
  // printf("div result: %d\n", s21_div(num, num2, &res))    ;
  // s21_print(&num);
  // s21_print(&num2);
  // s21_print(&res);
  return 0;
}

int sub_test() {
  s21_decimal num = {0};
  s21_decimal num2 = {0};
  s21_decimal res = {0};
  str_to_decimal("79228162514264337593543950335", &num);
  str_to_decimal("0.6", &num2);
  printf("sub result: %d\n", s21_sub(num, num2, &res));
  s21_print(&num);
  s21_print(&num2);
  s21_print(&res);

  return 0;
}

int add_test() {
  s21_decimal src1, src2, origin, result = {0};
  int value_type_result, value_type_origin;

  src1.bits[0] = 0b11111111111111111111111111110110;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000001001;
  src1.bits[3] = 0b00000000000000010000000000000000;

  src2.bits[0] = 0b00111001100010111010010101100010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  value_type_result = s21_add(src1, src2, &result);
  value_type_origin = 0;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;

  s21_print(&src1);
  s21_print(&src2);
  printf("\tresult and origin:\n");
  s21_print(&result);
  s21_print(&origin);

  printf("add result: %d\n", value_type_result);
  printf("origin result: %d\n", value_type_origin);

  return 0;
}

int comparision_test() {
  s21_decimal num = {0};
  s21_decimal num2 = {0};

  str_to_decimal("0", &num);
  str_to_decimal("-3", &num2);

  s21_print(&num);
  s21_print(&num2);

  printf("\n");

  printf("== %d\n", s21_is_equal(num, num2));
  printf("!= %d\n", s21_is_not_equal(num, num2));
  printf("<  %d\n", s21_is_less(num, num2));
  printf(">  %d\n", s21_is_greater(num, num2));
  printf("<= %d\n", s21_is_less_or_equal(num, num2));
  printf(">= %d\n", s21_is_greater_or_equal(num, num2));

  return 0;
}

int truncate_test() {
  s21_decimal num = {0};
  s21_decimal res = {0};
  str_to_decimal("79228162514264.337593543950335", &num);

  printf("truncate result: %d\n", s21_truncate(num, &res));
  s21_print(&num);
  s21_print(&res);
  return 0;
}

int floor_test() {
  s21_decimal num = {0};
  s21_decimal res = {0};
  str_to_decimal("-10.00000004", &num);

  printf("floor result: %d\n", s21_floor(num, &res));
  s21_print(&num);
  s21_print(&res);
  return 0;
}

int round_test() {
  char from_nums[16][100] = {
      "10",  "10.4",  "10.5",  "10.6",  "11",  "11.4",  "11.5",  "11.6",
      "-10", "-10.4", "-10.5", "-10.6", "-11", "-11.4", "-11.5", "-11.6"};

  char result[16][100] = {"10",  "10",  "10",  "11",  "11",  "11",
                          "12",  "12",  "-10", "-10", "-10", "-11",
                          "-11", "-11", "-12", "-12"};

  for (int i = 0; i < 16; ++i) {
    s21_decimal num = {0};
    s21_decimal res = {0};
    s21_decimal correct_res = {0};
    str_to_decimal(from_nums[i], &num);
    str_to_decimal(result[i], &correct_res);

    s21_round(num, &res);
    printf("input value: ");
    s21_print(&num);
    printf("func res:    ");
    s21_print(&res);
    printf("correct res: ");
    s21_print(&correct_res);
    printf("check:       %c\n\n", s21_is_equal(correct_res, res) ? 'v' : 'x');
  }
  return 0;
}

int convertors_test() {
  int from_int_to_decimal_test() {
    int i = 41241719;
    s21_decimal d = {0};
    printf("from_int_to_decimal result: %d\n", s21_from_int_to_decimal(i, &d));
    printf("int (from):   %d\ndecimal (to): ", i);
    s21_print(&d);
    return 0;
  }

  int from_decimal_to_int_test() {
    s21_decimal d = {0};
    d.bits[0] = 123412;
    d.bits[3] = 1 << 31;
    int i = 0;
    printf("from_decimal_to_int result: %d\n", s21_from_decimal_to_int(d, &i));
    printf("decimal (from): ");
    s21_print(&d);
    printf("int (to):       %d\n", i);
    return 0;
  }
  int status = 0;
  status = from_int_to_decimal_test();
  puts("");
  status &= from_decimal_to_int_test();
  return status;
}

int s21_get_fp_exp_test() {
  s21_decimal src = {0};
  int whole = 0, exp = 0;
  s21_decimal frac = {0};
  str_to_decimal("0.25", &src);

  s21_get_fp_exp(src, &exp, &whole, &frac);
  printf("IEE754 exp: %d\n", exp);
  printf("whole: %d\n", whole);
  s21_print(&frac);

  return 0;
}

int s21_from_decimal_to_float_test() {
  s21_decimal src = {0};
  float dst = 0;

  str_to_decimal("213.21124", &src);
  float temp = 213.21124;
  s21_from_decimal_to_float(src, &dst);

  printf("\n\n\nfloat  : %f\n", dst);
  float_representation(&dst);
  printf("float  : %f\n", temp);
  float_representation(&temp);
  puts("\n\n");

  return 0;
}

int main() {
  comparision_test();
  round_test();
  convertors_test();
  mult_test();
  div_test();
  add_test();
  s21_get_fp_exp_test();
  s21_from_decimal_to_float_test();
}
