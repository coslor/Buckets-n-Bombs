# TARGET ?= "bin/Buckets n Bombs.d64"
# SRC_DIRS ?="C:\Users\chris\Projects\cc65\Buckets n Bombs\src"

# SRCS := $(shell find $(SRC_DIRS) -name "*.c")) $(shell find $(SRC_DIRS) -name "*.s"))
# OBJS := $(TARGET)
# #OBJS := $(addsuffix .o,$(basename $(SRCS)))
# #DEPS := $(OBJS:.o=.d)

# #INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# #INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# CPPFLAGS ?= -I ./include -O

# CC=cl65
# $(TARGET): $(OBJS)
	# $(CC) $(LDFLAGS) $(OBJS) -o $@ $(LOADLIBES) $(LDLIBS)

# .PHONY: clean
# clean:
# #	$(RM) $(TARGET) $(OBJS) $(DEPS)
	# del bin/*.o bin/*.d64
# -include include

TARGET_NAME=Buckets_n_Bombs
TARGET="bin\$(TARGET_NAME).prg"

VICE_HOME=C:\Users\chris\WinVice\GTK3VICE-3.3-win64-r37152
VICE=$(VICE_HOME)\x64sc.exe
1541=$(VICE_HOME)\c1541.exe

DISK_NAME=bin\$(TARGET_NAME).d64
all: run

$(TARGET):
# 	-O for optimized ocde ,-g for debugging
#	cl65 -O -I "include" -o $(TARGET) "src\Buckets n Bombs.c" "src\sprite_data.s"
#	cl65 -g --debug-info -Wl  --dbgfile,BnB.dbg -Ln BnB.lbl -m BnB.map -C BnB-c64.cfg -I "include" -o $(TARGET) "src\Buckets n Bombs.c" "src\sprite_data.s"
	cl65 -g --debug-info -Wl  --dbgfile,BnB.dbg -Ln "bin\Buckets n Bombs.lbl" -m BnB.map  -I "include" -o $(TARGET) "src\Buckets n Bombs.c" "src\sprite_data.s"
	
clean:
	del /F $(DISK_NAME) $(TARGET)

run: $(DISK_NAME)
	$(VICE) $(DISK_NAME)
	
make_disk: $(DISK_NAME)

$(DISK_NAME): $(TARGET)
	$(1541) -format "$(TARGET_NAME),xx" d64 $(DISK_NAME)
	$(1541) -attach $(DISK_NAME) -delete $(TARGET_NAME).bin -write $(TARGET) -dir
