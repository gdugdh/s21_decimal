#include "s21_decimal.h"

int getBit(int *bits, int pos) { return (bits[pos / 32] >> (pos % 32)) & 1; }

void setBit(int *bits, int pos, int value) {
  value ? (bits[pos / 32] |= (1 << (pos % 32)))
        : (bits[pos / 32] &= ~(1 << (pos % 32)));
}

char getSign(int *bits) { return getBit(bits, 127); }

void setSign(int *bits, char value) { setBit(bits, 127, value); }

// funcs for joellenj_test.c
void s21_set_sign(s21_decimal *value, int sign) {
  setSign(value->bits, (char)sign);
}

void s21_set_degree(s21_decimal *value, int degree) {
  //   int degree_argument[8] = {0};
  //   for (int i = 7; i >= 0; i--) {
  //     degree_argument[i] = degree % 2;
  //     degree /= 2;
  //   }
  //   for (int i = 0, index = 119; i < 8; i++, index--) {
  //     s21_set_bit(value, index, degree_argument[i]);
  //   }
  value->bits[3] &= ~(0xFF << 16);
  value->bits[3] |= (degree & 0xFF) << 16;
}

// int getExp(unsigned int bits[]) {
//     unsigned int exp[1] = {0};
//     for (int i = 0; i < 8; i++) {
//         setBit(exp, i, getBit(bits, i + 16 + 96));
//     }
//     return exp[0];
// }

// void setExp(unsigned int bits[], unsigned char exp) {
//     setNBits(bits, 16 + 96, 8, 0);
//     bits[3] = (bits[3] & ~(1 << 16)) | (exp << 16);
// }

// void outputBits(int *bits, int cnt_bytes) {
//   for (int i = 0; i < cnt_bytes * 8; i++) {
//     if (i % 8 == 0 && i != 0) printf(" ");
//     printf("%d", getBit(bits, i));
//   }
//   printf("\n");
// }

// void revOutputBits(int *bits, int cnt_bytes) {
//   for (int i = cnt_bytes * 8 - 1; i >= 0; i--) {
//     if ((i + 1) % 8 == 0) printf(" ");
//     printf("%d", getBit(bits, i));
//   }
//   printf("\n");
// }

int str_to_decimal(char *str_val, s21_decimal *d_val) {
  size_t len = strlen(str_val);

  s21_double_decimal val = {0};

  for (size_t i = 0; i < len; ++i) {
    char c = str_val[i];
    if (c == '-') {
      setBit(val.bits, 223, 1);
    } else if (c >= '0' && c <= '9') {
      s21_dd_imult10(&val);
      s21_double_decimal digit = {0};
      digit.bits[0] = c - '0';
      s21_dd_iadd(&val, &digit, &val);
    } else if (c == '.') {
      val.bits[6] &= ~(0xFF << 16);
      val.bits[6] |= (len - i - 1) << 16;
    } else
      return 0;
  }

  s21_to_single(&val, d_val);

  return 1;
}

void s21_empty(s21_decimal *val) { memset(val->bits, 0, sizeof(s21_decimal)); }
