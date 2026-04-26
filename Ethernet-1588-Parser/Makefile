# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -I./includes -g 

# Directories and Files
SRCDIR = sources
INCDIR = includes
OBJDIR = objects
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/ptp.c
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET = ptp_slave
TEST_TARGET = ptp_slave_test

# Create Object Directory if not exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Compile for production
compile: $(OBJDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -lpcap

# Compile for test mode
compile_test: $(OBJDIR) $(OBJECTS)
	$(CC) $(CFLAGS) -DTEST_MODE $(SOURCES) -o $(TEST_TARGET) -lpcap

# Run the program
run: 
	./$(TARGET)

run_test:
	./$(TEST_TARGET)

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean all generated files
clean:
	rm -rf $(OBJDIR) $(TARGET) $(TEST_TARGET)

# Phony Targets
.PHONY: compile compile_test run clean
