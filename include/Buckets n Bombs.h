#define BOOL unsigned char
#define TRUE 1
#define FALSE 0
#define NUM_SPRITES 8

#define SPRITE_BUFFER $3000

//bucket
extern const unsigned char bucket_img[];

//first frame of bomb animation
extern  const unsigned char bomb_img1[];
		
typedef unsigned char color;

//These need to be int's, as x can be > 255, and I'd rather not mix types here -- confusing!
typedef struct  vector2 {
	unsigned int		x;
	unsigned int		y;
} vector2;


typedef struct  {
	unsigned int 	VIC_sprite_num;
	
	vector2			loc;
	vector2			speed;
	
	unsigned char	sprite_color;
	BOOL			multicolor;
	
	BOOL			expand_x;
	BOOL			expand_y;
	
	BOOL			enabled;
	
	unsigned char	*image_buffer;
} sprite;


int x[NUM_SPRITES];

sprite  sprites[NUM_SPRITES];

void copy_sprite_image(unsigned char sprite_image_buffer[], unsigned char image_data[]);

void delay(int time);
void main_loop(int *x, int *y);
void copy_sprite(unsigned char sprite_buffer[], unsigned char image_data[]);
void enable_sprite(sprite *s, BOOL enabled);
void set_multicolor(sprite *s, BOOL multicolor);
void set_sprite_color(sprite *s, BOOL multicolor, 
		unsigned char sprite_color);
void set_image(sprite *s, unsigned char *buffer);

void init_sprite(sprite *spr, int VIC_sprite_num, int x, int y, 
		x_speed, y_speed, BOOL active,
		BOOL multicolor, unsigned char sprite_color,
		BOOL expand_x, BOOL expand_y, unsigned char *image_buffer);


void set_VIC_sprite_location(sprite *s);

void move_sprites(sprite sprites[]);

unsigned int set_bit(unsigned int start_value, int bit_num, BOOL bool_value);