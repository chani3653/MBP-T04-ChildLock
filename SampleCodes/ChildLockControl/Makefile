# Makefile — UC-001 Electronic Child Lock (ECL)
# Implemented Date: 2026-03-22
# Model Version   : claude-sonnet-4-6

CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -Werror -Iinclude -I. -Icontroller -Ientity -Iboundary
LDFLAGS =

TARGET  = ecl_simulation

SRCS    = main.c \
          boundary/driver_button_if.c \
          boundary/can_bus_if.c \
          boundary/door_actuator_if.c \
          boundary/cluster_if.c \
          entity/lock_state.c \
          controller/ecl_controller.c

OBJS    = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
