#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  s21_empty(dst);
  dst->bits[0] = src < 0 ? -src : src;
  dst->bits[3] = (1 << 31) & src;
  return OK;
}

int s21_can_convert_to_int(s21_decimal src) {
  if (d_sign(&src)) {
    s21_decimal maxNegVal = {0};
    maxNegVal.bits[0] = (1 << 31);
    maxNegVal.bits[3] = (1 << 31);
    return s21_is_greater_or_equal(src, maxNegVal);  // src >= maxNegVal
  } else {
    s21_decimal maxPosVal = {0};
    maxPosVal.bits[0] = (1u << 31) - 1;
    return s21_is_less_or_equal(src, maxPosVal);  // src <= maxPosVal
  }
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (d_exp(&src) > 28)
    return CONV_ERROR;  // Если exp больше максимально возможного, то ошибка
  s21_truncate(src, &src);  // Урезаем дробную часть
  if (!s21_can_convert_to_int(src)) return CONV_ERROR;

  *dst = (d_sign(&src) && s21_neq_zero(&src)) ? -src.bits[0] : src.bits[0];
  return OK;
}

/* Все функции связанные с конвертацией decimal -> float стоит переделать */

// int s21_get_frac(s21_decimal src, s21_decimal *dst) {
//     s21_empty(dst);

//     s21_decimal whole = {0};
//     s21_truncate(src, &whole);

//     return s21_sub(src, whole, dst) != 0 ? CONV_ERROR : OK;
// }

void s21_pow_2(s21_decimal *dst, int exp) {
  s21_empty(dst);
  dst->bits[0] = 1;
  s21_decimal two = {{2, 0, 0, 0}};

  for (; exp > 0; --exp) {
    s21_decimal temp = {0};
    s21_mul(*dst, two, &temp);
    s21_assign(&temp, dst);
  }

  for (; exp < 0; ++exp) {
    s21_decimal temp = {0};
    s21_div(*dst, two, &temp);
    s21_assign(&temp, dst);
  }
}

int s21_get_fp_exp(s21_decimal src, int *exp, int *whole_mantissa,
                   s21_decimal *frac) {
  *exp = 0;
  *whole_mantissa = 0;
  s21_empty(frac);

  if (((src.bits[3] >> 16) & 0xFF) > 28) return CONV_ERROR;

  s21_decimal whole = {0};
  s21_decimal two = {{2, 0, 0, 0}}, one = {{1, 0, 0, 0}};
  s21_truncate(src, &whole);  // whole
  s21_sub(src, whole, frac);  // frac

  // *exp = *exp ? --*exp : *exp;
  // if (*exp) *whole_mantissa &= (1 << *exp) - 1;

  if (s21_eq_zero(&whole) && s21_neq_zero(frac)) {
    // 0 < |src| <= 1
    int sign = getSign(frac->bits);

    s21_decimal pow_2 = {{1, 0, 0, 0}};
    setSign(pow_2.bits, sign);

    for (; ((!sign && s21_is_less(*frac, pow_2)) ||
            (sign && s21_is_greater(*frac, pow_2))) &&
           *exp >= -28;
         --*exp) {
      s21_div(pow_2, two, &pow_2);
    }

    s21_decimal pow2 = {0};
    s21_pow_2(&pow2, -*exp);

    s21_mul(*frac, pow2, frac);
    setSign(one.bits, sign);
    s21_sub(*frac, one, frac);

  } else if (s21_neq_zero(&whole)) {
    for (; s21_neq_zero(&whole); ++*exp) {
      s21_decimal temp = {0}, R = {0};
      s21_idiv(&whole, &two, &temp, &R);
      s21_assign(&temp, &whole);

      *whole_mantissa |= R.bits[0] << *exp;
    }

    *whole_mantissa &= ~(1 << --*exp);  // ВНИМАНИЕ: --*exp
  }

  return OK;
}

void float_representation(float *src) {
  for (int i = 31; i >= 0; --i) {
    printf("%d", (*(int *)src >> i) & 0x1);
  }
  puts("");
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  /*
  src == 0
  */

  if (s21_eq_zero(&src)) {
    *dst = 0.0;
    return OK;
  }

  int sign = getSign(src.bits), exp = 0, mantissa = 0, whole = 0,
      mantissa_bits = 32;
  s21_decimal frac = {0}, sign_one = {{1, 0, 0, sign << 31}},
              two = {{2, 0, 0, 0}};

  if (s21_get_fp_exp(src, &exp, &whole, &frac) == CONV_ERROR) return CONV_ERROR;

  if (exp >= 0) {
    mantissa_bits -= exp < 23 ? exp : 23;
    mantissa = whole << mantissa_bits;
  }

  // if (s21_get_frac(src, &frac) == CONV_ERROR) return CONV_ERROR;

  while (mantissa_bits-- && s21_neq_zero(&frac)) {
    s21_mul(frac, two, &frac);
    if ((!sign && s21_is_greater_or_equal(frac, sign_one)) ||
        (sign && s21_is_less_or_equal(frac, sign_one))) {
      s21_sub(frac, sign_one, &frac);
      mantissa |= 1 << mantissa_bits;
    }
  }

  int discarded = mantissa & 0x1FF;
  mantissa = (int)((unsigned int)mantissa >> 9);

  if (discarded > (1 << 8))
    ++mantissa;
  else if (discarded == (1 << 8) && mantissa % 2)
    ++mantissa;

  exp += (mantissa >> 23) & 0x1;

  int int_dst = (sign << 31) | ((exp + 127) << 23) | (mantissa & 0x7FFFFF);
  *dst = *(float *)&int_dst;

  return OK;
}

//     int sign = getSign(src.bits), exp = 0, mantissa = 0, whole = 0,
//     mantissa_bits = 32; s21_decimal frac = {0}, one = {{1, 0, 0, sign <<
//     31}}, two = {{2, 0, 0, 0}};

//     if (s21_get_fp_exp(src, &exp, &whole) == CONV_ERROR) return CONV_ERROR;
//     mantissa_bits -= exp;
//     if (mantissa_bits > 0) mantissa = whole << mantissa_bits;

//     if (s21_get_frac(src, &frac) == CONV_ERROR) return CONV_ERROR;

//     if mantissa_bits >=
//     while (mantissa_bits-- && s21_neq_zero(&frac)) {
//         s21_decimal temp = {0};
//         s21_mul(frac, two, &temp);
//         s21_assign(&temp, &frac);
//         if ( (!sign && s21_is_greater_or_equal(frac, one)) || (sign &&
//         s21_is_less_or_equal(frac, one)) ) {
//             s21_sub(frac, one, &temp);
//             s21_assign(&temp, &frac);
//             mantissa |= 1 << mantissa_bits;
//         }
//     }

// int assign = 0;
// int i = 8;
// while (i--) {
//     s21_decimal temp = {0};
//     s21_mul(frac, two, &temp);
//     s21_assign(&temp, &frac);
//     if ( (!sign && s21_is_greater_or_equal(frac, one)) || (sign &&
//     s21_is_less_or_equal(frac, one)) ) {
//         s21_sub(frac, one, &temp);
//         s21_assign(&temp, &frac);
//         assign |= 1 << i;
//     }
// }

// if (assign > (1 << 7)) ++mantissa;
// else if (assign == (1 << 7) && mantissa % 2) ++mantissa;

// exp += (mantissa >> 23) & 0x1;

//  @author joellenj@student.21-school.ru
void s21_trailing_zeroes(unsigned int *str_num, int *degree) {
  // Уменьшает показатель степени на количество младших подряд идущих
  // десятичных разядов выставленных в 0
  int flag = 1;
  while (flag) {
    if (*str_num % 10 == 0) {
      *str_num /= 10;
      *degree -= 1;
    } else
      flag = 0;
  }
}

int s21_parser_string(char *str_num, s21_decimal *dst) {
  // Строковую экспоненциальную запись конвертирует в s21_decimal
  int error = 0, sign_dec = 0, degree = 0, degree_sign = 0, str_index = 0,
      int_index = 0;
  char degree_E[100] = {0}, str_int_num[100] = {0};
  if (str_num[0] == '-') {
    // Если первый символ знак, то пропускаем его
    sign_dec = 1;
    str_index++;
  }

  for (; str_num[str_index] != '\0'; str_index++) {
    // Проходим строку экспоненциальной записи (Пример: 1.23e+2)
    if (str_num[str_index] == 'e') {
      //Собираем последовательность цифр ответственных за экспоненту
      // Проверяем знак и пропускаем символ
      if (str_num[++str_index] == '-') degree_sign = 1;
      str_index++;
      // Создаем вложенный цикл для того, чтобы пробежаться до конца
      // строки и собрать все цифры в строку
      for (int i = 0; str_num[str_index] != '\0'; str_index++) {
        degree_E[i] = str_num[str_index];
        degree_E[++i] = '\0';
      }
    } else if (str_num[str_index] != '.') {
      // Собираем последовательность цифр идущих до 'e'
      // (из примера: 123)
      str_int_num[int_index] = str_num[str_index];
      str_int_num[++int_index] = '\0';
    }
  }
  int degree_E_int = atoi(degree_E);

  if (degree_E_int < 23) {
    if (degree_sign)
      degree = 6 + degree_E_int;
    else if (!degree_sign)
      degree = 6 - degree_E_int;

    unsigned int int_num = atoi(str_int_num);
    if (degree > 0) s21_trailing_zeroes(&int_num, &degree);

    if (degree < 29) {
      s21_decimal dec_ten = {{10, 0, 0, 0}};
      dst->bits[0] = int_num;

      if (!degree_sign) {
        for (; degree < 0 && !error; degree++) {
          error = s21_mul(*dst, dec_ten, dst);
        }
        if (error) error = 1;
      }

      s21_set_degree(dst, degree);
      setSign(dst->bits, sign_dec);
    } else
      error = 1;
  } else
    error = 1;
  return error;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int code = OK;
  if (dst != NULL) {
    s21_empty(dst);
    if (!isnan(src) && !isinf(src)) {
      if (src != 0) {
        char str_num[100] = {0};
        sprintf(str_num, "%e", src);
        code = s21_parser_string(str_num, dst);
        if (code) s21_empty(dst);
      }
    } else
      code = CONV_ERROR;
  } else
    code = CONV_ERROR;
  return code;
}
