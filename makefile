MAKEFLAGS += --silent

CC = gcc

CFLAGS = -g -Wall -Wextra -std=c99

OBJDIR = out

LIB_SRCS = chacha.c
SRC_SRCS = main.c
SRCS = $(LIB_SRCS) $(SRC_SRCS)

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

TARGET = $(OBJDIR)/chacha.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

run: $(TARGET)
	./$(TARGET)

clean:
	del /f /q $(OBJDIR)\*.o
	del /f /q .\$(TARGET)