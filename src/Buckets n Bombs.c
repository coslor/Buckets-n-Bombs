/* Buckets n Bombs */

#include <stdio.h>
#include <stdlib.h>
#include <c64.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>
#include "Buckets n Bombs.h"


/* The VIC pointer to data for sprite 0 */
unsigned char* SPRITE0_POINTER=(unsigned char*)0x7f8;

/* The buffer holding sprite 0 data */
//unsigned char* SPRITE0_BUFFER=(unsigned char*)0x3000;
//unsigned char SPRITE0_BUFFER[63];

//TODO This buffer is allocated in sprite_data.s at $3000, which is somewhere in the middle of BASIC memory space. I need to figure out a rational memory map.
//extern unsigned char SPRITE0_BUFFER[];

//stylized tennis shoe - 
//extern const unsigned char *shoe_img;


void main(void) {
	//int i;
	//int x,y, xInc, yInc;
	//int sprite_ratio;	
	sprite t = {
		0,			//VIC sprite num
		{0,0},		//loc
		{0,0},		//speed
		{0,1,2,3},	//color
		TRUE,		//multicolor?
		FALSE,		//expand x?
		FALSE,		//expand y?
		TRUE		//enabled?
		//(unsigned char*)bomb_img1	//image
	 };
	 t.image_buffer = bomb_img1;

	sprites[0] = t;
	
	
	
	
	//init_sprite();
	clrscr();
	//printf("Hello C64! shoe buffer=%x\n", shoe_img);
		
	//bufferInt = SPRITE0_BUFFER;
	//sprite_ratio = (int)shoe_img / 64;

	////TODO fix warning here: 
	//copy_sprite(SPRITE0_BUFFER, bomb_img1);
	

	//*SPRITE0_POINTER = sprite_ratio;
	
	//printf("SPRITE0_POINTER[0] should contain %d, does contain %d\n", sprite_ratio, *SPRITE0_POINTER);
	
	
	// VIC.spr_hi_x = 0;
	// VIC.spr0_x = 100;
	// VIC.spr0_y = 100;
	
	// printf("Turning sprite on...\n");
	
	// //Enable sprite 0
	// VIC.spr_ena |= 1;
	
	// xInc=1;
	// yInc=-1;
	// x=24;
	// y=100;
	
	// while (TRUE) {

		// main_loop(&x, &y);
		// delay(50);
	// }
/* 	for (i=0; i < 255; i++) {
		VIC.spr0_x = i;

		for (j=0; j < 100; j++) {
		}
 	}
*/	
	
	
}

void copy_sprite_image(unsigned char sprite_image_buffer[], unsigned char image_data[]) {

	memcpy(sprite_image_buffer, image_data, 63);
	
}



void main_loop(int *x, int *y) {
	
	static int xInc=1;
	static int yInc=1;

	*x += xInc;
	*y += yInc;
	
	if ((*x <= 24) || (*x >= 320)) {
		xInc *= -1;
	}
	
	if ((*y < 42) || (*y > 235)) {
		yInc *= -1;
	}
	
	VIC.spr_pos[0].x = (*x & 255);
	VIC.spr_pos[0].y = *y;
	
	if (*x > 255) {
		VIC.spr_hi_x = VIC.spr_hi_x | 1;
	}
	else {
		VIC.spr_hi_x = VIC.spr_hi_x & 254;
	}
	
}


void move_sprites(sprite sprites[]) {
	int i;
	sprite s;
	unsigned int vic_num;
	unsigned char pow2;
	
	for (i=0; i < NUM_SPRITES; i++) {
		s = sprites[i];
		if (s.enabled) {
			s.loc.x += s.speed.x;
			s.loc.y += s.speed.y;
			
			vic_num = s.VIC_sprite_num;
			VIC.spr_pos[vic_num].x = (s.loc.x & 255);
			VIC.spr_pos[vic_num].y = s.loc.y;
			
			/* 
				The sprite's x coord is a 9-bit value, and the top bit is held in spr_hi_x.
				To set it, we need to set (or unset) the bit corresponding to VIC's sprite #.
			*/
			if (s.loc.x > 255) {
					pow2 = 2 << vic_num;
				VIC.spr_hi_x = VIC.spr_hi_x | pow2;
			}
			else {
				VIC.spr_hi_x = VIC.spr_hi_x & (255 - pow2);
			}
			
		}
	}
}


void delay(int time) {
	int i;
	
	for (i=0; i < time; i++) {
		
	}
}

void set_sprite_image_handle(sprite *s, unsigned char *image_buffer) {
	//TODO deal with bank switching, etc. here
	int sprite_addr_ratio = (int)image_buffer / 64;

	*(SPRITE0_POINTER + s->VIC_sprite_num) = sprite_addr_ratio;

	s->image_buffer = image_buffer;

}

void init_sprite(sprite *spr, int x, int y, BOOL active,
		BOOL multicolor, unsigned char color0, unsigned char color1, unsigned char color2, unsigned char color3,
		BOOL expand_x, BOOL expand_y, unsigned char *image_buffer) {
	
	spr->loc.x = x;
	spr->loc.y = y;
	
	spr->loc.x = x;
	spr->loc.y = y;

	spr->expand_x = expand_x;
	spr->expand_y = expand_y;
	
	
	//Now, do the necessary VIC2 stuff for this sprite
	set_sprite_image_handle(spr, image_buffer);
	enable_sprite(spr, (BOOL)active);
	set_sprite_color(spr, multicolor, color0, color1, color2, color3);	
}


unsigned int set_bit(unsigned int start_value, int bit_num, BOOL bool_value) {
	unsigned int final_value;
	
	int pwr2 = 2 << bit_num;
	
	if (bool_value) {
		final_value = start_value | pwr2;
	}
	else {
		final_value = start_value & (255 - pwr2);
	}
	return final_value;
}


void enable_sprite(sprite *s, BOOL enabled) {
	VIC.spr_ena = set_bit(VIC.spr_ena, s->VIC_sprite_num, enabled);
	s->enabled = enabled;
}


void set_multicolor(sprite *s, BOOL multicolor) {
	VIC.spr_mcolor = set_bit(VIC.spr_mcolor, s->VIC_sprite_num, multicolor);
	s->multicolor = multicolor;
}


void set_sprite_color(sprite *s, BOOL multicolor, 
		unsigned char color0, unsigned char color1, unsigned char color2, unsigned char color3) {
	s->multicolor = multicolor;
	
	s->color[0] = color0;
	s->color[1] = color1;
	s->color[2] = color2;
	s->color[3] = color3;

	VIC.spr_mcolor = set_bit(VIC.spr_mcolor, s->VIC_sprite_num, multicolor);
	
	//Set x and y coords to 100
	*(unsigned char*)(VIC.spr0_color + s->VIC_sprite_num) = color1;
	VIC.spr_mcolor0 = color2;
	VIC.spr_mcolor1 = color3;
	
}


void set_image(sprite *s, unsigned char* buffer) {
	
}


void update_sprite(sprite *s) {
	
}

