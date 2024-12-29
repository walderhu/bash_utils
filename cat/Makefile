s21_cat: 
	@gcc  -Wall -Werror -Wextra -std=c11 -pedantic s21_cat.* -o s21_cat 

rebuild: clean all

clean:
	@rm -rf s21_cat *.log

test: s21_cat
	@bash test_cat.sh

c:
	clang-format --style=Google -n *.c *.h

clang:
	clang-format --style=Google -i *.c *.h
