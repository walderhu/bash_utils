s21_grep:
	@gcc -Wall -Wextra -Werror -std=c11 -pedantic s21_grep.c s21_grep.h -o s21_grep

clean: 
	@rm -rf logs s21_grep empty.file lyrics.txt lyrics2.txt pattern.file

rebuild:
	@clean s21_grep

c:
	clang-format --style=Google -n *.c *.h

clang:
	 clang-format --style=Google -i *.c *.h

test: s21_grep
	@bash functional_test_s21_grep.sh
