# 1. Compiler and Flags
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDLIBS = -lcurl # ADD THIS LINE for linker libraries

# 2. Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# 3. Executable Name
TARGET = $(BINDIR)/api_server

# 4. Find all .c files in the source directory
SOURCES = $(wildcard $(SRCDIR)/*.c)

# 5. Create a list of corresponding object files in the obj directory
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# --- Rules ---

all: $(TARGET)

# Rule to link the final executable.
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	# UPDATE THIS LINE to include the linker libraries
	$(CC) $(OBJECTS) -o $(TARGET) $(LDLIBS)
	@echo "Linking complete. Executable is at $(TARGET)"

# Rule to compile a .c file into a .o file.
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< into $@"

clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleaned build artifacts."

run: all
	./$(TARGET)

.PHONY: all clean run
