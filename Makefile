# by hmgle

TARGET := busy_job
CFLAGS := -Wall -g -O0
CC     := gcc
STRIP  := strip
TAR	   := tar

all: $(TARGET)

busy_job: busy_job.o
	$(CC) $^ -o $@
	# $(STRIP) $@

busy_job.o: busy_job.c
	$(CC) -c $^ -o $@ $(CFLAGS)

.PHONY: clean tar

clean:
	-rm -f *.o $(TARGET) *.tar.gz

tar:
	$(TAR) czf busy_job.tar.gz \
		$(wildcard *.c) ./Makefile README iw.in

