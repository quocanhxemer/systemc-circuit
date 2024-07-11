# Check for available compilers
CXX := $(shell command -v g++ || command -v clang++)
ifeq ($(strip $(CXX)),)
    $(error Neither clang++ nor g++ is available. Exiting.)
endif

CC := $(shell command -v gcc || command -v clang)
ifeq ($(strip $(CC)),)
    $(error Neither clang nor gcc is available. Exiting.)
endif


# Define the directories
SRCDIR = src
INCDIR = include
BUILDDIR = build
TARGET = cache

DEFAULTS = -D_DEFAULTS
DEFAULTS += -D_DEFAULT_CYCLES_VALUE=10
DEFAULTS += -D_DEFAULT_CACHELINE_SIZE_VALUE=4
DEFAULTS += -D_DEFAULT_L1_LINES_VALUE=4
DEFAULTS += -D_DEFAULT_L2_LINES_VALUE=8
DEFAULTS += -D_DEFAULT_L1_LATENCY_VALUE=2
DEFAULTS += -D_DEFAULT_L2_LATENCY_VALUE=3
DEFAULTS += -D_DEFAULT_MEMORY_LATENCY_VALUE=4

# Compiler flags
CXXFLAGS = -std=c++14 -D_POSIX_C_SOURCE ${DEFAULTS}
CFLAGS = -std=c17 -D_POSIX_C_SOURCE ${DEFAULTS}
LDFLAGS = -L$(SYSTEMC_HOME)/lib
LDLIBS = -lsystemc

# Include all subdirectories of include and SystemC include directory
INCLUDE_DIRS := $(shell find $(INCDIR) -type d) $(SYSTEMC_HOME)/include
INC_FLAGS := $(foreach dir, $(INCLUDE_DIRS), -I$(dir))

HEADERS = $(wildcard $(INCDIR)/**/*.h) $(wildcard $(INCDIR)/**/*.hpp)

CXXFLAGS += $(INC_FLAGS)
CFLAGS += $(INC_FLAGS)

# Find all source files
CPP_SOURCES = $(wildcard $(SRCDIR)/*.cpp)
C_SOURCES = $(wildcard $(SRCDIR)/*.c)
SOURCES = $(CPP_SOURCES) $(C_SOURCES)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(CPP_SOURCES)) \
          $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(C_SOURCES))

# OS-specific settings
UNAME_S := $(shell uname -s)
ifneq ($(UNAME_S), Darwin)
    LDFLAGS += -Wl,-rpath=$(SYSTEMC_HOME)/lib
endif

# Default rule
all: debug

# Link the target
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Debug build
debug: CXXFLAGS += 	-D_LOGGING -D_FULL_LOGGING -g
debug: CFLAGS 	+= 	-D_LOGGING -D_FULL_LOGGING -g
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O2
release: $(TARGET)

# Compile C++ source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

# Compile C source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $< 

# Clean rule
clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean debug release
