CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lopengl32 -lfreeglut -lglu32 -lglew32 -lglfw3 -lgdi32 -lmingw32

SRCDIR = src
INCDIR = include
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
HEADERS = $(wildcard $(INCDIR)/*.h)

TARGET = opengl_project

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -f $(OBJDIR)/*.o $(TARGET)