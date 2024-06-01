#include "s21_decimal.h"

/* =================================== DECIMAL
 * =================================== */
void s21_assign(s21_decimal* fromval, s21_decimal* toval) {
  memcpy(toval, fromval, sizeof(s21_decimal));
}

// ARITHMETIC

void s21_idiv10(s21_decimal* N, int* R) {
  // N / D = Q (ост R)
  // Используется метод деления столбиком
  int D = 10;
  s21_decimal Q = {0};
  // int R = 0;
  *R = 0;

  for (int i = sizeof(int) * 8 * 3; i >= 0; --i) {
    *R <<= 1;
    setBit(R, 0, getBit(N->bits, i));
    if (*R >= D) {
      *R -= D;
      setBit(Q.bits, i, 1);
    }
    // printf("\nR = %d\n", R);
    // outputBits(Q.bits, 3 * sizeof(int));
  }

  memcpy(N->bits, Q.bits, sizeof(int) * 3);
}

void s21_imod10(s21_decimal* val, int* R) {
  s21_decimal tmp = {0};
  s21_assign(val, &tmp);
  s21_idiv10(&tmp, R);
}

// PRINT
// void s21_print(s21_decimal* val_print) {
//   s21_decimal val = {0};
//   s21_assign(val_print, &val);

//   int exp = d_exp(&val);

//   if (exp > 28) {
//     // putc(d_sign(&val) ? '-' : '+', stdout);
//     // puts("inf");
//     puts("s21_decimal error: exp > 28");
//     return;
//   }

//   char buffer[100] = {'\0'};
//   int R = 0;
//   int i = 0;

//   if (!exp) buffer[i++] = '0';

//   while (exp--) {
//     s21_idiv10(&val, &R);
//     buffer[i++] = R + '0';
//   }
//   buffer[i++] = '.';

//   if (!s21_neq_zero(&val)) buffer[i++] = '0';

//   while (s21_neq_zero(&val)) {
//     s21_idiv10(&val, &R);
//     buffer[i++] = R + '0';
//   }

//   if (d_sign(&val)) putc('-', stdout);
//   while (i) putc(buffer[--i], stdout);
//   putc('\n', stdout);
// }

int s21_idiv(s21_decimal* N, s21_decimal* D, s21_decimal* Q, s21_decimal* R) {
  // Костыль :/

  s21_double_decimal Nd = {0}, Dd = {0}, Qd = {0}, Rd = {0};
  s21_to_double(N, &Nd);
  s21_to_double(D, &Dd);

  int status = s21_dd_idiv(&Nd, &Dd, &Qd, &Rd);

  status = status | s21_to_single(&Qd, Q);
  status = status | s21_to_single(&Rd, R);
  return status;
}

/* =================================== DOUBLE DECIMAL
 * =================================== */
void s21_dd_left_shift(s21_double_decimal* val, int shift,
                       s21_double_decimal* result) {
  if ((unsigned int)shift >= 192u) return;

  // for (int i = 0; i < shift; ++i) setBit(result->bits, i, 0);
  // for (int i = 0; i < 192 - shift; ++i) setBit(result->bits, i + shift,
  // getBit(val->bits, i));

  /*
  shift = 3
  76543 210

  11001 110 // val
  01110 000 // val << 3

  shftP zrs
  01110 000
  shiftP 7...3
  */
  for (int i = 191; i >= shift; --i)
    setBit(result->bits, i, getBit(val->bits, i - shift));
  for (int i = shift - 1; i >= 0; --i) setBit(result->bits, i, 0);
}

void s21_dd_assign(s21_double_decimal* fromval, s21_double_decimal* toval) {
  memcpy(toval, fromval, sizeof(s21_double_decimal));
}

int s21_dd_neq_zero(s21_double_decimal* val) {
  return val->bits[0] || val->bits[1] || val->bits[2] || val->bits[3] ||
         val->bits[4] || val->bits[5];
}

// ARITHMETIC
// INTEGER OPERATOR
void s21_dd_iadd(s21_double_decimal* val1, s21_double_decimal* val2,
                 s21_double_decimal* result) {
  int overflow = 0;
  int tmp = 0;
  for (int i = 0; i < 6; ++i) {
    tmp = val1->bits[i] + val2->bits[i] + overflow;
    overflow = (unsigned int)tmp < (unsigned int)val1->bits[i] ||
               ((tmp == val1->bits[i] || tmp == val2->bits[i]) && overflow);
    /*
    Здесь есть проблема с определением переполнения:

    Если сложить два числа val1->bits[i] и val2->bits[i], то в результате мы
    можем получить переполнение. Особенность переполнения в том, что число
    (tmp) являющееся результатом переполнения на 1 меньше, чем меньшее из
    двух чисел (unsigned). В следствии чего tmp меньше, любого из операндов
    сложения. Для этого мы проверяем tmp < val1->bits[i], как (unsigned)

    НО!!! tmp - это результат суммирования, не двух, а трех чисел. Так как с
    предыдушего цикла может быть переполнения и к результату сложения
    добавится 1. Если overflow == 1 (с прошлого цикла), то tmp будет равен
    меньшему из { val1->bits[i], val2->bits[i] }
    */
    result->bits[i] = tmp;
  }
}

void s21_dd_iincrement(s21_double_decimal* val) {
  s21_double_decimal add_num = {0};
  add_num.bits[0] = 1;
  s21_dd_iadd(val, &add_num, val);
}

void s21_dd_negation(s21_double_decimal* val) {
  for (int i = 0; i < 6; ++i) val->bits[i] = ~(val->bits[i]);
}

void s21_dd_inegate(s21_double_decimal* val) {
  // Не путать с s21_dd_negate
  s21_dd_negation(val);
  s21_dd_iincrement(val);
}

void s21_dd_isub(s21_double_decimal* val1, s21_double_decimal* val2,
                 s21_double_decimal* result) {
  s21_double_decimal minus_val2 = {0};
  s21_dd_assign(val2, &minus_val2);
  s21_dd_inegate(&minus_val2);
  s21_dd_iadd(val1, &minus_val2, result);
}

void s21_dd_imult(s21_double_decimal* val1, s21_double_decimal* val2,
                  s21_double_decimal* result) {
  for (int i = 0; i < 192; ++i) {
    if (getBit(val2->bits, i)) {
      s21_double_decimal shift_val2 = {0};
      s21_dd_left_shift(val1, i, &shift_val2);
      s21_dd_iadd(result, &shift_val2, result);
    }
  }
}

void s21_dd_imult10(s21_double_decimal* val) {
  s21_double_decimal val2 = {0};
  val2.bits[0] = 10;
  s21_double_decimal result = {0};
  s21_dd_imult(val, &val2, &result);
  memcpy(val->bits, result.bits, sizeof(int) * 6);
}

int s21_dd_idiv(s21_double_decimal* N, s21_double_decimal* D,
                s21_double_decimal* Q, s21_double_decimal* R) {
  // N / D = Q (ост R)
  // Используется метод деления столбиком
  // s21_double_decimal Q = {0};
  // *R = 0;
  if (!s21_dd_neq_zero(D)) return DIVISION_BY_ZERO;  // Деление на ноль

  for (int i = sizeof(int) * 8 * 6; i >= 0; --i) {  // i = 192 (count bit's)
    s21_dd_left_shift(R, 1, R);
    setBit(R->bits, 0, getBit(N->bits, i));
    if (!s21_dd_is_iless(R, D)) {  // if (*R >= D) { //
                                   // s21_dd_greates((s21_dd)R, (s21_dd)D);
      s21_dd_isub(R, D, R);
      setBit(Q->bits, i, 1);
    }
  }

  return OK;
}

void s21_dd_idiv10(s21_double_decimal* N, int* iR) {
  // N / D = Q (ост R)
  // Используется метод деления столбиком
  s21_double_decimal D = {0};
  D.bits[0] = 10;
  s21_double_decimal Q = {0};
  s21_double_decimal R = {0};

  s21_dd_idiv(N, &D, &Q, &R);

  *iR = R.bits[0];

  memcpy(N->bits, Q.bits, sizeof(int) * 6);
}

// void s21_dd_idiv10(s21_double_decimal* N, int* R) {
//     printf("1\n");
//     // N / D = Q (ост R)
//     // Используется метод деления столбиком
//     int D = 10;
//     s21_double_decimal Q = {0};
//     *R = 0;

//     for (int i = sizeof(int) * 8 * 6; i >= 0; --i) {  // i = 192 (count
//     bit's)
//         *R <<= 1;  // s21_double_left_shift(R, 1)
//         setBit(R, 0, getBit(N->bits, i));
//         if (*R >= D) { // s21_dd_greates((s21_dd)R, (s21_dd)D);
//             *R -= D;
//             setBit(Q.bits, i, 1);
//         }
//     }

//     memcpy(N->bits, Q.bits, sizeof(int) * 6);
// }

void s21_dd_imod10(s21_double_decimal* val, int* R) {
  s21_double_decimal tmp = {0};
  s21_dd_assign(val, &tmp);
  s21_dd_idiv10(&tmp, R);
}

int s21_dd_is_pow10(s21_double_decimal* val) {
  // Функция, которая проверяет, является ли число в val степенью 10.
  s21_double_decimal tmp = {0};
  s21_dd_assign(val, &tmp);

  // Делим до того момента пока не встречаем цифру отличную от 0
  int R = 0;
  while (s21_dd_neq_zero(&tmp) && !R) {
    s21_dd_idiv10(&tmp, &R);
  }

  // Если последней цифрой, которую мы встретили была 1 и эта цифра является
  // старшим разрядом, то val = 10^n;
  return R == 1 && !s21_dd_neq_zero(&tmp);
}

// PRINT
// void s21_dd_print(s21_double_decimal* val_print) {
//   s21_double_decimal val = {0};
//   s21_dd_assign(val_print, &val);

//   int exp = dd_exp(&val);

//   // if (exp > 28) {
//   //     putc(dd_sign(&val) ? '-' : '+', stdout);
//   //     puts("inf");
//   //     return;
//   // }

//   char buffer[100] = {'\0'};
//   int R = 0;
//   int i = 0;

//   if (!exp) buffer[i++] = '0';

//   while (exp--) {
//     s21_dd_idiv10(&val, &R);
//     buffer[i++] = R + '0';
//   }
//   buffer[i++] = '.';

//   if (!s21_dd_neq_zero(&val)) buffer[i++] = '0';

//   while (s21_dd_neq_zero(&val)) {
//     s21_dd_idiv10(&val, &R);
//     buffer[i++] = R + '0';
//   }

//   if (dd_sign(&val)) putc('-', stdout);
//   while (i) putc(buffer[--i], stdout);
//   putc('\n', stdout);
// }

/* =================================== ПРЕОБРАЗОВАНИЯ
 * =================================== */
// int s21_dd_normalize_by_exp(s21_double_decimal* val, int exp) {
int s21_dd_exp_up(s21_double_decimal* val, int exp) {
  int cur_exp = dd_exp(val);
  if (cur_exp > exp || exp > 0xFF) return 1;

  int shift = exp - cur_exp;
  while (shift--) s21_dd_imult10(val);

  val->bits[6] &= ~(0xFF << 16);
  val->bits[6] |= exp << 16;
  return 0;
}

int s21_dd_reduce_to_one_exp(s21_double_decimal* val1,
                             s21_double_decimal* val2) {
  int exp1 = dd_exp(val1);
  int exp2 = dd_exp(val2);
  if (exp1 > exp2)
    s21_dd_exp_up(val2, exp1);
  else
    s21_dd_exp_up(val1, exp2);
  return 0;
}

void s21_dd_reduction_exp(s21_double_decimal* val) {
  /*
      Убирает лишние нули в дробной части, уменьшая exp
      s21_dd_reduction_exp(123.123000) -> 123.123
  */

  if (s21_dd_neq_zero(val)) {
    int exp = dd_exp(val);
    int R = 0;
    s21_dd_imod10(val, &R);
    while (!R && exp) {
      s21_dd_idiv10(val, &R);
      s21_dd_imod10(val, &R);
      --exp;
    }

    val->bits[6] &= ~(0xFF << 16);
    val->bits[6] |= exp << 16;
  }
}

int s21_to_double(s21_decimal* fromval, s21_double_decimal* toval) {
  memset(toval->bits, 0, sizeof(s21_double_decimal));
  memcpy(toval->bits, fromval->bits, sizeof(int) * 3);
  toval->bits[6] = fromval->bits[3];
  return 0;
}

int s21_to_single(s21_double_decimal* fromval, s21_decimal* toval) {
  /*
  Присваивает значение из fromval в toval.
  Так как fromval имеет тип s21_double_decimal, когда toval имеет тип
  s21_decimal, может произойти такое, что значение из fromval, будет слишком
  велико для того, чтобы представить его в s21_decimal. В таком случае
  s21_to_single возвращает значение 1 (сигнализирует о положительном
  переполнение). В таком случае s21_to_single возвращает значение 1
  (сигнализирует о негативном переполнение). Иначе s21_to_single() -> 0
  (сигнализирует об успешном преобразовании).
  */

  s21_double_decimal from_val = {0};
  s21_dd_assign(fromval, &from_val);
  s21_dd_reduction_exp(&from_val);

  int exp = dd_exp(&from_val);

  int R = 0;
  int prev_part_not_zero = 0;
  while (from_val.bits[3] || from_val.bits[4] || from_val.bits[5]) {
    s21_dd_idiv10(&from_val, &R);
    --exp;
    if (from_val.bits[3] || from_val.bits[4] || from_val.bits[5])
      prev_part_not_zero = prev_part_not_zero || R;
  }

  int addition = R > 5;
  if (R == 5) {
    if (prev_part_not_zero)
      addition = 1;
    else {
      int tmp;
      s21_dd_imod10(&from_val, &tmp);
      addition = tmp % 2;
    }
  }
  if (addition) {  // все шик, так как это unsigned increment, в следствии
                   // чего увеличивается абсолютное значение числа
    s21_dd_iincrement(&from_val);
    exp += s21_dd_is_pow10(&from_val);
  }

  memcpy(toval->bits, from_val.bits, sizeof(int) * 3);

  // change EXP & SIGN
  int signBit = getBit(from_val.bits, 223) << 31;
  if (s21_eq_zero(toval)) signBit = 0;

  toval->bits[3] = 0;
  toval->bits[3] = (signBit) | ((exp & 0xFF) << 16);
  // toval->bits[3] &= ~(0xFF << 16);
  // toval->bits[3] |= (exp & 0xFF) << 16;

  int status = (exp & 0xFF) > 28;
  if (status && d_sign(toval)) status = 2;

  if (status || s21_eq_zero(toval)) s21_empty(toval);
  return status;
}

/* =================================== ФУНКЦИИ ИЗ ЗАДАНИЯ и их DOUBLE версии
 * =================================== */
void s21_dd_add(s21_double_decimal* val1, s21_double_decimal* val2,
                s21_double_decimal* result) {
  s21_dd_reduce_to_one_exp(val1, val2);

  // s21_dd_iadd(val1, val2, result);

  // result->bits[6] &= ~(0xFF << 16);
  // result->bits[6] |= dd_exp(val1) << 16;

  int sign1 = dd_sign(val1);
  int sign2 = dd_sign(val2);

  if (sign1 == sign2) {
    s21_dd_iadd(val1, val2, result);
    result->bits[6] = val1->bits[6];
  } else {
    setBit(val2->bits, 223, !sign2);  // reverse sign bit
    s21_dd_sub(val1, val2, result);
    setBit(val2->bits, 223, !sign2);  // reverse sign bit
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = result == NULL;

  if (!status) {
    s21_empty(result);
    s21_double_decimal val1 = {0};
    s21_double_decimal val2 = {0};
    s21_double_decimal res = {0};
    s21_to_double(&value_1, &val1);
    s21_to_double(&value_2, &val2);

    s21_dd_add(&val1, &val2, &res);
    status = s21_to_single(&res, result);

    if (status) s21_empty(result);
  }

  return status;
}

void s21_dd_sub(s21_double_decimal* val1, s21_double_decimal* val2,
                s21_double_decimal* result) {
  s21_dd_reduce_to_one_exp(val1, val2);

  int sign1 = dd_sign(val1);
  int sign2 = dd_sign(val2);

  if (sign1 == sign2) {
    // -val <=> ~val + 1
    s21_dd_isub(val1, val2, result);
    // s21_dd_negation(&minus_val2);
    // s21_dd_iincrement(&minus_val2);
    // s21_dd_iadd(val1, &minus_val2, result);

    result->bits[6] = val1->bits[6];

    /*
    val1 <  val2 => !sign
    val1 >= val2 => sign
    */

    if (s21_dd_is_iless(val1, val2)) {
      s21_dd_inegate(result);
      setBit(result->bits, 223, !sign2);  // reverse sign bit
    }

    if (!s21_dd_neq_zero(result)) setBit(result->bits, 223, 0);
  } else {
    setBit(val2->bits, 223, !sign2);  // reverse sign bit
    s21_dd_add(val1, val2, result);
    setBit(val2->bits, 223, !sign2);  // reverse sign bit
  }
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = result == NULL;

  if (!status) {
    s21_empty(result);
    s21_double_decimal val1 = {0};
    s21_double_decimal val2 = {0};
    s21_double_decimal res = {0};
    s21_to_double(&value_1, &val1);
    s21_to_double(&value_2, &val2);

    s21_dd_sub(&val1, &val2, &res);
    status = s21_to_single(&res, result);

    if (status) s21_empty(result);
  }

  return status;
}

void s21_dd_mult(s21_double_decimal* val1, s21_double_decimal* val2,
                 s21_double_decimal* result) {
  s21_dd_reduce_to_one_exp(val1, val2);

  s21_dd_imult(val1, val2, result);

  // sign
  int sign = dd_sign(val1) ^ dd_sign(val2);
  setBit(result->bits, 223, sign);

  // exp
  int exp = dd_exp(val1) + dd_exp(val2);
  result->bits[6] &= ~(0xFF << 16);
  result->bits[6] |= exp << 16;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status = result == NULL;

  if (!status) {
    s21_empty(result);
    s21_double_decimal val1 = {0};
    s21_double_decimal val2 = {0};
    s21_double_decimal res = {0};
    s21_to_double(&value_1, &val1);
    s21_to_double(&value_2, &val2);

    s21_dd_mult(&val1, &val2, &res);

    status = s21_to_single(&res, result);
    // if (!s21_neq_zero(result) && s21_neq_zero(&value_1)) status = 2;  //
    // МИЛЛИАРД ОШИБОК из-за этой строчки. Даже не помню зачем

    if (status) s21_empty(result);
  }

  return status;
}

int s21_dd_div(s21_double_decimal* val1, s21_double_decimal* val2,
               s21_double_decimal* result) {
  s21_double_decimal R = {0};

  /* Увеличивает val1 на exp(val2)
  Например:
  Если у делителя (val2) exp = 2, в следствии чего делитель на 10^2 меньше, то
  val2 = num2 / (10^2)
  деление соответственно будет val1 / val2 => val1 / (num / (10^2)), можно
  убрать уменьшение делителя, умоножив делимое на 10^2 val1 / (num / (10^2))
  == (val1 * 10^2) / num

  Данный цикл соответственно увеличивает значение делимого в 10^exp(val2)
  */
  for (int expR = dd_exp(val2); expR > 0; --expR) s21_dd_imult10(val1);

  s21_dd_exp_up(val1, 28);

  // s21_dd_idiv(N, D, Q, R);
  // N / D = Q (ост R)
  int status = s21_dd_idiv(val1, val2, result, &R);

  int sign = dd_sign(val1) ^ dd_sign(val2);
  setBit(result->bits, 223, sign);
  result->bits[6] &= ~(0xFF << 16);
  result->bits[6] |= 28 << 16;
  return status;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int status =
      result == NULL;  // if result != NULL then status = 0 else status = 1

  if (s21_eq_zero(&value_2)) status = DIVISION_BY_ZERO;

  if (!status) {
    s21_empty(result);
    if (s21_neq_zero(&value_1)) {
      s21_double_decimal val1 = {0};
      s21_double_decimal val2 = {0};
      s21_double_decimal res = {0};
      s21_to_double(&value_1, &val1);
      s21_to_double(&value_2, &val2);
      // s21_to_double(result, &res);

      s21_dd_div(&val1, &val2, &res);
      status = s21_to_single(&res, result);

      // if (!s21_neq_zero(result) && s21_neq_zero(&value_1) && status !=
      // DIVISION_BY_ZERO) status = 2;

      if (status) s21_empty(result);
    }
  }

  return status;
}
