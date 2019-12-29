EE_BIN = vtstech.elf
EE_BIN_PACKED = vtstech-packed.ELF
EE_BIN_STRIPPED = vtstech-stripped.ELF
EE_OBJS = vtstech.o OSDInit.o libcdvd_add.o
EE_LIBS = -ldebug -lc -lcdvd -lpatches -lfileXio

all:
	$(MAKE) $(EE_BIN_PACKED)

clean:
	@echo "================"
	@echo "=== Cleaning ==="
	@echo "================"
	rm -fr *.ELF *.o *.bak

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

$(EE_BIN_STRIPPED): $(EE_BIN)
	@echo "================="
	@echo "=== Stripping ==="
	@echo "================="
	$(EE_STRIP) -o $@ $<
	
$(EE_BIN_PACKED): $(EE_BIN_STRIPPED)
	@echo "==================="
	@echo "=== Compressing ==="
	@echo "==================="
	~/ps2homebrew/ps2-packer/ps2-packer -v $< $@
	
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal