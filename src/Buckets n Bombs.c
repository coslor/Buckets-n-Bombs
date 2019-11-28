/* Buckets n Bombs */

#include <stdio.h>
#include <stdlib.h>
#include <c64.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>

#define BOOL char
#define TRUE 1
#define FALSE 0

/* The VIC pointer to data for sprite 0 */
unsigned char* SPRITE0_POINTER=(unsigned char*)0x7f8;

/* The buffer holding sprite 0 data */
//unsigned char* SPRITE0_BUFFER=(unsigned char*)0x3000;
//unsigned char SPRITE0_BUFFER[63];

//TODO This buffer is allocated in sprite_data.s at $3000, which is somewhere in the middle of BASIC memory space. I need to figure out a rational memory map.
//extern unsigned char SPRITE0_BUFFER[];

//stylized tennis shoe - 
extern const unsigned char *shoe_img;

//bucket
extern const unsigned char *bucket_img;

//first frame of bomb animation
extern const unsigned char *bomb_img1;
		
typedef unsigned char color;

typedef struct  {
	int		x;
	int		y;
} vector2;

typedef struct  {
	vector2	loc;
	vector2	speed;
	
	unsigned char	color[4];
	BOOL	multicolor;
	
	//vector2	expand;
	BOOL	expand_x;
	BOOL	expand_y;
	
	BOOL	enabled;
	
	unsigned char	*image_buffer;
} sprite;
sprite  sprites[8];


void delay(int time);
void main_loop(int* x, int* y);
void copy_sprite(unsigned char sprite_buffer[], unsigned char image_data[]);
void enable_sprite(int sprite_num, BOOL enabled);
void enable_multicolor(int sprite_num, BOOL multicolor);
void set_sprite_color(int sprite_num, BOOL multicolor, 
		unsigned char color0, unsigned char color1, unsigned char color2, unsigned char color3);
void set_image(sprite* s, unsigned char* buffer);


void main(void) {
	int i;
	int x,y, xInc, yInc;
	int sprite0Ratio;

	clrscr();
	printf("Hello C64! shoe buffer=%x\n", shoe_img);
		
	//bufferInt = SPRITE0_BUFFER;
	sprite0Ratio = (int)shoe_img / 64;

	////TODO fix warning here: 
	//copy_sprite(SPRITE0_BUFFER, bomb_img1);
	

	*SPRITE0_POINTER = sprite0Ratio;
	
	printf("SPRITE0_POINTER[0] should contain %d, does contain %d\n", sprite0Ratio, *SPRITE0_POINTER);
	
	
	VIC.spr_hi_x = 0;
	VIC.spr0_x = 100;
	VIC.spr0_y = 100;
	
	printf("Turning sprite on...\n");
	
	//Enable sprite 0
	VIC.spr_ena |= 1;
	
	xInc=1;
	yInc=-1;
	x=24;
	y=100;
	
	while (TRUE) {

		main_loop(&x, &y);
		delay(50);
	}
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



void main_loop(int* x, int* y) {
	
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

void delay(int time) {
	int i;
	
	for (i=0; i < time; i++) {
		
	}
}

void set_sprite_image_handle(int sprite_num, unsigned char* image_buffer) {
	//TODO deal with paging here
	int sprite0Ratio = (int)image_buffer / 64;

	*(SPRITE0_POINTER + sprite_num) = sprite0Ratio;

	sprites[sprite_num].image_buffer = image_buffer;

}

void init_sprite(int sprite_num, int x, int y, BOOL active,
		BOOL multicolor, unsigned char color0, unsigned char color1, unsigned char color2, unsigned char color3,
		BOOL expand_x, BOOL expand_y, unsigned char* image_buffer) {
	sprites[sprite_num].loc.x = x;
	sprites[sprite_num].loc.y = y;
	

	sprites[sprite_num].expand_x = expand_x;
	sprites[sprite_num].expand_y = expand_y;
	
	
	//Now, do the necessary VIC2 stuff for this sprite
	set_sprite_image_handle(sprite_num, image_buffer);
	enable_sprite(sprite_num, active);
	set_sprite_color(sprite_num, multicolor, color0, color1, color2, color3);
	
	
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
void enable_sprite(int sprite_num, BOOL enabled) {
	VIC.spr_ena = set_bit(VIC.spr_ena, sprite_num, enabled);
	sprites[sprite_num].enabled = enabled;
}

void enable_multicolor(int sprite_num, BOOL multicolor) {
	VIC.spr_mcolor = set_bit(VIC.spr_mcolor, sprite_num, multicolor);
}

void set_sprite_color(int sprite_num, BOOL multicolor, 
		unsigned char color0, unsigned char color1, unsigned char color2, unsigned char color3) {
	sprites[sprite_num].multicolor = multicolor;
	
	sprites[sprite_num].color[0] = color0;
	sprites[sprite_num].color[0] = color1;
	sprites[sprite_num].color[0] = color2;
	sprites[sprite_num].color[0] = color3;

	VIC.spr_mcolor |= 1;
	
	//Set x and y coords to 100
	VIC.spr0_color = COLOR_BLACK;
	VIC.spr_mcolor0 = COLOR_ORANGE;
	VIC.spr_mcolor1 = COLOR_YELLOW;
	
}
void set_image(sprite* s, unsigned char* buffer) {
	
}
void update_sprite(sprite *sp) {
	
}

