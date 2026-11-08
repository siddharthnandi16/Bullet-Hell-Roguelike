CC = gcc
CFLAGS = -I/ucrt64/include/pdcurses -MMD -MP
LDFLAGS = -static
LDLIBS = -lpdcurses -lgdi32 -luser32 -ladvapi32 -lcomdlg32 -lwinmm -lm -lole32

TARGET = spacepilot.exe

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)