# Target library
lib	:= libuthread
objs	:= queue.o uthread.o context.o preempt.o

CC 	:= gcc
CFLAGS	:= -Wall -Werror -MMD
CFLAGS	+= -g

ifneq ($(V),1)
Q = @
endif

all: $(lib)

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

libuthread: $(objs)
	ar rcs libuthread.a $(objs)

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "clean"
	$(Q)rm -f $(lib) $(objs) $(deps)
