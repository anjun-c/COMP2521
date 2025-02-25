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

all: testDayTrip

testDayTrip: testDayTrip.c dayTrip.c Graph.c
	$(CC) $(CFLAGS) -o testDayTrip testDayTrip.c dayTrip.c Graph.c

clean:
	rm -f testDayTrip

