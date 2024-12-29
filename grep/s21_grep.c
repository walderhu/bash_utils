#include "s21_grep.h"

int main(int argc, char **argv) {
  config opt = {0};
  if (parser(&opt, argc, argv) != ERR) output(argc, argv, &opt);
  return 0;
}

static inline int parser(config *opt, int argc, char **argv) {
  int status = ONGOING;
  status = opt_check(opt, argc, argv);
  if (status == ERR) return status;
  if (!(opt->e || opt->f) && (argc > optind)) {
    strcat(opt->str_regexec, argv[optind]);
    optind++;
  }
  if (opt->e || opt->f) opt->str_regexec[strlen(opt->str_regexec) - 1] = '\0';
  if (opt->c && opt->n) opt->n = 0;
  if (opt->l && opt->n) opt->n = 0;
  if (opt->l && opt->o) opt->o = 0;
  if (opt->l && opt->c) opt->c = 0;
  if (opt->c && opt->o) opt->o = 0;
  opt->multiple_files = argc - optind;
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "rb");
    if (file == NULL) {
      if (!opt->s) fprintf(stderr, FILE_ERR_MSG, argv[i]);
    } else
      fclose(file);
  }
  return status;
}

static inline int opt_check(config *opt, int argc, char **argv) {
  int res, status = ONGOING;
  int opterr_backup = opterr;  // Сохраняем текущее значение переменной opterr,
                               // чтобы не выводить сообщение об ошибках
  opterr = 0;  // Устанавливаем значение opterr в 0
  while ((res = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (res) {
      case 'e':
        opt->e = 1;
        if (strstr(opt->str_regexec, optarg) == NULL) {
          strcat(opt->str_regexec, optarg);
          strcat(opt->str_regexec, "|");
        }
        break;
      case 'i':
        opt->i = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'c':
        opt->c = 1;
        break;
      case 'l':
        opt->l = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'h':
        opt->h = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'f':
        opt->f = 1;
        strcpy(opt->file_pattern, optarg);
        status = f_flag(opt);
        break;
      case 'o':
        opt->o = 1;
        break;
    }
  }
  opterr = opterr_backup;  // Восстанавливаем значение переменной opterr
  return status;
}

static inline void output(int argc, char **argv, config *opt) {
  FILE *file;
  regex_t reg;
  int flag_i = (!opt->i) ? REG_EXTENDED : REG_EXTENDED | REG_ICASE;
  regcomp(&reg, opt->str_regexec, flag_i);
  for (int i = optind; i < argc; i++) {
    file = fopen(argv[i], "rb");
    if (file != NULL) {
      if (opt->c && (opt->multiple_files > 1) && !opt->h)
        printf("%s:", argv[i]);
      if (opt->n) opt->numb_line = 0;
      if (opt->c || opt->l) opt->numb_word = 0;
      options(opt, file, reg, argv[i]);
      if (opt->l && opt->numb_word) printf("%s\n", argv[i]);
      fclose(file);
      if (opt->c) printf("%d\n", opt->numb_word);
    }
  }
  regfree(&reg);
}

static inline void options(config *opt, FILE *file, regex_t reg,
                           char *filename) {
  int status = ONGOING;
  while (fgets(opt->str, BUFER, file) != NULL) {
    if (opt->n) opt->numb_line++;
    status = regexec(&reg, opt->str, 0, NULL, 0);
    if (opt->v) status = !status;
    if (status == 0) {
      if (!(strcmp(opt->str, "\n") == 0 && strcmp(opt->str_regexec, ".") == 0))
        opt->numb_word++;
      if (opt->o) o_flag(opt, reg, filename);
      if (!opt->c && !opt->l && !opt->o) {
        if ((opt->multiple_files > 1) && !opt->l && !opt->h && !opt->c)
          printf("%s:", filename);
        if (opt->n) printf("%d:", opt->numb_line);
        printf("%s", opt->str);  // главный вывод))
        if (opt->str[strlen(opt->str) - 1] != '\n') printf("\n");
      }
    }
  }
}

static inline int f_flag(config *opt) {
  int status = ONGOING;
  FILE *file;
  file = fopen(opt->file_pattern, "rb");
  if (file == NULL) {
    fprintf(stderr, FILE_ERR_MSG, opt->file_pattern);
    status = ERR;
  } else {
    while (!feof(file)) {
      if (fgets(opt->str, BUFER, file) != NULL) {
        if (opt->str[strlen(opt->str) - 1] == '\n' && strlen(opt->str) - 1 != 0)
          opt->str[strlen(opt->str) - 1] = '\0';
        strcat(opt->str_regexec, opt->str);
        strcat(opt->str_regexec, "|");
      }
    }
    fclose(file);
  }
  return status;
}

static inline void o_flag(config *opt, regex_t reg, char *filename) {
  regmatch_t pmatch[1];
  const char *p = opt->str;
  int status = regexec(&reg, p, 1, pmatch, 0);
  while (status == 0) {
    int start = pmatch[0].rm_so;
    int end = pmatch[0].rm_eo;
    char *match = malloc(end - start + 1);
    memcpy(match, &p[start], end - start);
    match[end - start] = '\0';
    if (match[0] != '\0' && (opt->multiple_files > 1) && !opt->l && !opt->h &&
        !opt->c)
      printf("%s:", filename);
    if (opt->n) printf("%d:", opt->numb_line);
    printf("%s\n", match);
    free(match);
    p += end;
    status = regexec(&reg, p, 1, pmatch, 0);
  }
}