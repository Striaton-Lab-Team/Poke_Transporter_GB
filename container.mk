BUILD_LANGS := $(shell jq -r '.BUILD_LANGS | join (" ")' < $(SRCDIR)/options.json)
BUILD_TYPES := $(shell jq -r '.BUILD_TYPES | join (" ")' < $(SRCDIR)/options.json)
BUILD_XLSXS := $(shell jq -r '.BUILD_XLSXS | join (" ")' < $(SRCDIR)/options.json)

BUILD_LANG ?= $(shell jq -r '.selected.lang' < $(SRCDIR)/options.json)
BUILD_TYPE ?= $(shell jq -r '.selected.type' < $(SRCDIR)/options.json)
BUILD_XLSX ?= $(shell jq -r '.selected.xlsx' < $(SRCDIR)/options.json)

GIT_SUFFIX := $(shell git describe --tags --long --dirty | sed -E 's/^[^-]+-([0-9]+)-g[0-9a-f]+(-dirty)?$$/\1/')
GIT_FULL := $(shell git describe --tags --always --dirty 2>/dev/null)

ifeq ($(GIT_SUFFIX),0)
GIT_VERSION = $(shell git describe --tags --abbrev=0)
else
GIT_VERSION = $(shell git describe --tags --abbrev=0)b
endif

CMD_GOALS := $(filter-out build,$(MAKECMDGOALS))

LANG_INDEX := $(shell echo $(BUILD_LANGS) | tr ' ' '\n' | nl -v0 | grep -w $(BUILD_LANG) | awk '{print $$1 + 1}')
TYPE_INDEX := $(shell echo $(BUILD_TYPES) | tr ' ' '\n' | nl -v0 | grep -w $(BUILD_TYPE) | awk '{print $$1}')

CPPFLAGS   += -DPTGB_BUILD_LANGUAGE=$(LANG_INDEX)
CPPFLAGS   += -DDEBUG_MODE=$(TYPE_INDEX)
CPPFLAGS   += -DBUILD_INFO=\"$(GIT_FULL)\"

CFLAGS += $(CPPFLAGS)
CXXFLAGS += $(CPPFLAGS)


#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif


include $(DEVKITARM)/gba_rules

# The directory this makefile is located in.
# This is relevant when the second stage of this Makefile is called from the build directory.
MKFILE_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

#---------------------------------------------------------------------------------
# the LIBGBA path is defined in gba_rules, but we have to define LIBTONC ourselves
#---------------------------------------------------------------------------------
LIBTONC := $(DEVKITPRO)/libtonc
LIBPCCS := $(CURDIR)/PCCS

#---------------------------------------------------------------------------------
# Environment for building tool binaries using the host's compiler, not the devkitARM toolchain
# These tools need to run on the build machine, not the GBA.
#---------------------------------------------------------------------------------
HOST_ENV := env - \
	PATH="$(PATH)" \
	TMPDIR=/tmp TMP=/tmp TEMP=/tmp \
	SYSTEMROOT="$(SYSTEMROOT)" \
	CC=cc \
	CXX=c++ \
	CFLAGS= \
	CXXFLAGS= \
	LDFLAGS= \
	AR=ar

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
# DATA is a list of directories containing binary data
# GRAPHICS is a list of directories containing files to be processed by grit
#
# All directories are specified relative to the project directory where
# the makefile is found
#
#---------------------------------------------------------------------------------
PTGB_PROJECT_NAME ?= $(notdir $(CURDIR))
TARGET		:= $(PTGB_PROJECT_NAME)_mb
LOADERNAME  := $(PTGB_PROJECT_NAME)_standalone
BUILD		:= build
GENERATED_DIR := $(BUILD)/generated
SOURCES     := source
INCLUDES    := include PCCS/lib/include
DATA		:= data
MUSIC		:= audio
GRAPHICS	:= graphics graphics/languages/$(BUILD_LANG)
FILE_CONTAINERS := container

#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------
ARCH	:=	-mthumb -mthumb-interwork

CFLAGS	+=	-Wall -O2\
		-mcpu=arm7tdmi -mtune=arm7tdmi -masm-syntax-unified\
		$(ARCH) 

CFLAGS	+=	$(INCLUDE) -ffunction-sections -fdata-sections -Os -Wall -mthumb -mcpu=arm7tdmi -mtune=arm7tdmi -fstack-usage
CXXFLAGS	+=	$(CFLAGS) -g0 -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -std=c++20 -Wno-volatile -D_GLIBCXX_USE_CXX20_ABI=0 -fstack-usage

ifeq ($(BUILD_TYPE), debug)
	CFLAGS += -O0 -g3 -DDEBUG
	CXXFLAGS += -O0 -g3 -DDEBUG
else ifeq ($(BUILD_TYPE), release)
	CFLAGS += -flto


endif

ASFLAGS	:=	$(ARCH)
LDFLAGS	=	-Os $(ARCH) -Wl,-Map,$(notdir $*.map) -Wl,--gc-sections -mthumb -mcpu=arm7tdmi -mtune=arm7tdmi -Wl,-Map,output.map,--cref -nodefaultlibs

# eliminate libsysbase_libsysbase_a-handle_manager.o and its 4KB IWRAM buffer
LDFLAGS += -Wl,--wrap=__get_handle -Wl,--wrap=_close_r

ifeq ($(BUILD_TYPE), release)
LDFLAGS += -flto
endif

ifeq ($(BUILD_TYPE), debug)
ASFLAGS += -g
LDFLAGS += -O0 -g
endif

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS	:= -lmm -ltonc -lgba -lc -lgcc -lsysbase -lpccs


#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib.
# the LIBGBA path should remain in this list if you want to use maxmod
#---------------------------------------------------------------------------------
LIBDIRS	:=	$(LIBGBA) $(LIBTONC) $(LIBPCCS)

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------


ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

SOURCE_DIRS_RECURSIVE := $(shell find $(SOURCES) -type d)

export VPATH	:=	$(CURDIR)/$(GENERATED_DIR) \
			$(foreach dir,$(SOURCE_DIRS_RECURSIVE),$(CURDIR)/$(dir)) \
			$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
			$(foreach dir,$(GRAPHICS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

CFILES		:=	$(sort $(notdir $(shell find $(SOURCES) -type f -name "*.c")))
CPPFILES	:=	$(sort $(notdir $(shell find $(SOURCES) -type f -name "*.cpp")) translated_text.cpp)
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
PNGFILES	:=	$(foreach dir,$(GRAPHICS),$(notdir $(wildcard $(dir)/*.png)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
	export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
	export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export OFILES_GRAPHICS := $(PNGFILES:.png=.o)

export OFILES := $(OFILES_SOURCES) $(OFILES_GRAPHICS)

ifneq ($(strip $(MUSIC)),)
	export AUDIOFILES	:=	$(foreach dir,$(notdir $(wildcard $(MUSIC)/*.*)),$(CURDIR)/$(MUSIC)/$(dir))
	BINFILES += soundbank.bin
	OFILES += soundbank.bin.o
endif

export HFILES := $(addsuffix .h,$(subst .,_,$(BINFILES))) $(PNGFILES:.png=.h)

export INCLUDE	:=	$(foreach dir,$(INCLUDES),-iquote $(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					-I$(CURDIR)/$(GENERATED_DIR) \
					-I$(CURDIR)/PCCS/tools/table-generator/include \
					-I$(CURDIR)/$(BUILD)

export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all clean

GENERATE_STAMP := $(BUILD)/.generate_data.$(BUILD_LANG).$(BUILD_TYPE).stamp
BUILD_STAMP := $(BUILD)/.build.$(BUILD_LANG).$(BUILD_TYPE).stamp

TEXT_GENERATED_OUTPUTS := \
	$(GENERATED_DIR)/translated_text.h \
	$(GENERATED_DIR)/translated_text.cpp \
	$(GENERATED_DIR)/fonts.h

all:
	@set -e; \
	if [ "$(BUILD_XLSX)" = "remote" ]; then \
		$(MAKE) --no-print-directory -f $(SRCDIR)/container.mk text_generated BUILD_LANG=$(BUILD_LANG) BUILD_TYPE=$(BUILD_TYPE) BUILD_XLSX=$(BUILD_XLSX); \
	fi; \
	before=$$(stat -c %Y $(BUILD_STAMP) 2>/dev/null || echo 0); \
	$(MAKE) --no-print-directory -f $(SRCDIR)/container.mk $(BUILD_STAMP) BUILD_LANG=$(BUILD_LANG) BUILD_TYPE=$(BUILD_TYPE) BUILD_XLSX=$(BUILD_XLSX); \
	after=$$(stat -c %Y $(BUILD_STAMP) 2>/dev/null || echo 0); \
	if [ "$$before" = "$$after" ] && [ "$$after" != "0" ]; then \
		echo "PTGB build up to date."; \
	fi

text_generated: to_compress generated_dir data
	@PTGB_GEN_DIR="$(CURDIR)/$(GENERATED_DIR)" python3 tools/text_helper/main.py $(BUILD_LANG) $(BUILD_TYPE) $(BUILD_XLSX)

data:
	@mkdir -p $@

to_compress:
	@mkdir -p $@

generated_dir:
	@mkdir -p $(GENERATED_DIR)

generate_data: $(GENERATE_STAMP)

$(GENERATE_STAMP): compress_lz10.sh | data to_compress generated_dir
	@if [ "$(BUILD_XLSX)" != "remote" ]; then \
		$(MAKE) --no-print-directory -f $(SRCDIR)/container.mk text_generated BUILD_LANG=$(BUILD_LANG) BUILD_TYPE=$(BUILD_TYPE) BUILD_XLSX=$(BUILD_XLSX); \
	fi
	@echo "----------------------------------------------------------------"
	@echo "Building v$(GIT_VERSION) with parameters: $(BUILD_LANG), $(BUILD_TYPE), $(BUILD_XLSX)"
	@echo "----------------------------------------------------------------"
	@$(HOST_ENV) $(MAKE) -C tools/make-file-container
	@$(HOST_ENV) $(MAKE) -C tools/gb-payload-generator
	@$(HOST_ENV) $(MAKE) -C tools/gba-payload-generator
	@echo
	@echo "----------------------------------------------------------------"
	@echo
	@find tools/text_helper/build -name "*.containerdef" -print0 | xargs -0 -n1 tools/make-file-container/make-file-container to_compress
	tools/generate_gba_payloads.sh build/ to_compress/RSEFRLG.chunk0.bin
	@find build/bps-patches -name "*.containerdef" -print0 | xargs -0 -n1 tools/make-file-container/make-file-container -H $(BUILD) to_compress
	@find $(FILE_CONTAINERS) -name "*.containerdef" -print0 | xargs -0 -n1 tools/make-file-container/make-file-container -H $(BUILD) to_compress
	@echo "Compressing bin files!" 
	@echo -n "["
	@find to_compress -name "*.bin" -print0 | xargs -0 -n1 $(SRCDIR)/compress_lz10.sh
	@echo "]"
	@echo "Compressing finished!"
	@echo
	@echo "----------------------------------------------------------------"
	@echo
	@touch $@

#---------------------------------------------------------------------------------
$(BUILD_STAMP): $(GENERATE_STAMP) | $(BUILD)
	@$(MAKE) -C PCCS \
		CC="$(CC)" \
		CXX="$(CXX)" \
		CFLAGS="$(CFLAGS)" \
		CXXFLAGS="$(CXXFLAGS)" \
		LDFLAGS="$(LDFLAGS)"
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/container.mk
	@mkdir -p loader/data
	@cp $(TARGET).gba loader/data/multiboot_rom.bin
	@$(MAKE) -C loader
	@cp loader/loader.gba $(LOADERNAME).gba
	@touch $@

$(BUILD):
	@mkdir -p $@

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@$(MAKE) -C tools/make-file-container clean
	@$(MAKE) -C tools/gb-payload-generator clean
	@$(MAKE) -C tools/gba-payload-generator clean
	@$(MAKE) -C loader clean
	@$(MAKE) -C PCCS clean
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba $(LOADERNAME).gba data/ to_compress/
	@rm -f text_helper/output.json
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba data/ to_compress/
	@rm -fr $(GENERATED_DIR)



#---------------------------------------------------------------------------------
else

BINFILES	:=	$(foreach dir,../$(DATA),$(notdir $(wildcard $(dir)/*.*)))
export OFILES_BIN := $(addsuffix .o,$(BINFILES))
OFILES := $(OFILES_BIN) $(OFILES)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------

$(OUTPUT).gba	:	$(OUTPUT).elf

$(OUTPUT).elf	:	$(OFILES)

$(OFILES_SOURCES) : $(HFILES)

#---------------------------------------------------------------------------------
# The bin2o rule should be copied and modified
# for each extension used in the data directories
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
# rule to build soundbank from music files
#---------------------------------------------------------------------------------
soundbank.bin soundbank.h : $(AUDIOFILES)
#---------------------------------------------------------------------------------
	@mmutil $^ -osoundbank.bin -hsoundbank.h

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
# This rule creates C source files using grit
# grit takes an image file and a .grit describing how the file is to be processed
# add additional rules like this for each image extension
# you use in the graphics folders
#---------------------------------------------------------------------------------
%.c %.h: %.png %.grit
#---------------------------------------------------------------------------------
	@echo "grit $(notdir $<)"
	@grit $< -ftc -o$*

# make likes to delete intermediate files. This prevents it from deleting the
# files generated by grit after building the GBA ROM.
.SECONDARY:

-include $(DEPSDIR)/*.d
#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
