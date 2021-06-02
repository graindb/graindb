.PHONY: duckdb clean debug release

all: duckdb release

clean:
	rm -rf build

GENERATOR=
FORCE_COLOR=
ifeq ($(GEN),ninja)
	GENERATOR=-G "Ninja"
	FORCE_COLOR=-DFORCE_COLORED_OUTPUT=1
endif

debug:
	mkdir -p build/debug && \
	cd build/debug && \
	cmake $(GENERATOR) $(FORCE_COLOR) -DCMAKE_BUILD_TYPE=Debug ../.. && \
	make

release:
	mkdir -p build/release && \
	cd build/release && \
	cmake $(GENERATOR) $(FORCE_COLOR) -DCMAKE_BUILD_TYPE=Release ../.. && \
	make

duckdb:
	cd ../../.. && \
	make debug -j8
# main:
# 	mkdir -p build && mkdir -p build/release && \
# 	cd build/release && cmake ../.. && make
# 	build/example
