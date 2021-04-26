# ---------------------------------------------------------------
# https://www.rapidtables.com/code/linux/gcc/gcc-l.html <- how to link libs

CC := gcc

C_FLAGS :=

I_FLAGS :=
I_FLAGS += -I./inc
I_FLAGS += -I./lib/c_doc

L_FLAGS :=
L_FLAGS += -L./lib
L_FLAGS += ./lib/c_doc/libdoc.a
L_FLAGS += -lpthread
L_FLAGS += -lz

SOURCES := main.c 
MAIN_APP := main.exe
BUILD_DIR := build/

# ---------------------------------------------------------------

OBJS := $(SOURCES:.c=.o)

OBJS_BUILD := $(addprefix $(BUILD_DIR), $(notdir $(SOURCES:.c=.o)))

# ---------------------------------------------------------------

.PHONY : build

build : C_FLAGS += -g
build : $(MAIN_APP)

release : C_FLAGS += -O3
release : $(MAIN_APP)

$(MAIN_APP) : $(OBJS) $(RES_OUT)
	$(CC) $(OBJS_BUILD) $(RES_OUT) $(L_FLAGS) -o $@

%.o : %.c
	$(CC) $(C_FLAGS) $(I_FLAGS) -c $< -o $(addprefix $(BUILD_DIR), $(notdir $@))

clear : 
	@rm $(MAIN_APP)
	@rm -f $(OBJS_BUILD)
	@rm -f $(RES_OUT)
