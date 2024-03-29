# -O3 optimizes, -g saves debug information, -Wall and -Wextra turns on additional warnings
CFLAGS=-O3 -g -Wall -Wextra

# Debugging flags (empty by default)
DEBUG_FLAGS=

# Check for the debugging option
ifdef DEBUG
    CFLAGS += -g
    DEBUG_FLAGS = -DDEBUG
endif

gammaCorr: main.c src/gammaCorrect.c src/getAndEditPPM.c
	gcc $(CFLAGS) -o $@ $^ -lm

.PHONY: clean
clean:
	rm -f gammaCorr