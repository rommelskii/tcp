# 1. Compiler and Flags
CC = gcc
# -Iinclude tells the compiler to look in the 'include' directory for header files.
# -Wall and -Wextra are flags for showing warnings.
CFLAGS = -Iinclude -Wall -Wextra -g

# 2. Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# 3. Executable Name
TARGET = $(BINDIR)/api_server

# 4. Find all .c files in the source directory
SOURCES = $(wildcard $(SRCDIR)/*.c)

# 5. Create a list of corresponding object files in the obj directory
# This replaces 'src/%.c' with 'obj/%.o' for each source file
OBJECTS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# --- Rules ---

# The 'all' rule is the default one that runs when you just type 'make'.
# It depends on the final executable.
all: $(TARGET)

# Rule to link the final executable.
# It depends on all the object files.
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)  # Create the bin directory if it doesn't exist
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "Linking complete. Executable is at $(TARGET)"

# Rule to compile a .c file into a .o file.
# This pattern matches any .c file in src/ and creates a corresponding .o file in obj/.
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)  # Create the obj directory if it doesn't exist
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< into $@"

# 'clean' rule to remove all build artifacts.
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "Cleaned build artifacts."

# A phony target to run the program.
run: all
	./$(TARGET)

# Phony targets are ones that don't represent actual files.
.PHONY: all clean run
