TARGET	= ircserver
CFLAGS	= -Wall -Wextra -Wno-unused-parameter -pedantic -O2 -g
LFLAGS	= -pthread

SRCDIR	= src
OBJDIR	= obj
BINDIR	= bin
DEPDIR	= dep

CMNDIR  = ../common
INCDIR  = ../include

SOURCES	:= $(wildcard $(SRCDIR)/*.c)
OBJECTS	:= $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS	:= $(OBJECTS:$(OBJDIR)/%.o=$(DEPDIR)/%.d)

CMN_SRC := $(wildcard $(CMNDIR)/*.c)
CMN_OBJ := $(CMN_SRC:$(CMNDIR)/%.c=$(OBJDIR)/%.o)
CMN_DEP := $(CMN_OBJ:$(OBJDIR)/%.o=$(DEPDIR)/%>d)


REMOVE	:= rm -rf

# Linking
$(BINDIR)/$(TARGET): $(OBJECTS) $(CMN_OBJ)
	mkdir -p $(BINDIR)
	$(CC) $(LFLAGS) -o $@ $(OBJECTS) $(CMN_OBJ)
	@echo "Linking complete"

-include $(DEPS)

# Compilation
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	mkdir -p $(OBJDIR)
	mkdir -p $(DEPDIR)
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM -MT '$(OBJDIR)/$*.o' $(SRCDIR)/$*.c > $(DEPDIR)/$*.d
	@echo "Compiled $<"

# Common code compilation
$(CMN_OBJ) : $(OBJDIR)/%.o : $(CMNDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@
	$(CC) -MM -MT '$(OBJDIR)/$*.o' $(CMNDIR)/$*.c > $(DEPDIR)/$*.d
	@echo "Compiled $<"

.PHONY: clean
clean:
	$(REMOVE) $(OBJECTS) $(OBJDIR) $(BINDIR) $(DEPDIR)
	@echo "Deleted $<"

.PHONY: remove
remove:
	$(REMOVE) $(BINDIR)/$(TARGET)
	@echo "Deleted $<"
