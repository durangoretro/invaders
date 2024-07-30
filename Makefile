BUILD_DIR=bin
SOURCE_DIR=src
RES_DIR=res
DDK?=../DurangoLib
COMPILER_OPT= -I $(DDK)/inc -I inc -I res --cpu 6502
ASM_OPT = -t none
LINKER_OPT= -C $(DDK)/cfg/durango.cfg
LINKER_LIBS=  $(DDK)/bin/durango.lib
RESCOMP?=../rescomp/target/rescomp.jar

all: rom.dux

$(RES_DIR)/playerSptr.h: $(RES_DIR)/player.png
	java -jar ${RESCOMP} -n playerSptr -m SPRITESHEET -w 16 -h 8 -i $(RES_DIR)/player.png -o $(RES_DIR)/playerSptr.h
$(RES_DIR)/redSptr.h: $(RES_DIR)/red.png
	java -jar ${RESCOMP} -n redSptr -m SPRITESHEET -w 10 -h 8 -i $(RES_DIR)/red.png -o $(RES_DIR)/redSptr.h
$(RES_DIR)/greenSptr.h: $(RES_DIR)/green.png
	java -jar ${RESCOMP} -n greenSptr -m SPRITESHEET -w 10 -h 8 -i $(RES_DIR)/green.png -o $(RES_DIR)/greenSptr.h 
$(RES_DIR)/yellowSptr.h: $(RES_DIR)/yellow.png
	java -jar ${RESCOMP} -n yellowSptr -m SPRITESHEET -w 10 -h 8 -i $(RES_DIR)/yellow.png -o $(RES_DIR)/yellowSptr.h  
$(RES_DIR)/bulletSptr.h: $(RES_DIR)/bullet.png
	java -jar ${RESCOMP} -n bulletSptr -m SPRITESHEET -w 2 -h 6 -i $(RES_DIR)/bullet.png -o $(RES_DIR)/bulletSptr.h  
$(BUILD_DIR)/invaders.s: $(RES_DIR)/playerSptr.h $(RES_DIR)/bulletSptr.h $(RES_DIR)/redSptr.h $(RES_DIR)/greenSptr.h $(RES_DIR)/yellowSptr.h $(SOURCE_DIR)/main.c
	cc65 $(COMPILER_OPT) -o $(BUILD_DIR)/invaders.s $(SOURCE_DIR)/main.c 
$(BUILD_DIR)/invaders.o: $(BUILD_DIR)/invaders.s
	ca65 $(ASM_OPT) $(BUILD_DIR)/invaders.s -o $(BUILD_DIR)/invaders.o
invaders.bin: $(BUILD_DIR)/ $(BUILD_DIR)/invaders.o 
	ld65 $(LINKER_OPT) $(BUILD_DIR)/invaders.o  $(LINKER_LIBS) -o invaders.bin
rom.dux: invaders.bin
	java -jar ${RESCOMP} -m SIGNER -n Invaders -t INVADERS -d "Invaders From Space" -i invaders.bin -o rom.dux

$(BUILD_DIR)/:
	mkdir -p $(BUILD_DIR)

clean:
	rm -Rf $(BUILD_DIR)/ invaders.bin rom.dux $(RES_DIR)/*.h
