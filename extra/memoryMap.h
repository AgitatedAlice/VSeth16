#ifndef MEMORY_MAP_h
#define MEMORY_MAP_H

/* Memory map:

0x00: Zero Page, unregulated shared memory space, currently interrupt vectors are set in upper range of this, use for things that need to be accessed quickly
0x01: General Purpose User RAM
0x02: General Purpose User RAM
0x03: General Purpose User RAM
0x04: General Purpose User RAM
0x05: General Purpose User RAM
0x06: General Purpose User RAM
0x07: General Purpose User RAM
0x08: General Purpose User RAM
0x09: General Purpose User RAM
0x0A: General Purpose User RAM
0x0B: General Purpose User RAM
0x0C: General Purpose User RAM
0x0D: General Purpose User RAM
0x0E: General Purpose User RAM
0x0F: General Purpose User RAM
0x10: BOOT (ROM)
0x11: Screen
0x12: Peripheral Device Range (audio, keyboard, mouse, etc)
0x13: IDKLMAO
0x14: 
from here to 0x1F reserved all 12 pages of "flash" memory

Screen page memory map:
Upper word at FFFF determines mode of the screen, 0xFFFE contains screen opcode register, 0xFFFD contains screen status register
0 is monochrome lower-byte only 40x25 character text mode
1 is color 40x25 text mode
2 is 320x200 4bit color mode, one pixel per word, single buffer mode
3 is 320x200 256 color full 32 bit color palette mode with two buffers, one word is two pixels each color determined by palette index value, lower byte is right pixel, upper byte is left pixel

0x0000 -> 0xF9FF --- Frame buffer, single in mode 2, covers two in mode 3
0x0000 -> 0x7CFF --- Frame buffer A in mode 3
0x7D00 -> 0xF9FF --- Frame buffer B in mode 3
0xFA00 -> 0xFBFF --- 256 32 bit color palette for mode 3
0xFC00 -> 0xFFE8 --- Text Buffer for mode 0


TBD*/

#define SCREEN_INDEX 0x11
#define FRAME_BUFFER_A 0x7CFF

#enfif
