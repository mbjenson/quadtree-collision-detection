BUILD_DIR := build
CONFIG ?= Release

.PHONY: all configure build clean

all: build

configure:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -S ../ -B . -DCMAKE_BUILD_TYPE=$(CONFIG)

build: configure
	cd $(BUILD_DIR) && cmake --build . --config $(CONFIG)

run:
	./$(BUILD_DIR)/bin/PhysicsSim

clean:
	rm -rf $(BUILD_DIR)
