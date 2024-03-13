
PATH := /d/GBADev/DevKitPro_Good/devkitARM/bin:$(PATH)
PATH_LIB_GCC =  /d/GBADev/DevKitPro_Good/lib/gcc/arm-none-eabi/13.2.0/thumb

# name of application
NAME = "Cheesy Nip Clicker By JJDB210"

# convert graphics
GFX2GBA  := gfx2gba.exe
B2X      := b2x.exe

# assemble and link
PREFIX  := arm-none-eabi-
GCC      := $(PREFIX)gcc
LD      := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
AS      := $(PREFIX)as


# e-reader tools
NEVPK    := nevpk
NESAV    := neflmake
NEDCMAKE := nedcmake
NEDCENC  := nedcenc
NEDCBMP  := nedcbmp

all : sav bmp3

sav : main.sav

bmp3 : bmp list list2

bmp : main.vpk
	$(NEDCMAKE) -i $< -o "a" -type 2 -bin -fill 1

list: a.*.bin
	for file in $^; do \
		$(NEDCENC) -i $${file} -o $${file}.raw;\
	done
	
list2: a.*.bin.raw
	for file in $^; do \
		$(NEDCBMP) -dpi 300 -i $${file} -o $${file};  \
	done

main.sav : main.vpk
	$(NESAV) -i $< -o "$@" -type 2 -name $(NAME)

main.vpk : main.bin
	$(NEVPK) -i "$<" -o "$@" -c -level 1

main.bin : main.elf
	$(OBJCOPY) -O binary "$<" "$@"

main.elf : crt0.o main.o posprintf.o ereader.ld 
	$(LD) crt0.o main.o posprintf.o -lgcc -L $(PATH_LIB_GCC) -T ereader.ld -O3 -o "$@"

main.o : main.c background.bmp.raw.c background.bmp.pal.c background.bmp.map.c nip.bmp.raw.c nip.bmp.pal.c beta.bmp.raw.c beta.bmp.pal.c
	$(GCC) -mthumb -c -O3 -o "$@" "$<"

posprintf.o : posprintf.S
	$(AS) -o "$@" "$<"
	
crt0.o : crt0.s
	$(AS) -o "$@" "$<"

background.bmp.raw.c : background.bmp.raw
	$(B2X) -c -n background_gfx -t u8 < "$<" > "$@"

background.bmp.pal.c : background.bmp.pal
	$(B2X) -c -n background_pal -t u8 < "$<" > "$@"

background.bmp.map.c : background.bmp.map
	$(B2X) -c -n background_map -t u8 < "$<" > "$@"

background.bmp.raw : background.bmp
	$(GFX2GBA) -q -t8 -c16 -x -m -St.bmp.raw -D -P "$<"

background.bmp.pal : background.bmp
	$(GFX2GBA) -q -t8 -c16 -x -m -Sp.bmp.pal -D -G "$<"

background.bmp.map : background.bmp
	$(GFX2GBA) -q -t8 -c16 -x -m -Sm.bmp.map -G -P "$<"

beta.bmp.raw.c : beta.bmp.raw
	$(B2X) -c -n beta_gfx -t u8 < "$<" > "$@"
	
beta.bmp.pal.c : beta.bmp.pal
	$(B2X) -c -n beta_pal -t u8 < "$<" > "$@"

beta.bmp.raw : beta.bmp
	$(GFX2GBA) -q -t8 -c16 -x -St.bmp.raw -D -P "$<"

beta.bmp.pal : beta.bmp
	$(GFX2GBA) -q -t8 -c16 -x -Sp.bmp.pal -D -G "$<"

nip.bmp.raw.c : nip.bmp.raw
	$(B2X) -c -n nip_gfx -t u8 < "$<" > "$@"
	
nip.bmp.pal.c : nip.bmp.pal
	$(B2X) -c -n nip_pal -t u8 < "$<" > "$@"

nip.bmp.raw : nip.bmp
	$(GFX2GBA) -q -t8 -c16 -x -St.bmp.raw -D -P "$<"

nip.bmp.pal : nip.bmp
	$(GFX2GBA) -q -t8 -c16 -x -Sp.bmp.pal -D -G "$<"


clean :
	rm -f main.*.bin main.*.raw main.*.bmp
	rm -f main.elf main.map main.bin main.vpk main.sav
	rm -f main.o crt0.o
	rm -f background.bmp.*
