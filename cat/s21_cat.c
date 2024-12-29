#include "s21_cat.h"

int main(int argc, char *argv[]) {
  config flag = {0};
  if (!parser(&flag, argc, argv)) reading_file(argc, argv, &flag);
  return 0;
}

static inline int parser(config *flag, int argc, char **argv) {
  unsigned short error = 0;
  for (int i = 1; i < argc && error == 0; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file == NULL)  // проверка не является ли флаг файлом, если все норм, то
    {
      if (argv[i][0] == '-')  // первый символ флага
        error = (argv[i][1] == '-') ? two_defis_flags_check(flag, i, argv)
                                    : one_defis_flags_check(flag, i, argv);
      err(argv, i, error);  // выводит все ошибки
    }
  }
  return error;
}

static void err(char **argv, int i, int error) {
  switch (error) {
    case 1:
      printf("cat: illegal option -- %c\n", argv[i][1]);
      printf("usage: cat [-benstv] [file ...]\n");
      break;
    case 2:  // two_defis_flags_check возвращает 3
      printf("cat: unrecognized option \'%s\'\n", argv[i]);
      printf("Try 'cat --help' for more information.\n");
      break;
  }
}

static int s21_strcmp(const char *str1, const char *str2) {
  while (*str1 && *str1 == *str2) str1++, str2++;
  return *str1 - *str2;
}

static int two_defis_flags_check(config *flag, int i, char **argv) {
  unsigned short error = 0;
  if (s21_strcmp(argv[i], "--squeeze-blank") == 0)
    flag->s = 1;
  else if (s21_strcmp(argv[i], "--number-nonblank") == 0)
    flag->b = 1;
  else if (s21_strcmp(argv[i], "--number") == 0)
    flag->n = 1;
  else if (s21_strcmp(argv[i], "--show-ends") == 0)
    flag->e = 1;
  else if (s21_strcmp(argv[i], "--show-tabs") == 0)
    flag->t = 1;
  else
    error = 2;
  return error;
}

static int one_defis_flags_check(config *flag, int i, char **argv) {
  unsigned short error = 0;
  for (int j = 1; argv[i][j] != '\0' && error == 0; j++)
    // итерируемся до конца флага, проверяя каждый его символ,
    switch (argv[i][j]) {
      case 'e':  // проскок для того, чтобы включился и флаг -e и -v
        flag->v = 1;
        flag->e = 1;  // потом включить
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':  // проскок для того, чтобы включился и флаг -t и -v
        flag->v = 1;
        flag->t = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'b':
        flag->b = 1;
        break;
      default:
        error = 1;
        break;
    }
  if (flag->b && flag->n) flag->n = 0;
  return error;
}

static inline void reading_file(int argc, char *argv[], config *flg) {
  for (int i = 1; i < argc; i++) {  // вместо 1 начинаем с флага
    FILE *file = fopen(argv[i], "rt");
    if (file == NULL) continue;
    int q, str_count = 0, cntr = 0, empty = 0, previous = 0;
    while ((q = fgetc(file)) != EOF) {
      if (flg->s) {
        if (previous == '\n' && q == '\n') {
          if (empty) continue;
          empty = 1;
        } else
          empty = 0;
      }
      if (flg->b) {
        if (cntr == 0 && q != '\n') printf("%6d\t", ++str_count);
        cntr = 1;
        if (q == '\n') cntr = 0;
      }
      if (flg->n) {
        if (cntr == 0) printf("%6d\t", ++str_count);
        cntr = 1;
        if (q == '\n') cntr = 0;
      }
      if (flg->e && q == '\n') printf("$");
      if (flg->t && q == '\t') {
        printf("^");
        q = '\t' + 64;
      }
      if (flg->v) {
        if (q > 127 && q < 160) printf("M-^");
        if ((q < 32 && q != '\n' && q != '\t') || q == 127) printf("^");
        if ((q < 32 || (q > 126 && q < 160)) && q != '\n' && q != '\t')
          q = q > 126 ? q - 128 + 64 : q + 64;
      }
      printf("%c", q);
      previous = q;
      // приравниваем чтобы использовать в следующей итерации цикла!
    }
    fclose(file);
  }
}
/*
q - текущий симов
str_count - счетчик строк
cntr - флажок для нумерации строк
empty -  флажок для флага -s, чтобы пропускать строки
previous -  флажок для флага -s, чтобы пропускать строки
*/
