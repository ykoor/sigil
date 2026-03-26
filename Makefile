##
# Sigil
#
# @file
# @version 0.1

CC      = clang
CFLAGS  = -Wall -Wextra -g -MMD -MP
INCLUDE = -I.
BUILD   = build

SIGIL_SRCS := $(wildcard sigil/*.c) $(wildcard sigil/types/*.c)
SIGIL_OBJS := $(SIGIL_SRCS:.c=.o)
SIGIL_DEPS := $(SIGIL_OBJS:.o=.d)

TOMB_SRCS := tomb/parse.c
TOMB_OBJS := $(TOMB_SRCS:.c=.o)
TOMB_DEPS := $(TOMB_OBJS:.o=.d)

SIGIL_TYPE_OBJS := sigil/types/rune.o sigil/types/sigil.o sigil/types/futhark.o

$(BUILD)/tomb: $(TOMB_OBJS) $(SIGIL_TYPE_OBJS) | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $^

JINN_SRCS := $(wildcard jinn/*.c)
JINN_OBJS := $(JINN_SRCS:.c=.o)
JINN_DEPS := $(JINN_OBJS:.o=.d)

$(BUILD)/jinn: $(JINN_OBJS) | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $^

CONJURE_SRCS := $(wildcard conjure/*.c)
CONJURE_OBJS := $(CONJURE_SRCS:.c=.o)
CONJURE_DEPS := $(CONJURE_OBJS:.o=.d)

$(BUILD)/conjure: $(CONJURE_OBJS) | $(BUILD)
	$(CC) $(CFLAGS) -o $@ $^

ALL_DEPS := $(SIGIL_DEPS) $(TOMB_DEPS) $(JINN_DEPS) $(CONJURE_DEPS)
ALL_OBJS := $(SIGIL_OBJS) $(TOMB_OBJS) $(JINN_OBJS) $(CONJURE_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

-include $(ALL_DEPS)

$(BUILD):
	mkdir -p $(BUILD)

all: $(BUILD)/tomb

clean:
	rm -f $(ALL_OBJS) $(ALL_DEPS)
	rm -rf $(BUILD)

.PHONY: all clean
# end
