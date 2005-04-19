all: suball hex xpm hex_bin

suball:

hex:	original.hex \
	CAOS_de.hex \
	CAOS_uk.hex \
	K8911.hex \
	SLZ_I.hex

xpm:	original.xpm \
	CAOS_de.xpm \
	CAOS_uk.xpm \
	K8911.xpm \
	SLZ_I.xpm

hex_bin:	original.bin \
		CAOS_de.bin \
		CAOS_uk.bin \
		K8911.bin \
		SLZ_I.bin

bin_hex:

clean:
	rm -f *.bin
	rm -f *.hex
	rm -f *.xpm

.SUFFIXES:
.SUFFIXES: .bin .hex .xpm

%.hex:
	mkzg.vid2k -xv $*/*.xpm > $@

%.xpm:
	mkzg.vid2k -qo 32 $*/*.xpm > $@

.hex.bin:
	xxd -r -c 16 $< > $@

.bin.hex:
	xxd -g 1 -c 16 -u $< > $@

