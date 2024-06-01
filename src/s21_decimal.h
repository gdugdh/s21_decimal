#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define d_exp(x) (((x)->bits[3] >> 16) & 0xFF)
#define dd_exp(x) (((x)->bits[6] >> 16) & 0xFF)

#define d_sign(x) (((x)->bits[3] >> 31) & 0x1)
#define dd_sign(x) (((x)->bits[6] >> 31) & 0x1)

#define FALSE 0
#define TRUE 1

#define OK 0
#define POS_INFINITY 1
#define LITTLE_OR_NEG_INFINITY 2
#define DIVISION_BY_ZERO 3

// #define OK 0
#define CONV_ERROR 1

// #define OK 0
#define EXPR_ERROR 1

typedef struct {
  int bits[4];  // int * 4
                // 0, 1, 2, 3
} s21_decimal;

typedef struct {
  int bits[7];  // 0-6 num // 7[16-23] exp 7[30] inf_flag??? 7[31] sign
} s21_double_decimal;

/* =================================== ARITHEMTIC
 * =================================== */
void s21_assign(s21_decimal* fromval, s21_decimal* toval);
void s21_idiv10(s21_decimal* N, int* R);
void s21_imod10(s21_decimal* val, int* R);
// void s21_print(s21_decimal* val_print);
int s21_idiv(s21_decimal* N, s21_decimal* D, s21_decimal* Q, s21_decimal* R);

void s21_dd_left_shift(s21_double_decimal* val, int shift,
                       s21_double_decimal* result);
void s21_dd_assign(s21_double_decimal* fromval, s21_double_decimal* toval);
int s21_dd_neq_zero(s21_double_decimal* val);

void s21_dd_iadd(s21_double_decimal* val1, s21_double_decimal* val2,
                 s21_double_decimal* result);

void s21_dd_iincrement(s21_double_decimal* val);
void s21_dd_negation(s21_double_decimal* val);
void s21_dd_inegate(s21_double_decimal* val);

void s21_dd_isub(s21_double_decimal* val1, s21_double_decimal* val2,
                 s21_double_decimal* result);
void s21_dd_imult(s21_double_decimal* val1, s21_double_decimal* val2,
                  s21_double_decimal* result);
void s21_dd_imult10(s21_double_decimal* val);

int s21_dd_idiv(s21_double_decimal* N, s21_double_decimal* D,
                s21_double_decimal* Q, s21_double_decimal* R);
void s21_dd_idiv10(s21_double_decimal* N, int* iR);
void s21_dd_imod10(s21_double_decimal* val, int* R);

int s21_dd_is_pow10(s21_double_decimal* val);

// void s21_dd_print(s21_double_decimal* val_print);

int s21_dd_exp_up(s21_double_decimal* val, int exp);
int s21_dd_reduce_to_one_exp(s21_double_decimal* val1,
                             s21_double_decimal* val2);
void s21_dd_reduction_exp(s21_double_decimal* val);

int s21_to_double(s21_decimal* fromval, s21_double_decimal* toval);
int s21_to_single(s21_double_decimal* fromval, s21_decimal* toval);

// s21_add
void s21_dd_add(s21_double_decimal* val1, s21_double_decimal* val2,
                s21_double_decimal* result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// s21_sub
void s21_dd_sub(s21_double_decimal* val1, s21_double_decimal* val2,
                s21_double_decimal* result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// s21_mul
void s21_dd_mult(s21_double_decimal* val1, s21_double_decimal* val2,
                 s21_double_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// s21_div
int s21_dd_div(s21_double_decimal* val1, s21_double_decimal* val2,
               s21_double_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

/* =================================== COMPARISION
 * =================================== */
int s21_dd_is_iless(s21_double_decimal* val1, s21_double_decimal* val2);
// int s21_is_iless(s21_decimal* val1, s21_decimal* val2);

int s21_neq_zero(s21_decimal* val);
int s21_eq_zero(s21_decimal* val);

int s21_is_less(s21_decimal val1, s21_decimal val2);
int s21_is_less_or_equal(s21_decimal val1, s21_decimal val2);

int s21_is_greater(s21_decimal val1, s21_decimal val2);
int s21_is_greater_or_equal(s21_decimal val1, s21_decimal val2);

int s21_is_equal(s21_decimal val1, s21_decimal val2);
int s21_is_not_equal(s21_decimal val1, s21_decimal val2);

/* =================================== CONVERTORS
 * =================================== */
int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);

int s21_get_fp_exp(s21_decimal src, int* exp, int* whole_mantissa,
                   s21_decimal* frac);
int s21_get_frac(s21_decimal src, s21_decimal* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

int s21_from_float_to_decimal(float src, s21_decimal* dst);

/* =================================== ANOTHER
 * =================================== */
int s21_negate(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_floor(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);

/* =================================== UTILS ===================================
 */
int getBit(int* bits, int pos);
void setBit(int* bits, int pos, int value);

char getSign(int* bits);
void setSign(int* bits, char value);
void s21_set_sign(s21_decimal* value, int sign);
void s21_set_degree(s21_decimal* value, int degree);

void outputBits(int* bits, int cnt_bytes);
void revOutputBits(int* bits, int cnt_bytes);
int str_to_decimal(char* str_val, s21_decimal* d_val);
void s21_empty(s21_decimal* val);

void float_representation(float* src);

#endif
