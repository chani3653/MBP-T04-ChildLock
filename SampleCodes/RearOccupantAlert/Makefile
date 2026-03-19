# Makefile — UC-003 Rear Occupant Alert (ROA)
# Implemented Date: 2026-03-15
# Model Version   : claude-sonnet-4-6

CC      = gcc
# CFLAGS  = -std=c11 -Wall -Wextra -Werror -Iinclude
CFLAGS = -std=c11 -Wall -Wextra -Werror -Iinclude -I. -Icontroller -Ientity -Iboundary
LDFLAGS = -lm

TARGET  = roa_simulation

SRCS    = main.c \
          boundary/radar_sensor_if.c \
          boundary/cluster_if.c \
          boundary/alarm_if.c \
          boundary/mobile_app_if.c \
          entity/occupant_data.c \
          controller/roa_controller.c

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
