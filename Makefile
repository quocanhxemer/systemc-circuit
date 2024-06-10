.PHONY: all clean

# entry point for the program and target name
MAIN := sc_main.c

SRCDIR = src
INCDIR = $(wildcard include/**/*.h*)

SCPATH := $(shell pwd)/systemc

CFLAGS = -Wall -std=c17 -I$(SCPATH)/include -L$(SCPATH)/lib -lsystemc -lm -I$(INCDIR)
CXXFLAGS = -Wall -std=c++14 -I$(SCPATH)/include -L$(SCPATH)/lib -lsystemc -lm -I$(INCDIR)

# Add rpath except for MacOS
UNAME_S := $(shell uname -s)
ifneq ($(UNAME_S), Darwin)
    CXXFLAGS += -Wl,-rpath=$(SCPATH)/lib
endif

# Define the list of source files and object files
CSOURCES = $(wildcard $(SRCDIR)/*.c)
CXXSOURCES = $(wildcard $(SRCDIR)/*.cpp)

COBJS = $(CSOURCES:.c=.o)
CXXOBJS = $(CXXSOURCES:.cpp=.o)

OBJS = $(COBJS) $(CXXOBJS)

# Define the target executable
TARGET = cache

# Check if clang++ is available, if not, check if g++ is available
CXX := $(shell command -v clang++ 2>/dev/null)
ifeq ($(strip $(CXX)),)
	CXX := $(shell command -v g++ 2>/dev/null)
endif

# Check if clang is available, if not, check if gcc is available
CC := $(shell command -v clang 2>/dev/null)
ifeq ($(strip $(CC)),)
	CC := $(shell command -v gcc 2>/dev/null)
endif

all: debug

# Debug build
debug: CXXFLAGS += -g
debug: CFLAGS += -g
debug: systemc cache

# Release build
release: CXXFLAGS += -O2
release: CFLAGS += -O2
release: systemc cache

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Rule to build C++ object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build C object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@










systemc: configure
	mkdir -p systemc
	cd temp/systemc/objdir && \
	cmake -DCMAKE_INSTALL_PREFIX="$(dest_dir)" -DCMAKE_CXX_STANDARD=14  .. && \
	make -j$$(nproc) 2> ../../../install.log && \
	make install

configure: temp
	cd temp/systemc && \
	mkdir -p objdir

temp:
	mkdir -p temp
	cd temp && git clone --depth 1 --branch 2.3.4 https://github.com/accellera-official/systemc.git

clean:
	rm -rf packages
	rm -rf temp
	rm -f $(OBJS) $(TARGET)