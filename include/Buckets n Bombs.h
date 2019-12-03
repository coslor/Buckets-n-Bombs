#define BOOL unsigned char
#define TRUE 1
#define FALSE 0
#define NUM_SPRITES 8


//bucket
extern const unsigned char *bucket_img;

//first frame of bomb animation
extern  unsigned char *bomb_img1;
		
typedef unsigned char color;

typedef struct  vector2 {
	unsigned int		x;
	unsigned int		y;
} vector2;


typedef struct  {
	unsigned int 	VIC_sprite_num;
	
	vector2	loc;
	vector2	speed;
	
	//TODO This doesn't make sense. Each sprite only has 2 unique colors.
	unsigned char	color[4];
	BOOL	multicolor;
	
	BOOL	expand_x;
	BOOL	expand_y;
	
	BOOL	enabled;
	
	unsigned char	*image_buffer;
} sprite;


int x[NUM_SPRITES];

sprite  sprites[NUM_SPRITES];


void delay(int time);
void main_loop(int *x, int *y);
void copy_sprite(unsigned char sprite_buffer[], unsigned char image_data[]);
void enable_sprite(sprite *s, BOOL enabled);
void set_multicolor(sprite *s, BOOL multicolor);
void set_sprite_color(sprite *s, BOOL multicolor, 
		unsigned char color0, unsigned char color1, unsigned char color2, unsigned char color3);
void set_image(sprite *s, unsigned char *buffer);


