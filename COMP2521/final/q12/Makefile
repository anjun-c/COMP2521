# COMP2521 - Sample Exam

# !!! DO NOT MODIFY THIS FILE !!!

CC = clang
CFLAGS = -Wall -Wvla -Werror -g

########################################################################

.PHONY: asan msan nosan

asan: CFLAGS += -fsanitize=address,leak,undefined
asan: all

msan: CFLAGS += -fsanitize=memory,undefined -fsanitize-memory-track-origins
msan: all

nosan: all

########################################################################

all: testMinDiff

testMinDiff: testMinDiff.c minDiff.c bst.c
	$(CC) $(CFLAGS) -o testMinDiff testMinDiff.c minDiff.c bst.c

clean:
	rm -f testMinDiff

