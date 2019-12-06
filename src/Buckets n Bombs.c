/* Buckets n Bombs */

#include <stdio.h>
#include <stdlib.h>
#include <c64.h>
#include <peekpoke.h>
#include <conio.h>
#include <string.h>
#include "Buckets n Bombs.h"


/* The VIC pointer to data for sprite 0 */
unsigned char* SPRITE0_POINTER=(unsigned char*)0x07f8;

int sprite_ratio;

void main(void) {
	int i,j;
	unsigned char colors[4] = { COLOR_RED, COLOR_BLACK, COLOR_YELLOW, COLOR_BLACK };
	
	clrscr();
	
	printf("bucket_img=%p bomb_img1=%p\n", bucket_img, bomb_img1);
	
	printf("bucket_img=%d\n", bucket_img);

	
	init_sprite(&sprites[0],
		0,				//VIC sprite num
		100, 100, 		//x,y location
		10, 10,
		TRUE, 			//enabled
		TRUE,			//multicolor?
		COLOR_BLACK, 	//colors
		FALSE, 			//expand x?
		FALSE, 			//expand y?
		(unsigned char *)bomb_img1
		);

	VIC.spr_mcolors[0] = COLOR_RED;
	VIC.spr_mcolors[1] = COLOR_YELLOW;
	
	while (TRUE) {
		move_sprites(sprites);
		
		for (i=0; i < NUM_SPRITES; i++) {
			if ((sprites[i].loc.x < 24) || (sprites[i].loc.x > 343)){
				sprites[i].speed.x *= -1;
			}
			if ((sprites[i].loc.y < 30) || (sprites[i].loc.y > 229)) {
				sprites[i].speed.y *= -1;
			}
		}//for
	}//while
}

void copy_sprite_image(unsigned char sprite_image_buffer[], unsigned char image_data[]) {

	memcpy(sprite_image_buffer, image_data, 63);
	
}


void move_sprites(sprite sprites[]) {
	int i;
	sprite s;
	
	printf("move_sprites()\n");
	
	for (i=0; i < NUM_SPRITES; i++) {
		s = sprites[i];
		if (s.enabled) {
			printf("move_sprites(%d %d %d)\n", i, s.speed.x, s.speed.y);
			
			s.loc.x = s.loc.x + s.speed.x;
			s.loc.y = s.loc.y + s.speed.y;
			
		set_VIC_sprite_location(&s);
		}
	}
}

void set_VIC_sprite_location(sprite *s) {
	unsigned char pow2;
	int vic_num = s->VIC_sprite_num;
	
	printf("set_VIC_sprite_location()%d, %d, %d\n", s->VIC_sprite_num, s->loc.x, s->loc.y);

	VIC.spr_pos[vic_num].x = (s->loc.x & 255);
	VIC.spr_pos[vic_num].y = s->loc.y;
		
	/* 
		The sprite's x coord is a 9-bit value, and the top bit is held in spr_hi_x.
		To set it, we need to set (or unset) the bit corresponding to VIC's sprite #.
	*/
	if (s->loc.x > 255) {
			pow2 = 2 << vic_num;
		VIC.spr_hi_x = VIC.spr_hi_x | pow2;
	}
	else {
		VIC.spr_hi_x = VIC.spr_hi_x & (255 - pow2);
	}

}

void delay(int time) {
	int i;
	
	for (i=0; i < time; i++) {
		
	}
}

void set_sprite_image_handle(sprite *s, unsigned char *image_buffer) {
	//TODO deal with bank switching, etc. here
	
	
	int i;
	int sprite_addr_ratio;
	//int sprite_addr_ratio = 192; //((int)image_buffer / 64);


	//image_buffer = (unsigned char *)(0x340);

	sprite_addr_ratio = (int)image_buffer / 64;
	
	printf("sprite:%p image_buffer:%p\n", s, image_buffer);
	// for (i=0; i < 63; i++) {
		// //*(image_buffer + i) = 0xff;
		// printf("%x ", *(image_buffer + i));
	// }
	// printf("\n");
	


	SPRITE0_POINTER[s->VIC_sprite_num] = sprite_addr_ratio;

	printf("set_sprite_image_handle(%p, %p):ratio=%d\n", s, image_buffer, sprite_addr_ratio);

}

void init_sprite(sprite *spr, int VIC_sprite_num, 
		int x, int y, 
		int x_speed, int y_speed,
		BOOL active,
		BOOL multicolor, unsigned char sprite_color, 
		BOOL expand_x, BOOL expand_y, unsigned char *image_buffer) {
	
	spr->VIC_sprite_num = VIC_sprite_num;
	
	spr->loc.x = x;
	spr->loc.y = y;
	
	spr->speed.x = x_speed;
	spr->speed.y = y_speed;

	spr->expand_x = expand_x;
	spr->expand_y = expand_y;
	
	
	printf("init_sprite():image_buffer = %x\n", image_buffer);
	
	//Now, do the necessary VIC2 stuff for this sprite
	set_sprite_image_handle(spr, image_buffer);
	set_sprite_color(spr, multicolor, sprite_color);	
	set_VIC_sprite_location(spr);
	enable_sprite(spr, active);
}


/* Returns the value with the bit set. Does NOT set the bit in memory! */
unsigned int set_bit(unsigned int start_value, int bit_num, BOOL bool_value) {
	unsigned int final_value;
	
	int pwr2 = 1 << bit_num;

	printf("set_bit(%d, %d,%d)\n", start_value, bit_num, bool_value);
	
	
	printf("pwr2=%d\n", pwr2);
	
	if (bool_value) {
		final_value = start_value | pwr2;
	}
	else {
		final_value = start_value & (255 - pwr2);
	}
	
	printf("final_value=%d\n", final_value);
		
	return final_value;
}


void enable_sprite(sprite *s, BOOL enabled) {
	//TODO FIX
	//VIC.spr_ena = 1; 
	VIC.spr_ena = set_bit(VIC.spr_ena, s->VIC_sprite_num, enabled);
	s->enabled = enabled;
	
	printf("enable_sprite()\n");
}


void set_multicolor(sprite *s, BOOL multicolor) {
	VIC.spr_mcolor = set_bit(VIC.spr_mcolor, s->VIC_sprite_num, multicolor);
	s->multicolor = multicolor;
	
	printf("set_multicolor()\n");
}


void set_sprite_color(sprite *s, BOOL multicolor, 
		unsigned char sprite_color) {
	s->multicolor = multicolor;
	
	s->sprite_color = sprite_color;
	
	printf("set_sprite_color(%d)\n", sprite_color);
	
	VIC.spr_mcolor = set_bit(VIC.spr_mcolor, s->VIC_sprite_num, multicolor);
	
	VIC.spr_color[s->VIC_sprite_num] = sprite_color;
	
	printf("set_Sprite_color\n");
}


void set_image(sprite *s, unsigned char* buffer) {
	
}


void update_sprite(sprite *s) {
	set_VIC_sprite_location(s);
	
}

