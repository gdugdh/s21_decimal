#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) return EXPR_ERROR;

  s21_assign(&value, result);
  setSign(result->bits, !getSign(value.bits));
  return OK;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (!result) return EXPR_ERROR;

  s21_assign(&value, result);
  int exp = d_exp(result);
  if (exp > 28) return EXPR_ERROR;

  int R = 0;
  while (exp--) s21_idiv10(result, &R);

  result->bits[3] &= 1 << 31;  // зануляю все биты кроме бита знака
  return OK;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result) return EXPR_ERROR;

  int status = s21_truncate(value, result);

  if (status == OK && d_sign(&value) && s21_is_not_equal(value, *result)) {
    // Если число отрицательное и после округления к нулю стало меньше, то
    // нужно отнять единицу, чтобы получился эффект округления к минус
    // бесконечности
    s21_decimal one = {0};
    one.bits[0] = 1;
    s21_sub(*result, one, result);
  }

  return status;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  if (!result) return EXPR_ERROR;

  s21_assign(&value, result);
  int exp = d_exp(result);
  if (exp > 28) return EXPR_ERROR;

  int R = 0;
  int prev_part_not_zero = 0;
  while (exp--) {
    s21_idiv10(result, &R);
    if (exp) prev_part_not_zero = prev_part_not_zero || R;
  }

  result->bits[3] &= 1 << 31;  // зануляю все биты кроме бита знака

  int addition = R > 5;
  if (R == 5) {
    if (prev_part_not_zero)
      addition = 1;
    else {
      int tmp;
      s21_imod10(result, &tmp);
      addition = tmp % 2;
    }
  }

  if (addition) {  // increment
    s21_decimal one = {0};
    one.bits[0] = 1;
    if (d_sign(result))
      s21_sub(*result, one,
              result);  // Используется вычитание для увеличение
                        // абсолютный величины отрицательного числа
    else
      s21_add(*result, one, result);
  }

  return OK;
}
