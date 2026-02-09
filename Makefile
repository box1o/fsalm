BUILD_DIR := build
BUILD_TYPE := Release
NUM_JOBS := $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

.DEFAULT_GOAL := run
.PHONY: configure build rebuild clean install run rwb


# ARGS  for the executable 
# -v for vide path -
# -c for camera path 
ARGS := -v="$(CURDIR)/dataset/video.mp4" -c="$(CURDIR)/dataset/camera.yaml"

configure:
	@echo "→ Configuring ($(BUILD_TYPE))..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. \
		-DCMAKE_BUILD_TYPE=$(BUILD_TYPE) \
		-DCMAKE_POLICY_VERSION_MINIMUM=4.2.0 \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON


build: configure
	@echo "→ Building..."
	@cmake --build $(BUILD_DIR) -j$(NUM_JOBS)
	@echo "✓ Build complete"

rebuild: clean build

debug:
	@$(MAKE) run BUILD_TYPE=Debug

release:
	@$(MAKE) run BUILD_TYPE=Release

run: build 
	@echo "→ Running cc studio..."

	@cd $(BUILD_DIR)/bin && ./studio $(ARGS)


all: build

rwb:
	@cd $(BUILD_DIR)/bin && ./studio $(ARGS)


