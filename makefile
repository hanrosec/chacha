MAKEFLAGS += --silent

CC = gcc

CFLAGS = -g -Wall -Wextra -std=c99

OBJDIR = out

LIB_SRCS = chacha.c
SRCS = $(LIB_SRCS)

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

TARGET = libchacha.a

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	del /f /q $(OBJDIR)
	del /f /q libchacha.a
