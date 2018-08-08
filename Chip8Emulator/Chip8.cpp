#include "Chip8.h"
#include <iostream>
#include <random>


Chip8::Chip8()
{
}


Chip8::~Chip8()
{
}

void Chip8::initialize()
{
	pc	= 0x200; // Program start at 0x200 in memory
	opcode	= 0;	 // reset variables below
	I	= 0;
	sp	= 0;

	// Clear display	
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	// Reset timers

	// load
}


void Chip8::emulateCycle()
{
	// Fetching opcode
	opcode = memory[pc] << 8 | memory[pc + 1];

	// Decoding opcode
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 0x00E0: Clears the screen        
			     // Execute opcode
			break;

		case 0x000E: // 0x00EE: Returns from subroutine
			--sp;
			pc = stack[sp];
			break;

		default:
			printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			break;
		}
		break;

	case 0x1000: // 0x1NNN: Jumps to address NNN.
		pc = opcode & 0x0FFF;
		break;

	case 0x2000: // 0x2NNN: Calls subroutine at NNN
		stack[sp] = pc;
		++sp;
		pc = opcode & 0x0FFF;
		break;

	case 0x3000: // 0x3XNN: Skips the next instruction if VX equals NN. 
		if (V[(opcode & 0x0F00) >> 8] == opcode & 0x00FF)
			pc += 4;
		else
			pc += 2;
		break;

	case 0x4000: // 0x4XNN: Skips the next instruction if VX doesn't equal NN
		if (V[(opcode & 0x0F00) >> 8] != opcode & 0x00FF)
			pc += 4;
		else
			pc += 2;

		break;

	case 0x5000: // 0x5XY0: Skips the next instruction if VX equals VY.
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			pc += 4;
		else
			pc += 2;
		break;

	case 0x6000: // 0x6VNN: Sets VX to NN.
		V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		pc += 2;
		break;

	case 0x7000: // 0x7XNN: Adds NN to VX. (Carry flag is not changed)
		V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
		pc += 2;
		break;

	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000: // 0x8XY0: Sets VX to the value of VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0001: // 0x8XY1: Sets VX to VX or VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] | V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;

		case 0x0002: // 0x8XY2: Sets VX to VX and VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] & V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;


		case 0x0003: // 0x8XY3: Sets VX to VX xor VY.
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] ^ V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;

		case 0x0004: // 0x8XY4: Adds VY to VX. VF is set to 1 when there's a carry
			if (V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
				V[0xF] = 1; //carry
			else
				V[0xF] = 0;
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0005: // VY is subtracted from VX. VF is set to 0 when 
			     //there's a borrow, and 1 when there isn't.
			if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4])
				V[0xF] = 0; // borrow
			else
				V[0xF] = 1;
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0006: // 0x8XY6: Stores the least significant bit of 
			     // VX in VF and then shifts VX to the right by 1.
			V[0xF] = V[(opcode & 0x0F00) >> 8] & 0b0000'0001;
			V[(opcode & 0x0F00) >> 8] >> 1;
			pc += 2;
			break;

		case 0x0007: // 0x8XY7: Sets VX to VY minus VX. VF is set to 0 when 
			     // there's a borrow, and 1 when there isn't.
			if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8])
				V[0xF] = 0; // borrow
			else
				V[0xF] = 1;
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;

		case 0x000E: // 0x8XY7: Stores the most significant bit of VX in VF 
			     // and then shifts VX to the left by 1.
			V[0xF] = V[(opcode & 0x0F00) >> 8] & 0b1000'0000;
			V[(opcode & 0x0F00) >> 8] << 1;
			pc += 2;
			break;

		default:
			printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
			break;
		}

	case 0x9000: // 0x9XY0: Skips the next instruction if VX doesn't equal VY.
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
			pc += 4;
		else
			pc += 2;
		break;

	case 0xA000: // ANNN: Sets I to the address NNN
		I = opcode & 0x0FFF;
		pc += 2;
		break;

	case 0xB000: // 0xBNNN: Jumps to the address NNN plus V0.
		pc = (opcode & 0x0FFF) + V[0x0];
		break;

	case 0xC000: // 0xCXNN: Sets VX to the result of a bitwise 
		     // and operation on a random number (Typically: 0 to 255) and NN.
		std::random_device random_device; 
		std::mt19937 generator(random_device()); 
		std::uniform_int_distribution<> distribution(0, 255); 
		int rand = distribution(generator); // generate random number

		V[(opcode & 0x0F00) >> 8] = rand & (opcode & 0x00FF);
		pc += 2;
		break;

	case 0xD000: // Draws a sprite at coordinate (VX, VY) that 
		     // has a width of 8 pixels and a height of N pixels.
		unsigned short x = V[(opcode & 0x0F00) >> 8];
		unsigned short y = V[(opcode & 0x00F0) >> 4];
		unsigned short height = opcode & 0x000F;
		unsigned short pixel;

		V[0xF] = 0;
		for (int yline = 0; yline < height; yline++)
		{
			pixel = memory[I + yline];
			for (int xline = 0; xline < 8; xline++)
			{
				if ((pixel & (0x80 >> xline)) != 0)
				{
					if (gfx[(x + xline + ((y + yline) * 64))] == 1)
						V[0xF] = 1;
					gfx[x + xline + ((y + yline) * 64)] ^= 1;
				}
			}
		}

		drawFlag = true;
		pc += 2;
		break;

	case 0xE000:
		switch (opcode & 0x00FF)
		{
			// EX9E: Skips the next instruction 
			// if the key stored in VX is pressed
		case 0x009E:
			if (key[V[(opcode & 0x0F00) >> 8]] != 0)
				pc += 4;
			else
				pc += 2;
			break;

		case 0x00A1: // 0xEXA1: Skips the next instruction if 
			     // the key stored in VX isn't pressed. 
			if (key[V[(opcode & 0x0F00) >> 8]] == 0)
				pc += 4;
			else
				pc += 2;
			break;

		default:
			printf("Unknown opcode [0xE000]: 0x%X\n", opcode);
			break;
		}

	case 0xF000:
		switch (opcode & 0x00FF) {
		case 0x0007: // 0xFX07: Sets VX to the value of the delay timer.
			V[(opcode & 0x0F00) >> 8] = delay_timer;
			pc += 2;
			break;

		case 0x0015: // 0xFX15: Sets the delay timer to VX. 
			delay_timer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;

		case 0x0033: // Stores the Binary-coded decimal representation of VX 
			     // at the addresses I, 
			     // I plus 1, and I plus 2
			memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			pc += 2;
			break;

		default:
			printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
			break;
		}
		break;


		// Updating timers
		if (delay_timer > 0)
			--delay_timer;

		if (sound_timer > 0)
		{
			if (sound_timer == 1)
				printf("BEEP!\n");
			--sound_timer;
		}
	}
}


