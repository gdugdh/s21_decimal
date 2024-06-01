CFLAGS = -Wall -Wextra -Werror -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage -fPIC -O0

HEADERS = s21_decimal.h
SOURCES_ALL = $(wildcard ./*.c)

OBJ_LIBRARY := $(patsubst %.c, %.o, $(SOURCES_ALL))

%.o: %.c $(HEADER)
	gcc $(CFLAGS) -c $< -o $@

all: s21_decimal.a test gcov_report

s21_decimal.a: $(OBJ_LIBRARY) $(HEADERS)
	ar rcs s21_decimal.a $(OBJ_LIBRARY)
	ranlib s21_decimal.a
	rm -rf ./*/*.o
	rm -rf ./*.o

test: test_another test_comparison test_convertors test_arithmetic joellenj_test

joellenj_test: s21_decimal.a
	gcc $(CFLAGS) ./tests/joellenj_test.c s21_decimal.a -o test.out -lcheck -lm -lpthread -lrt -lsubunit
	./test.out
	rm -rf test.out
	
test_another: s21_decimal.a
	gcc $(CFLAGS) ./tests/test_another.c s21_decimal.a -o test.out -lcheck -lm -lpthread -lrt -lsubunit
	./test.out
	rm -rf test.out

test_comparison: s21_decimal.a
	gcc $(CFLAGS) ./tests/test_comparison.c s21_decimal.a -o test.out -lcheck -lm -lpthread -lrt -lsubunit
	./test.out
	rm -rf test.out

test_convertors: s21_decimal.a
	gcc $(CFLAGS) ./tests/test_convertors.c s21_decimal.a -o test.out -lcheck -lm -lpthread -lrt -lsubunit
	./test.out
	rm -rf test.out

test_arithmetic: s21_decimal.a
	gcc $(CFLAGS) ./tests/test_arithmetic.c s21_decimal.a -o test.out -lcheck -lm -lpthread -lrt -lsubunit
	./test.out
	rm -rf test.out

test_valgrind: s21_decimal.a
	gcc $(CFLAGS) ./tests/test_sub.c s21_decimal.a -o test.out -lcheck -lm -lpthread -lrt -lsubunit
	valgrind --tool=memcheck --leak-check=yes ./test.out
	rm -rf test.out

test_sub: s21_decimal.a
	gcc $(CFLAGS) ./tests/test_sub.c s21_decimal.a -o test.out -lcheck -lm -lpthread -lrt -lsubunit
	./test.out
	rm -rf test.out

gcov_report: s21_decimal.a joellenj_test
	gcc $(GCOVFLAGS) $(SOURCES_ALL) s21_decimal.a ./tests/joellenj_test.c -o gcov_test.out -lcheck -lsubunit -lm 
	./gcov_test.out
	~/.local/bin/gcovr -r . --exclude-unreachable-branches --html-details -o gcov_report.html
	firefox gcov_report.html

check:
	clang-format -n ./*.c ./*.h ./tests/*.c

fix: check
	clang-format -i ./*.c ./*.h ./tests/*.c

rebuild: clean all

clean:
	-rm -f *.o *.a *.gcno *.gcda *.info *.out *.gcov gcov_report*
	-rm -rf ./report
