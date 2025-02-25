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

all: testAppend

testAppend: testAppend.c append.c list.c
	$(CC) $(CFLAGS) -o testAppend testAppend.c append.c list.c

clean:
	rm -f testAppend

