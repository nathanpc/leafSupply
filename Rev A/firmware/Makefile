NAME = leafsupply-firmware
DEVICE= msp430g2553
PROJECT = $(NAME)-$(DEVICE)
CC = msp430-gcc -std=c99 -Os -mmcu=$(DEVICE)
MSPDEBUG = sudo mspdebug rf2500
SRC = main.c rotary_encoder.c 74HC595.c HD44780.c delay.c bitio.c ftoa.c voltage_regulators.c

all: $(PROJECT).elf

$(PROJECT).elf: $(SRC)
	$(CC) $(SRC) -o $(PROJECT).elf

write: $(PROJECT).elf
	$(MSPDEBUG) "prog $(PROJECT).elf"

run: $(PROJECT).elf
	$(MSPDEBUG) "prog $(PROJECT).elf" "reset" "run"

reset:
	$(MSPDEBUG) "reset"

erase:
	$(MSPDEBUG) "erase"

clean:
	rm -f *.elf
