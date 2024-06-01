#include "s21_decimal.h"

int s21_dd_is_iless(s21_double_decimal* val1, s21_double_decimal* val2) {
  /*
  a = 10001
  b = 10110
  */
  for (int i = 191; i >= 0; --i) {
    int bit1 = getBit(val1->bits, i);
    int bit2 = getBit(val2->bits, i);
    if (!bit1 && bit2)
      return 1;
    else if (bit1 && !bit2)
      break;
  }

  return 0;
}

// int s21_is_iless(s21_decimal* val1, s21_decimal* val2) {
//   /*
//   a = 10001
//   b = 10110
//   */
//   for (int i = 95; i >= 0; --i) {
//     int bit1 = getBit(val1->bits, i);
//     int bit2 = getBit(val2->bits, i);
//     if (!bit1 && bit2)
//       return 1;
//     else if (bit1 && !bit2)
//       break;
//   }

//   return 0;
// }

int s21_neq_zero(s21_decimal* val) {
  return val->bits[0] || val->bits[1] || val->bits[2];
}

int s21_eq_zero(s21_decimal* val) { return !s21_neq_zero(val); }

int s21_is_equal(s21_decimal val1, s21_decimal val2) {  // ==
  if (s21_eq_zero(&val1) && s21_eq_zero(&val2)) return 1;
  if (d_sign(&val1) != d_sign(&val2)) return 0;

  s21_double_decimal dd_val1 = {0};
  s21_double_decimal dd_val2 = {0};
  s21_to_double(&val1, &dd_val1);
  s21_to_double(&val2, &dd_val2);
  s21_dd_reduce_to_one_exp(&dd_val1, &dd_val2);

  for (int i = 0; i < 192; ++i) {
    if (getBit(dd_val1.bits, i) != getBit(dd_val2.bits, i)) return 0;
  }

  return 1;
}

int s21_is_not_equal(s21_decimal val1, s21_decimal val2) {  // !=
  return !s21_is_equal(val1, val2);
}

int s21_is_less(s21_decimal val1, s21_decimal val2) {
  if (s21_eq_zero(&val1) && s21_eq_zero(&val2)) return 0;  // val1 == val2 -> 0

  if (d_sign(&val1) && !d_sign(&val2)) return 1;  // -val1 < +val2 -> 1
  if (!d_sign(&val1) && d_sign(&val2)) return 0;  // +val1 < -val2 -> 0

  s21_double_decimal dd_val1 = {0};
  s21_double_decimal dd_val2 = {0};
  s21_to_double(&val1, &dd_val1);
  s21_to_double(&val2, &dd_val2);
  s21_dd_reduce_to_one_exp(&dd_val1, &dd_val2);

  if (d_sign(&val1) == 0) {  // +
    return s21_dd_is_iless(&dd_val1, &dd_val2);
  } else {  // -
    return !s21_dd_is_iless(&dd_val1, &dd_val2);
  }
}

int s21_is_less_or_equal(s21_decimal val1, s21_decimal val2) {  // val1 <= val2
  return s21_is_less(val1, val2) || s21_is_equal(val1, val2);
}

int s21_is_greater(s21_decimal val1, s21_decimal val2) {  // val1 > val2
  return s21_is_less(val2, val1);
}

int s21_is_greater_or_equal(s21_decimal val1,
                            s21_decimal val2) {  // val1 >= val2
  return s21_is_greater(val1, val2) || s21_is_equal(val1, val2);
}
