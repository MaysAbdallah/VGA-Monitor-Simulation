# Verilator and SDL2 flags
VFLAGS = -O3 --x-assign fast --x-initial fast --noassert
SDL_CFLAGS = `sdl2-config --cflags`
SDL_LDFLAGS = `sdl2-config --libs`

USER_MODULE ?= user_image
TARGET_FILE = top_module.sv
TOP_BACKUP = $(TARGET_FILE).bak
OUTPUT = $(USER_MODULE)
BIN_BACKUP_DIR = old_bin
VERILOG_FILES = top_module.sv vga_signal_generator.sv
CPP_DRIVER = VGA_Monitor_SV.cpp

all: backup_existing_executables check_file pre_clean replace_image build restore_image restore_binaries


backup_existing_executables:
	@mkdir -p $(BIN_BACKUP_DIR)
	@echo "Backing up existing executables to $(BIN_BACKUP_DIR)..."
	@find . -maxdepth 1 -type f -perm -111 -exec mv {} $(BIN_BACKUP_DIR)/ \; 2>/dev/null || true

check_file:
	@if [ ! -f "$(USER_MODULE).sv" ]; then \
		echo "Error: The specified USER_MODULE ($(USER_MODULE).sv) does not exist."; \
		exit 1; \
	fi

pre_clean:
	@if [ -f "$(OUTPUT)" ]; then \
		echo "Removing existing binary: $(OUTPUT)"; \
		rm -f $(OUTPUT); \
	fi

validate_file:
	@verilator --lint-only $(USER_MODULE).sv || { \
		echo "Error: $(USER_MODULE).sv has syntax issues. Aborting build."; \
		cp $(TOP_BACKUP) $(TARGET_FILE); \
		exit 1; \
	}

replace_image: check_file validate_file
	@echo "Backing up $(TARGET_FILE) to $(TOP_BACKUP)"
	@cp $(TARGET_FILE) $(TOP_BACKUP)
	@echo "Replacing 'user_image' with '$(USER_MODULE)' in $(TARGET_FILE)"
	@sed -i "s/\buser_image\b/$(USER_MODULE)/g" $(TARGET_FILE)

build:
	@echo "Building simulation..."
	@verilator $(VFLAGS) \
		-cc $(TARGET_FILE) --exe $(CPP_DRIVER) -o $(OUTPUT) \
		-CFLAGS "$(SDL_CFLAGS)" -LDFLAGS "$(SDL_LDFLAGS)" \
		-I. -I.. \
		--top-module top_module || { \
			echo "Build failed. Restoring original module name..."; \
			cp $(TOP_BACKUP) $(TARGET_FILE); \
			rm -f $(TOP_BACKUP); \
			exit 1; \
		}
	@make -C ./obj_dir -f Vtop_module.mk
	@mv ./obj_dir/$(OUTPUT) ./

restore_image:
	@if [ -f "$(TOP_BACKUP)" ]; then \
		echo "Restoring original module to $(TARGET_FILE)"; \
		cp $(TOP_BACKUP) $(TARGET_FILE); \
		rm -f $(TOP_BACKUP); \
	fi

restore_binaries:
	@if [ -d "$(BIN_BACKUP_DIR)" ]; then \
		echo "Restoring backed up binaries from $(BIN_BACKUP_DIR)..."; \
		mv $(BIN_BACKUP_DIR)/* ./ 2>/dev/null || true; \
		rmdir $(BIN_BACKUP_DIR); \
	fi

clean:
	rm -rf ./obj_dir $(OUTPUT)
	rm -f $(TOP_BACKUP)
	rm -f $(wildcard *_image)
	rm -f $(wildcard *.out)
	rm -rf $(BIN_BACKUP_DIR)

.PHONY: all clean backup_existing_executables check_file pre_clean validate_file replace_image build restore_image restore_binaries
