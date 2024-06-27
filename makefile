MAKEFLAGS += --silent

CC = gcc

CFLAGS = -g -Wall -Wextra -std=c99

OBJDIR = out

LIB_SRCS = chacha.c
SRCS = $(LIB_SRCS)

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

TARGET = libchacha.a

ifeq ($(OS),Windows_NT)
    RM = del /f /q
    MKDIR = mkdir
else
    RM = rm -f
    MKDIR = mkdir -p
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -O0 -c $< -o $@

$(OBJDIR):
	$(MKDIR) $(OBJDIR)

clean:
ifeq ($(OS),Windows_NT)
	$(RM) $(OBJDIR)\*.o
	$(RM) $(TARGET)
else
	$(RM) $(OBJDIR)/*.o
	$(RM) $(TARGET)
endif