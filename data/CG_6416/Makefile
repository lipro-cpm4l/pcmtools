all: suball hex xpm hex_bin

suball:

hex:	original.hex \
	original_inv.hex

xpm:	original.xpm \
	original_inv.xpm

hex_bin:original.bin \
	original_inv.bin

bin_hex:

clean:
	rm -f *.bin
	rm -f *.hex
	rm -f *.xpm

.SUFFIXES:
.SUFFIXES: .bin .hex .xpm

%_inv.hex:
	mkcg.6416 --verbose --hexdump --neg $(subst _inv,,$*)/*.xpm > $@

%.hex:
	mkcg.6416 --verbose --hexdump $*/*.xpm > $@

%_inv.xpm:
	mkcg.6416 --quiet --overview=32 --neg $(subst _inv,,$*)/*.xpm > $@

%.xpm:
	mkcg.6416 --quiet --overview=32 $*/*.xpm > $@

.hex.bin:
	xxd -r -c 8 $< > $@

.bin.hex:
	xxd -g 1 -c 8 -u $< > $@

