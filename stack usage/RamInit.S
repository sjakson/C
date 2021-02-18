; Assembly file for initializing RAM

; Assignment. Here is where you'll put your RAM initialization code.
; 1. Use the .section command to specify that this code is to be executed
;    before the data and bss initialization.
; 2. Fill the RAM, from 0x200 to 0x21FF, with 0xAA.

.section .init4,"ax",@progbits ; set memory section to execute before main

		ldi r17,0xAA ; load 0xAA into a register so it can be copied to RAM
		ldi r28,0xFF ; Set register Y to point to top of RAM 0x21FF. Set Y low byte
		ldi r29,0x21 ; Set Y high byte
		clr r26		 ; Clear X low byte
		ldi r27,0x02 ; Set X high byte to point to bottom of RAM 0x200
loop:	st X+,r17    ; Store 0xAA into mem address in RAM specified by X. Post increment X
		cp r27,r29   ; Compare high byte of X with high byte of Y
		brne loop    ; If X high byte and Y high byte not equal jump back to loop:
		cp r26,r28   ; Compare low byte of X with low byte of Y
		brne loop    ; If X high byte and Y high byte not equal jump back to loop:

.section .init9 ; jump to main