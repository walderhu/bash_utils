#ifndef S21_CAT_H
#define S21_CAT_H
#include <stdio.h>
#include <stdlib.h>
typedef struct config {
  unsigned short b, e, n, s, t, v;
} config;
static inline int parser(config *flag, int argc, char **argv);
static inline int s21_strcmp(const char *str1, const char *str2);
static inline void err(char **argv, int i, int error);
static inline int one_defis_flags_check(config *flag, int i, char **argv);
static inline int two_defis_flags_check(config *flag, int i, char **argv);
static inline void reading_file(int argc, char *argv[], config *flg);
#endif