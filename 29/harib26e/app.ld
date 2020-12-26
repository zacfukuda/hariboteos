OUTPUT_FORMAT("binary");

SECTIONS
{
	.head 0x0 : {
		LONG(128 * 1024) /* 0: Size of stack+.data+heap */
		LONG(0x69726148) /* 4: Signature "Hari" */
		LONG(0) /* 8: Size of mmarea */
		LONG(__stack) /* 12: Initial stack value, where .data is navigated to */
		LONG(SIZEOF(.data)) /* 16: Size of .data */
		LONG(LOADADDR(.data)) /* 20: ? */
		LONG(0xE9000000) /* 24: ? */
		LONG(HariMain - 0x20) /* 28: Entry Address */
		LONG(24 * 1024) /* heap(malloc) the starting address */
	}

	.text : { *(.text) }

	.data __stack : AT ( ADDR(.text) + SIZEOF(.text) ) {
		*(.data)
		*(.rodata*)
		*(.bss)
	}

	/DISCARD/ : { *(.eh_frame) }

}
