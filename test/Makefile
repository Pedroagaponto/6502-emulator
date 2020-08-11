TOPTARGETS := all clean

RMDIR = rm -rf

SRC_PATH = asm
OBJ_PATH = obj
BIN_PATH = bin

CODE_FILES = $(wildcard $(SRC_PATH)/*.s)
OBJ_FILES = $(addprefix $(OBJ_PATH)/,$(notdir $(CODE_FILES:.s=.o)))
BIN_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(OBJ_FILES:.o=.nes)))

COMPILER = ca65
LINKER = ld65

DIRECTIVES = -t nes
CONFIG = -C nes.cfg

all:$(BIN_FILES)


$(OBJ_PATH)/%.o: $(SRC_PATH)/%.s
	@mkdir -p $(OBJ_PATH)
	$(COMPILER) $< -o $@ 
	
$(BIN_PATH)/%.nes: $(OBJ_PATH)/%.o
	@mkdir -p $(BIN_PATH)
	$(LINKER) $< -o $@ $(DIRECTIVES)


clean:
	$(RMDIR) $(BIN_PATH) $(OBJ_PATH)	
