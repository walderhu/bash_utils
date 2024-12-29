#ifndef S21_GREP
#define S21_GREP
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFER 2048
#define FILE_ERR_MSG "grep: %s: No such file or directory\n"
#define ARG_ERR_MSG "grep: option requires an argument -- %c\n"
#define ONGOING 0
#define ERR 1
typedef struct config {
  int e, i, v, c, l, n, h, s, f, o, pattern;
  int multiple_files;
  int numb_line;
  int numb_word;
  int numb_file;
  char file_pattern[BUFER];
  char str_regexec[BUFER];
  char str[BUFER];
} config;
static inline int parser(config *opt, int argc, char **argv);
static inline int opt_check(config *opt, int argc, char **argv);
static inline void output(int argc, char **argv, config *opt);
static inline int f_flag(config *opt);
static inline void o_flag(config *opt, regex_t reg, char *filename);
static inline void options(config *opt, FILE *file, regex_t reg,
                           char *filename);
#endif /* S21_GREP */