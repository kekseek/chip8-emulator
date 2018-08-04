#pragma once
// CPU core implementation

class Chip8
{
private:
	// CHIP-8 has 35 opcodes, which are all two bytes long
	unsigned short opcode;

	// CHIP-8 has 4K memory
	// The systems memory map :
	// 0x000 - 0x1FF - Chip 8 interpreter(contains font set in emu)
	// 0x050 - 0x0A0 - Used for the built in 4x5 pixel font set(0 - F)
	// 0x200 - 0xFFF - Program ROM and work RAM
	unsigned char memory[4096];

	// CPU registers:
	// The Chip 8 has 15 8-bit general purpose registers 
	// named V0,V1 up to VE. The 16th register is used  for 
	// the ‘carry flag’.
	unsigned char V[16];

	// There is an Index register I and a program counter (pc) 
	// which can have a value from 0x000 to 0xFFF
	unsigned short I;
	unsigned short pc;

	// The graphics of the Chip 8 are black and white and 
	// the screen has a total of 2048 pixels (64 x 32). 
	// This can easily be implemented using an array that 
	// hold the pixel state (1 or 0).
	unsigned char gfx[2048];


	// CHIP - 8 has two timers.They both count down at 60 hertz, 
	// until they reach 0.
	// Delay timer : This timer is intended to be used for timing the events of games.
	// Its value can be set and read.
	unsigned char delay_timer;
	// Sound timer : This timer is used for sound effects.
	// When its value is nonzero, a beeping sound is made.
	unsigned char sound_timer;

	// Stack and stack pointer
	unsigned char stack[16];
	unsigned char sp;

	// HEX-based keypad
	unsigned char key[16];

	unsigned char chip8_fontset[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

public:
	Chip8();
	~Chip8();

	// initialize registers and memory once
	void initialize();

	// Emulates one cycle of the Chip 8 CPU.During this cycle, 
	// the emulator will Fetch, Decode and Execute one opcode.
	void emulateCycle();
	bool drawFlag = false;
};
