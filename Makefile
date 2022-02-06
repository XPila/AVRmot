#-----------------------------------------------------------------------------------------------------------------------
# Makefile to compile and link AVRmot
PROJECT ?= AVRmot
# build configuration list
BUILD_CONFIGURATIONS  := einsy mmctl slactl
# selected build configuration, default is einsy
BUILD_CONFIGURATION  ?= einsy
# check valid build configuration
ifneq (,$(filter-out $(BUILD_CONFIGURATIONS), $(BUILD_CONFIGURATION)))
$(error invalid configuration "$(BUILD_CONFIGURATION)")
endif

# build artefacts
BUILD := elf sym hex bin lst

# board number (10=einsy, 20=mmctl, 30=slactl)
BOARD := $(strip \
$(if $(findstring $(BUILD_CONFIGURATION),einsy),10,\
$(if $(findstring $(BUILD_CONFIGURATION),mmctl),20,\
$(if $(findstring $(BUILD_CONFIGURATION),slactl),30,\
))))
ifneq (,$(filter-out 10 20 30, $(BOARD)))
$(error invalid board "$(BOARD)")
endif

# mcu type (atmega32u4 or atmega2560, automaticaly determined from BOARD)
MCU := $(if $(findstring $(BOARD),20 30),atmega32u4,$(if $(findstring $(BOARD),10),atmega2560))
# cpu frequency (16MHz)
F_CPU := 16000000

# default buildnumber from git
GIT_BUILD_NUMBER := $(shell git rev-list --count HEAD)
FW_BUILD_NUMBER ?= $(if $(GIT_BUILD_NUMBER),$(GIT_BUILD_NUMBER),0)
# firmware version from file 'version.txt', format '0.9-BETA'
FW_VERSION ?= $(file < version.txt)
# firmware version full, format '0.9-BETA+64'
FW_VERSION_FULL ?= $(FW_VERSION)+$(FW_BUILD_NUMBER)

# C and C++ language
GCCSTD := gnu99
GPPSTD := c++98

# target platform prefix
TOOLCHAIN_PREFIX := avr

# file path separator (for unix '/', for windows '\\', automaticaly determined from PATH)
PATH_SEPARATOR := $(if $(findstring \,$(PATH)),\,/)
# unix shell or windows shell - determine by PATH_SEPARATOR
SHELL_IS_UNIX := $(findstring /,$(PATH_SEPARATOR))
# shell command for removing directories
SHELL_RMDIR ?= $(if $(SHELL_IS_UNIX),rm -rvf,RMDIR /S /Q)
# shell command for creating directories
SHELL_MKDIR ?= $(if $(SHELL_IS_UNIX),mkdir -p,MKDIR)
# shell command for removing files
SHELL_RM ?= $(if $(SHELL_IS_UNIX),rm,DEL /Q)
# shell command for renaming files/dirs
SHELL_MV ?= $(if $(SHELL_IS_UNIX),mv,REN)
# shell command 'cat'
SHELL_CAT ?= $(if $(SHELL_IS_UNIX),cat,TYPE)
# shell command separator
SHELL_CMDSEP ?= $(if $(SHELL_IS_UNIX), ; , & )

# output folder (output directory tree will be automaticaly created)
OUT := build/$(BUILD_CONFIGURATION)

# gcc, g++, ld, objcopy, objdump, nm
GCC     := $(if $(TOOLCHAIN_PREFIX),$(TOOLCHAIN_PREFIX)-)gcc
GPP     := $(if $(TOOLCHAIN_PREFIX),$(TOOLCHAIN_PREFIX)-)g++
LD      := $(if $(TOOLCHAIN_PREFIX),$(TOOLCHAIN_PREFIX)-)g++
OBJCOPY := $(if $(TOOLCHAIN_PREFIX),$(TOOLCHAIN_PREFIX)-)objcopy
OBJDUMP := $(if $(TOOLCHAIN_PREFIX),$(TOOLCHAIN_PREFIX)-)objdump
NM      := $(if $(TOOLCHAIN_PREFIX),$(TOOLCHAIN_PREFIX)-)nm
AVRDUDE := bin/windows/avrdude/avrdude
AVRDUDE_PORT := COM12

# output files
OUTDIR := $(OUT)/$(PROJECT).dir # file containing list of created directories
OUTELF := $(OUT)/$(PROJECT).elf # elf output file
OUTMAP := $(OUT)/$(PROJECT).map # map output file
OUTSYM := $(OUT)/$(PROJECT).sym # sym output file
OUTHEX := $(OUT)/$(PROJECT).hex # hex output file
OUTBIN := $(OUT)/$(PROJECT).bin # bin output file
OUTLST := $(OUT)/$(PROJECT).lst # lst output file

# all source
ALLSRC := $(addprefix src/,\
timer0.c sys.c uart.c rbuf.c adc.c gpio_$(MCU).c tmc2130.c st4.c st4_sr2d.c cmd.c cmd_xyze.c \
)
# einsy source
ALLSRC += $(if $(findstring $(BOARD),10),\
$(addprefix src/einsy/,main.c einsy.c cmd_einsy.c tmc2130_hw_einsy.c))
# mmctl source
ALLSRC += $(if $(findstring $(BOARD),20),\
$(addprefix src/mmctl/,main.c mmctl.c cmd_mmctl.c shr16.c tmc2130_hw_mmctl.c))

# external definitions
SYMBOLS := $(addprefix -D,\
MCU=$(MCU)\
F_CPU=$(F_CPU)\
BOARD=$(BOARD)\
FW_VERSION_FULL=$(FW_VERSION_FULL)\
)

# include directories
INCLUDES := -I./src
# einsy includes
INCLUDES += $(if $(findstring $(BOARD),10),-I./src/einsy)
# mmctl includes
INCLUDES += $(if $(findstring $(BOARD),20),-I./src/mmctl)

# common flags (compilers and linker)
CMNFLAGS := -g -Os -mmcu=$(MCU)
CMNFLAGS += -flto

# compiler flags
GCCFLAGS := $(CMNFLAGS) $(INCLUDES) $(SYMBOLS) -ffunction-sections -fdata-sections -MMD -std=$(GCCSTD) -Wall
GPPFLAGS := $(CMNFLAGS) $(INCLUDES) $(SYMBOLS) -ffunction-sections -fdata-sections -MMD -std=$(GPPSTD) -fno-exceptions -Wall
GCCFLAGS += -fno-fat-lto-objects
GPPFLAGS += -fno-fat-lto-objects

# linker flags
#LDFLAGS  := $(CMNFLAGS) -Wl,--gc-sections -Wl,-Map=$(OUTMAP) -Wl,-u,vfprintf -lprintf_min -Wl,-u,vfscanf -lscanf_min
LDFLAGS := $(CMNFLAGS) -Wl,-Map=$(OUTMAP)
LDFLAG_PRINTF := -Wl,-u,vfprintf -lprintf_flt
LDFLAG_SCANF := -Wl,-u,vfscanf -lscanf_flt
LDFLAG_SECTIONS := -Wl,--gc-sections
LDFLAGS += $(LDFLAG_SECTIONS) $(LDFLAG_PRINTF) $(LDFLAG_SCANF)

# list of all directories
ALLDIR := $(addprefix $(OUT)/,$(sort $(subst / , ,$(dir $(ALLSRC)) )))

# lists of all assembler, C and C++ source files
GCCSRC := $(filter %.c, $(ALLSRC))
GPPSRC := $(filter %.cpp, $(ALLSRC))

# lists of all assembler, C and C++ object files
GCCOBJ := $(addprefix $(OUT)/,$(GCCSRC:.c=.o))
GPPOBJ := $(addprefix $(OUT)/,$(GPPSRC:.cpp=.o))


$(OUTDIR):
	@echo creating output directory tree
	@$(SHELL_MKDIR) $(subst /,$(PATH_SEPARATOR),$(OUT))
	@$(SHELL_MKDIR) $(subst /,$(PATH_SEPARATOR),$(ALLDIR))
	@echo $(ALLDIR) >$(OUTDIR)

$(OUT)/%.o: %.c $(OUTDIR)
	@echo compiling $<
	@$(GCC) -c $(GCCFLAGS) -o $@ $<

$(OUT)/%.o: %.cpp $(OUTDIR)
	@echo compiling $<
	@$(GPP) -c $< $(GPPFLAGS) -o $@

$(OUTELF): $(GCCOBJ) $(GPPOBJ)
	@echo linking $@
#	@$(file > $(OUT)/.objlist,$(GCCOBJ) $(GPPOBJ))
	@$(LD) -o $(OUTELF) $(GCCOBJ) $(GPPOBJ) $(LDFLAGS)
#	@$(LD) -o $(OUTELF) @$(OUT)/.objlist $(LDFLAGS)

$(OUTSYM): $(OUTELF)
	@echo generating sym file $@
	@$(NM) -n $< > $@ 

$(OUTHEX): $(OUTELF)
	@echo generating hex file $@
	@$(OBJCOPY) -O ihex $< $@

$(OUTBIN): $(OUTELF)
	@echo generating bin file $@
	@$(OBJCOPY) -O binary $< $@

$(OUTLST): $(OUTELF)
	@echo generating lst file $@
	@$(OBJDUMP) -h -S -z $< > $@ 


all: build

build: $(BUILD)

elf: $(OUTELF)
sym: $(OUTSYM)
hex: $(OUTHEX)
bin: $(OUTBIN)
lst: $(OUTLST)


clean:
	@echo removing output files $(OUT)
ifneq ("$(wildcard $(OUT))","")
	@$(SHELL_RMDIR) $(subst /,$(PATH_SEPARATOR),$(OUT))
endif



flash_usbasp: hex
	$(AVRDUDE) -p $(MCU) c usbasp -P usb --F -v -u -V -U flash:w:$(OUTHEX)

flash_wiring: hex
	$(AVRDUDE) -p $(MCU) -c wiring -P $(AVRDUDE_PORT) -F -v -u -V -b115200 -D -U flash:w:$(OUTHEX)


.PHONY: all build clean flash_usbasp flash_wiring test


test:
	$(GIT)

printvars:
	@echo BUILD_CONFIGURATION=$(BUILD_CONFIGURATION)
	@echo BUILD=$(BUILD)
	@echo MCU=$(MCU)
	@echo BOARD=$(BOARD)
	@echo F_CPU=$(F_CPU)
	@echo FW_BUILD_NUMBER=$(FW_BUILD_NUMBER)
	@echo FW_VERSION=$(FW_VERSION)
	@echo FW_VERSION_FULL=$(FW_VERSION_FULL)
	@echo TOOLCHAIN_PREFIX=$(TOOLCHAIN_PREFIX)
	@echo PATH_SEPARATOR=$(PATH_SEPARATOR)
	@echo SHELL_IS_UNIX=$(SHELL_IS_UNIX)
	@echo SHELL_RMDIR=$(SHELL_RMDIR)
	@echo SHELL_MKDIR=$(SHELL_MKDIR)
	@echo SHELL_RM=$(SHELL_RM)
	@echo SHELL_MV=$(SHELL_MV)
	@echo SHELL_CAT=$(SHELL_CAT)
#	@echo SHELL_CMDSEP=$(SHELL_CMDSEP)
	@echo OUT=$(OUT)
	@echo GCC=$(GCC)
	@echo GPP=$(GPP)
	@echo LD=$(LD)
	@echo OBJCOPY=$(OBJCOPY)
	@echo OBJDUMP=$(OBJDUMP)
	@echo OUTDIR=$(OUTDIR)
	@echo OUTELF=$(OUTELF)
	@echo OUTSYM=$(OUTSYM)
	@echo OUTMAP=$(OUTMAP)
	@echo OUTHEX=$(OUTHEX)
	@echo OUTBIN=$(OUTBIN)
	@echo ALLSRC=$(ALLSRC)
	@echo SYMBOLS=$(SYMBOLS)
	@echo INCLUDES=$(INCLUDES)
	@echo CMNFLAGS=$(CMNFLAGS)
	@echo GCCFLAGS=$(GCCFLAGS)
	@echo GPPFLAGS=$(GPPFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo ALLDIR=$(ALLDIR)
	@echo GCCSRC=$(GCCSRC)
	@echo GPPSRC=$(GPPSRC)
	@echo GCCOBJ=$(GCCOBJ)
	@echo GPPOBJ=$(GPPOBJ)
	@echo GCCOBJ=$(GCCOBJ)
