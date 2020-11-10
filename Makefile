# Optimize, turn on additional warnings
CFLAGS=-O3 -g -Wall -Wextra -no-pie

.PHONY: all
all: NewtonCotes
NewtonCotes: main.c newton_cotes_1.S  newton_cotes_2.S

	$(CC) $(CFLAGS) -o $@ $^ -ldl

.PHONY: clean
clean:
	rm -f NewtonCotes
