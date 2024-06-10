.PHONY: all clean

# Check if clang++ is available, if not, check if g++ is available
CXX := $(shell command -v clang++ 2>/dev/null)
ifeq ($(strip $(CXX)),)
	CXX := $(shell command -v g++ 2>/dev/null)
endif

systemc_dir := $(shell pwd)/systemc


all: systemc



systemc: configure
	mkdir -p $(systemc_dir)
	cd temp/systemc/objdir && \
	cmake -DCMAKE_INSTALL_PREFIX="$(dest_dir)" -DCMAKE_CXX_STANDARD=14  .. && \
	make -j$$(nproc) 2> ../../../install.log && \
	make install
	rm -rf temp

configure: temp
	cd temp/systemc && \
	mkdir -p objdir

temp:
	mkdir -p temp
	cd temp && git clone --depth 1 --branch 2.3.4 https://github.com/accellera-official/systemc.git

clean:
	rm -rf packages
	rm -rf temp