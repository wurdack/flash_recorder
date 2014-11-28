O=obj
B=bin

LIBGCC = ${shell ${CC} ${CC_ARCH_FLAGS} -print-libgcc-file-name}
LIBC   = ${shell ${CC} ${CC_ARCH_FLAGS} -print-file-name=libc.a}
LIBM   = ${shell ${CC} ${CC_ARCH_FLAGS} -print-file-name=libm.a}

#-------------------------------------------------------------------------------

all: $(B)/test_flash

clean:
	rm -f $(O)/*
	rm -f $(B)/*

astyle:
	astyle --style=otbs --add-brackets --align-pointer=name  --keep-one-line-blocks *.[ch]

.PSEUDO: all clean astyle

#.SILENT:

#-------------------------------------------------------------------------------

main.c: sys.h flash.h
flash_emu.c: sys.h flash.h

$(O)/%.o: %.c
	-mkdir -p $(O)
	$(CC) $(CFLAGS) $(CDEFINES) -c $< -o $@

$(B)/test_flash: $(O)/main.o $(O)/flash_emu.o $(O)/encode_decode.o
	-mkdir -p $(B)
	$(LD) $^ -o $@ -lc -macosx_version_min 10.8
