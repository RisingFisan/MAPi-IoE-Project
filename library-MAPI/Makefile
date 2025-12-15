# Makefile to package the library-MAPI folder into a versioned zip for distribution
# Usage:
#   make        -> creates ZIP using name-version from library.properties
#   make zip    -> same as make
#   make clean  -> remove generated zip
#   make info   -> show detected name/version

LIBDIR := $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
LIBPROPS := $(LIBDIR)library.properties

# Read name/version from library.properties (fallback to timestamp for version)
LIBNAME := $(shell awk -F= '/^name[[:space:]]*=/ {gsub(/^[ \t]+|[ \t]+$$/,"",$$2); print $$2}' $(LIBPROPS))
VERSION := $(shell awk -F= '/^version[[:space:]]*=/ {gsub(/^[ \t]+|[ \t]+$$/,"",$$2); print $$2}' $(LIBPROPS))
ifeq ($(VERSION),)
VERSION := $(shell date +%Y%m%d%H%M%S)
endif
ZIPNAME := $(LIBNAME)-$(VERSION).zip

.PHONY: all zip clean dist info install install-force
all: zip

# Directory used for assembling the distribution (temporary)
DISTDIR := $(LIBDIR)dist

zip: clean
	@echo "Packaging $(LIBNAME) version $(VERSION) -> $(ZIPNAME)"
	@rm -rf "$(DISTDIR)"
	@mkdir -p "$(DISTDIR)"
	@cd "$(LIBDIR)" && \
		rsync -a --exclude 'Makefile' --exclude '.git' --exclude '.DS_Store' --exclude 'dist' --exclude '*.zip' ./ "$(DISTDIR)/$(LIBNAME)-$(VERSION)/" >/dev/null 2>&1 || (echo "rsync failed—ensure 'rsync' is installed"; exit 1)
	@cd "$(DISTDIR)" && zip -r "$(abspath $(LIBDIR)/../$(ZIPNAME))" "$(LIBNAME)-$(VERSION)" >/dev/null || (echo "zip failed—ensure the 'zip' utility is installed"; exit 1)
	@rm -rf "$(DISTDIR)"
	@echo "Created $(abspath $(LIBDIR)/../$(ZIPNAME))"

clean:
	@echo "Removing existing zip: $(ZIPNAME)"
	@rm -f "$(abspath $(LIBDIR)/../$(ZIPNAME))"
	@rm -rf "$(DISTDIR)"

dist: zip

info:
	@echo "Library dir: $(LIBDIR)"
	@echo "library.properties: $(LIBPROPS)"
	@echo "Name: $(LIBNAME)"
	@echo "Version: $(VERSION)"
	@echo "Zip: $(ZIPNAME)"

# Default Arduino libraries directory (can be overridden by setting ARDUINO_LIB_DIR)
ARDUINO_LIB_DIR ?= $(HOME)/Documents/Arduino/libraries

# install: create the zip and unzip it into ARDUINO_LIB_DIR but fail if destination exists
.PHONY: _create_zip
_create_zip: zip

install: _create_zip
	@echo "Installing to $(ARDUINO_LIB_DIR)"
	@mkdir -p "$(ARDUINO_LIB_DIR)"
	@cd "$(ARDUINO_LIB_DIR)" && \
		if [ -d "$(LIBNAME)-$(VERSION)" ] || [ -d "$(LIBNAME)" ]; then \
			echo "Destination already exists in $(ARDUINO_LIB_DIR) — aborting. Remove existing folder or use 'make install-force'."; exit 1; \
		fi
	@unzip -q "$(abspath $(LIBDIR)/../$(ZIPNAME))" -d "$(ARDUINO_LIB_DIR)" || (echo "unzip failed"; exit 1)
	@echo "Installed $(ZIPNAME) into $(ARDUINO_LIB_DIR)"

# install-force: remove any existing library folder and install the zip (use with care)
install-force: _create_zip
	@echo "Force-installing to $(ARDUINO_LIB_DIR) (will remove existing $(LIBNAME) or $(LIBNAME)-$(VERSION))"
	@mkdir -p "$(ARDUINO_LIB_DIR)"
	@rm -rf "$(ARDUINO_LIB_DIR)/$(LIBNAME)" "$(ARDUINO_LIB_DIR)/$(LIBNAME)-$(VERSION)"
	@unzip -q "$(abspath $(LIBDIR)/../$(ZIPNAME))" -d "$(ARDUINO_LIB_DIR)" || (echo "unzip failed"; exit 1)
	@echo "Force-installed $(ZIPNAME) into $(ARDUINO_LIB_DIR)"
