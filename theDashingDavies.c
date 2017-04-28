/*
 * tiles.c
 * program which demonstraes tile mode 0
 */
#include <stdio.h>
/* include the image we are using */
#include "background.h"
//#include "text_background.h" // for text 

/* include the tile map we are using */
#include "map.h"
#include "map2.h"
#include "sprites.h"

/* the width and height of the screen */
#define WIDTH 240
#define HEIGHT 160

/* the three tile modes */
#define MODE0 0x00
#define MODE1 0x01
#define MODE2 0x02

/* enable bits for the four tile layers */
#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800

/* flags to set sprite handling in display control register */
#define SPRITE_MAP_2D 0x0
#define SPRITE_MAP_1D 0x40
#define SPRITE_ENABLE 0x1000

/* the control registers for the four tile layers */
volatile unsigned short* bg0_control = (volatile unsigned short*) 0x4000008;
volatile unsigned short* bg1_control = (volatile unsigned short*) 0x400000a;
volatile unsigned short* bg2_control = (volatile unsigned short*) 0x400000c;
volatile unsigned short* bg3_control = (volatile unsigned short*) 0x400000e;

/* palette is always 256 colors */
#define PALETTE_SIZE 256

/* there are 128 sprites on the GBA */
#define NUM_SPRITES 128

/* the display control pointer points to the gba graphics register */
volatile unsigned long* display_control = (volatile unsigned long*) 0x4000000;

/* the memory location which controls sprite attributes */
volatile unsigned short* sprite_attribute_memory = (volatile unsigned short*) 0x7000000;

/* the memory location which stores sprite image data */
volatile unsigned short* sprite_image_memory = (volatile unsigned short*) 0x6010000;

/* the address of the color palette */
volatile unsigned short* bg_palette = (volatile unsigned short*) 0x5000000;
volatile unsigned short* sprite_palette = (volatile unsigned short*) 0x5000200;

/* the button register holds the bits which indicate whether each button has
 * been pressed - this has got to be volatile as well
 */
volatile unsigned short* buttons = (volatile unsigned short*) 0x04000130;

/* scrolling registers for backgrounds */
volatile short* bg0_x_scroll = (unsigned short*) 0x4000010;
volatile short* bg0_y_scroll = (unsigned short*) 0x4000012;
volatile short* bg1_x_scroll = (unsigned short*) 0x4000014;
volatile short* bg1_y_scroll = (unsigned short*) 0x4000016;
volatile short* bg2_x_scroll = (unsigned short*) 0x4000018;
volatile short* bg2_y_scroll = (unsigned short*) 0x400001a;
volatile short* bg3_x_scroll = (unsigned short*) 0x400001c;
volatile short* bg3_y_scroll = (unsigned short*) 0x400001e;


/* the bit positions indicate each button - the first bit is for A, second for
 * B, and so on, each constant below can be ANDED into the register to get the
 * status of any one button */
#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3)
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)
#define BUTTON_UP (1 << 6)
#define BUTTON_DOWN (1 << 7)
#define BUTTON_R (1 << 8)
#define BUTTON_L (1 << 9)

/* the scanline counter is a memory cell which is updated to indicate how
 * much of the screen has been drawn */
volatile unsigned short* scanline_counter = (volatile unsigned short*) 0x4000006;

/* wait for the screen to be fully drawn so we can do something during vblank */
void wait_vblank( ) {
    /* wait until all 160 lines have been updated */
    while (*scanline_counter < 160) { }
}


/* this function checks whether a particular button has been pressed */
unsigned char button_pressed(unsigned short button) {
    /* and the button register with the button constant we want */
    unsigned short pressed = *buttons & button;

    /* if this value is zero, then it's not pressed */
    if (pressed == 0) {
        return 1;
    } else {
        return 0;
    }
}


/* return a pointer to one of the 4 character blocks (0-3) */
volatile unsigned short* char_block(unsigned long block) {
    /* they are each 16K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x4000));
}

/* return a pointer to one of the 32 screen blocks (0-31) */
volatile unsigned short* screen_block(unsigned long block) {
    /* they are each 2K big */
    return (volatile unsigned short*) (0x6000000 + (block * 0x800));
}


/* function to setup background 0 for this program */
void setup_background() {

    /* load the palette from the image into palette memory*/
    for (int i = 0; i < PALETTE_SIZE; i++) {
        bg_palette[i] = background_palette[i];
    }

    /* load the image into char block 0 (16 bits at a time) */
    volatile unsigned short* dest  = char_block(0);
    volatile unsigned short* dest2 = char_block(1);

    unsigned short* image = (unsigned short*) background_data;
    for (int i = 0; i < ((background_width * background_height) / 2); i++) {
        dest[i] = image[i];
        dest2[i] = image[i];
    }
    

    /* set all control the bits in this register */    
    *bg1_control = 0 |    /* priority, 0 is highest, 3 is lowest */
        (1 << 2)  |       /* the char block the image data is stored in */
        (0 << 6)  |       /* the mosaic flag */
        (1 << 7)  |       /* color mode, 0 is 16 colors, 1 is 256 colors */
        (17 << 8) |       /* the screen block the tile data is stored in */
        (1 << 13) |       /* wrapping flag */
        (0 << 14);        /* bg size, 0 is 256x256 */
    
    *bg0_control = 1 |    /* priority, 0 is highest, 3 is lowest */
        (0 << 2)  |       /* the char block the image data is stored in */
        (0 << 6)  |       /* the mosaic flag */
        (1 << 7)  |       /* color mode, 0 is 16 colors, 1 is 256 colors */
        (16 << 8) |       /* the screen block the tile data is stored in */
        (1 << 13) |       /* wrapping flag */
        (0 << 14);        /* bg size, 0 is 256x256 */


    /* load the  data into screen block 16 */
    dest = screen_block(16);
    for (int i = 0; i < (map_width * map_height); i++) {
	dest[i] = map[i];
    }
    dest2 = screen_block(17);
    for (int j = 0; j < (map2_width * map2_height); j++){
	dest2[j] = map2[j];
    }
    volatile unsigned short* ptr = screen_block(18);
    /* clear the tile map in screen block to all black tile*/
    for(int i = 0; i < 32 * 32; i++){
	ptr[i] = 95; 
    }
    //clear the text map to be all blanks
    ptr = screen_block(24);
    for (int i = 0; i < 32 * 32; i++){
    	ptr[i] = 0;
    }
}



/* a sprite is a moveable image on the screen */
struct Sprite {
    unsigned short attribute0;
    unsigned short attribute1;
    unsigned short attribute2;
    unsigned short attribute3;
};

/* array of all the sprites available on the GBA */
struct Sprite sprites[NUM_SPRITES];
int next_sprite_index = 0;

/* the different sizes of sprites which are possible */
enum SpriteSize {
    SIZE_8_8,
    SIZE_16_16,
    SIZE_32_32,
    SIZE_64_64,
    SIZE_16_8,
    SIZE_32_8,
    SIZE_32_16,
    SIZE_64_32,
    SIZE_8_16,
    SIZE_8_32,
    SIZE_16_32,
    SIZE_32_64,
};

/* function to initialize a sprite with its properties, and return a pointer */
struct Sprite* sprite_init(int x, int y, enum SpriteSize size,
        int horizontal_flip, int vertical_flip, int tile_index, int priority) {

    /* grab the next index */
    int index = next_sprite_index++;

    /* setup the bits used for each shape/size possible */
    int size_bits, shape_bits;
    switch (size) {
        case SIZE_8_8:   size_bits = 0; shape_bits = 0; break;
        case SIZE_16_16: size_bits = 1; shape_bits = 0; break;
        case SIZE_32_32: size_bits = 2; shape_bits = 0; break;
        case SIZE_64_64: size_bits = 3; shape_bits = 0; break;
        case SIZE_16_8:  size_bits = 0; shape_bits = 1; break;
        case SIZE_32_8:  size_bits = 1; shape_bits = 1; break;
        case SIZE_32_16: size_bits = 2; shape_bits = 1; break;
        case SIZE_64_32: size_bits = 3; shape_bits = 1; break;
        case SIZE_8_16:  size_bits = 0; shape_bits = 2; break;
        case SIZE_8_32:  size_bits = 1; shape_bits = 2; break;
        case SIZE_16_32: size_bits = 2; shape_bits = 2; break;
        case SIZE_32_64: size_bits = 3; shape_bits = 2; break;
    }

    int h = horizontal_flip ? 1 : 0;
    int v = vertical_flip ? 1 : 0;

    /* set up the first attribute */
    sprites[index].attribute0 = y |             /* y coordinate */
                            (0 << 8) |          /* rendering mode */
                            (0 << 10) |         /* gfx mode */
                            (0 << 12) |         /* mosaic */
                            (1 << 13) |         /* color mode, 0:16, 1:256 */
                            (shape_bits << 14); /* shape */

    /* set up the second attribute */
    sprites[index].attribute1 = x |             /* x coordinate */
                            (0 << 9) |          /* affine flag */
                            (h << 12) |         /* horizontal flip flag */
                            (v << 13) |         /* vertical flip flag */
                            (size_bits << 14);  /* size */

    /* setup the second attribute */
    sprites[index].attribute2 = tile_index |   // tile index */
                            (priority << 10) | // priority */
                            (0 << 12);         // palette bank (only 16 color)*/

    /* return pointer to this sprite */
    return &sprites[index];
}

/* flag for turning on DMA */
#define DMA_ENABLE 0x80000000

/* flags for the sizes to transfer, 16 or 32 bits */
#define DMA_16 0x00000000
#define DMA_32 0x04000000

/* pointer to the DMA source location */
volatile unsigned int* dma_source = (volatile unsigned int*) 0x40000D4;

/* pointer to the DMA destination location */
volatile unsigned int* dma_destination = (volatile unsigned int*) 0x40000D8;

/* pointer to the DMA count/control */
volatile unsigned int* dma_count = (volatile unsigned int*) 0x40000DC;

/* copy data using DMA */
void memcpy16_dma(unsigned short* dest, unsigned short* source, int amount) {
    *dma_source = (unsigned int) source;
    *dma_destination = (unsigned int) dest;
    *dma_count = amount | DMA_16 | DMA_ENABLE;
}

/* update all of the spries on the screen */
void sprite_update_all() {
    /* copy them all over */
    memcpy16_dma((unsigned short*) sprite_attribute_memory, (unsigned short*) sprites, NUM_SPRITES * 4);
}

/* setup all sprites */
void sprite_clear() {
    /* clear the index counter */
    next_sprite_index = 0;

    /* move all sprites offscreen to hide them */
    for(int i = 0; i < NUM_SPRITES; i++) {
        sprites[i].attribute0 = HEIGHT;
        sprites[i].attribute1 = WIDTH;
    }
}

/* set a sprite postion */
void sprite_position(struct Sprite* sprite, int x, int y) {
    /* clear out the y coordinate */
    sprite->attribute0 &= 0xff00;

    /* set the new y coordinate */
    sprite->attribute0 |= (y & 0xff);

    /* clear out the x coordinate */
    sprite->attribute1 &= 0xfe00;

    /* set the new x coordinate */
    sprite->attribute1 |= (x & 0x1ff);
}

/* move a sprite in a direction */
void sprite_move(struct Sprite* sprite, int dx, int dy) {
    /* get the current y coordinate */
    int y = sprite->attribute0 & 0xff;

    /* get the current x coordinate */
    int x = sprite->attribute1 & 0x1ff;

    /* move to the new location */
    sprite_position(sprite, x + dx, y + dy);
}

/* change the vertical flip flag */
void sprite_set_vertical_flip(struct Sprite* sprite, int vertical_flip) {
    if (vertical_flip) {
        /* set the bit */
        sprite->attribute1 |= 0x2000;
    } else {
        /* clear the bit */
        sprite->attribute1 &= 0xdfff;
    }
}

/* change the vertical flip flag */
void sprite_set_horizontal_flip(struct Sprite* sprite, int horizontal_flip) {
    if (horizontal_flip) {
        /* set the bit */
        sprite->attribute1 |= 0x1000;
    } else {
        /* clear the bit */
        sprite->attribute1 &= 0xefff;
    }
}

/* change the tile offset of a sprite */
void sprite_set_offset(struct Sprite* sprite, int offset) {
    /* clear the old offset */
    sprite->attribute2 &= 0xfc00;

    /* apply the new one */
    sprite->attribute2 |= (offset & 0x03ff);
}
/* setup the sprite image and palette */
void setup_sprite_image() {
    /* load the palette from the image into palette memory*/

    memcpy16_dma((unsigned short*) sprite_palette, (unsigned short*) sprites_palette, PALETTE_SIZE);

    /* load the image into char block 0 */
    memcpy16_dma((unsigned short*) sprite_image_memory, (unsigned short*) sprites_data, (sprites_width * sprites_height) / 2);
}

/* finds which tile a screen coordinate maps to, taking scroll into account */
unsigned short tile_lookup(int x, int y, int xscroll, int yscroll,
        const unsigned short* tilemap, int tilemap_w, int tilemap_h) {

    /* adjust for the scroll */
    x += xscroll;
    y += yscroll;

    /* convert from screen coordinates to tile coordinates */
    x >>= 3;
    y >>= 3;

    /* account for wraparound */
    while (x >= tilemap_w) {
        x -= tilemap_w;
    }
    while (y >= tilemap_h) {
        y -= tilemap_h;
    }
    while (x < 0) {
        x += tilemap_w;
    }
    while (y < 0) {
        y += tilemap_h;
    }

    /* lookup this tile from the map */
    int index = y * tilemap_w + x;

    /* return the tile */
    return tilemap[index];
}










struct Davies {
    struct Sprite* sprite;	/* the actual sprite attribute info */
    int x, y;			/* the x and y postion, in 1/256 pixels */
    int yvel;			/* the Davies's y velocity in 1/256 pixels/second */
    int gravity; 		/* the Davies's y acceleration in 1/256 pixels/second^2 */
    int frame;			/* which frame of the animation he is on */
    int animation_delay;	/* the number of frames to wait before flipping */
    int counter;		/* the animation counter counts how many frames until we flip */
    int move;			/* whether the koopa is moving right now or not */
    int border;			/* the number of pixels away from the edge of the screen the koopa stays */
    int falling;		/* if the koopa is currently falling */
    int hurt;			/*if finlayson touches davies */
};
/* initialize the Davies */
void davies_init(struct Davies* davies) {
    davies->x = 116 << 8;
    davies->y = 100 << 8;
    davies->yvel = 0;
    davies->gravity = 50;
    davies->border = 40;
    davies->frame = 0;
    davies->move = 0;
    davies->counter = 0;
    davies->falling = 0;
    davies->hurt = 0;
    davies->animation_delay = 4;
    davies->sprite = sprite_init(davies->x >> 8, davies->y >> 16, SIZE_16_32, 0, 0, davies->frame, 0);
}

/* a struct for the finlayson's logic and behavior */
struct Finlayson {
    struct Sprite* sprite;	/* the actual sprite attribute info */
    int x, y;			/* the x and y postion, in 1/256 pixels */
    int yvel;			/* the finlayson's y velocity in 1/256 pixels/second */
    int gravity; 		/* the finlayson's y acceleration in 1/256 pixels/second^2 */
    int frame;			/* which frame of the animation he is on */
    int animation_delay;	/* the number of frames to wait before flipping */
    int counter;		/* the animation counter counts how many frames until we flip */
    int move;			/* whether the finlayson is moving right now or not */
    int border;			/* the number of pixels away from the edge of the screen the finlayson stays */
    int falling;		/* if the finlayson is currently falling */
};
/* initialize the finlayson */
void finlayson_init(struct Finlayson* finlayson) {
    finlayson->x = 20 << 8;	//20
    finlayson->y = 113 << 8;	//113
    finlayson->yvel = 0;
    finlayson->gravity = 50;
    finlayson->frame = 64;
    finlayson->move = 0;
    finlayson->counter = 0;
    finlayson->falling = 0;
    finlayson->animation_delay = 8;
    finlayson->sprite = sprite_init(finlayson->x >> 8, finlayson->y >> 8, SIZE_32_32, 0, 0, finlayson->frame, 0);
//		int x, int y, enum SpriteSize size, 
//		int horizontal_flip, int vertical_flip, int tile_index, int priority
}



/* move the davies left or right returns if it is at edge of the screen */
int davies_left(struct Davies* davies) {
    /* face left */
    sprite_set_horizontal_flip(davies->sprite, 1);
    davies->move = 1;

    /* if we are at the left end, just scroll the screen */
    if ((davies->x >> 8) < davies->border) {
        return 1;
    } else {
        /* else move left */
        davies->x -= 256;
        return 0;
    }
}
int davies_right(struct Davies* davies) {
    /* face right */
    sprite_set_horizontal_flip(davies->sprite, 0);
    davies->move = 1;

    /* if we are at the right end, just scroll the screen */
    if ((davies->x >> 8) > (WIDTH - 16 - davies->border)) {
        return 1;
    } else {
        /* else move right */
        davies->x += 256;
        return 0;
    }
}

/* stop the davies from walking left/right */
void davies_stop(struct Davies* davies) {
    davies->move = 0;
    davies->frame = 0;
    davies->counter = 7;
    sprite_set_offset(davies->sprite, davies->frame);
}

void jump(int* falling, int* yvel); //assembly call

/* start the davies jumping, unless already fgalling */
void davies_jump(struct Davies* davies) {
    jump(&davies->falling, &davies->yvel); //assembly call

    if (davies->falling == 0){
        	davies->yvel = -900;
        	davies->falling = 1;
     }
}

/* update the davies, returns davis position */
int davies_update(struct Davies* davies, struct Finlayson* finlayson, int xscroll, int yscroll) {
    /* update y position and speed if falling */
    if(davies->yvel > 4000){davies->yvel=4000;}
    if (davies->falling) {
        davies->y    += davies->yvel + davies->yvel + 100;
        davies->yvel += davies->gravity;
    }
    /* check which tile the davies's feet are over */
    unsigned short tile = tile_lookup((davies->x >> 8) + 8, (davies->y >> 8) + 32, xscroll,
            yscroll, map2, map2_width, map2_height);

    /* if it's block tile
     * these numbers refer to the tile indices of the blocks the davies can walk on */
    if (tile >= 44 && tile <= 48) {	//pillars:sma_44,big_45 Plat:truss_46,fill_47,sun_48 //out:43,49
        /* stop the fall! */
        davies->falling = 0;
	if (davies->yvel > 0){
		davies_jump(davies);
	}
        /* make him line up with the top of a block
         * works by clearing out the lower bits to 0 */
        davies->y &= ~0x7ff;

        /* move him down one because there is a one pixel gap in the image */
        davies->y = davies->y + 2;

    } else {
        /* he is falling now */
        davies->falling = 1;
    }
    /*davies->hurt=0;
    if ((davies->x <= finlayson->x + 7169 && davies->x >= finlayson->x - 7169) && 	//min=7169->8192=max
	(davies->y <= finlayson->y + 7169 && davies->y >= finlayson->y - 7169)){
	davies->hurt = 1;
    }*/
    if(davies->hurt){davies->counter++;}	/* shift frame to red frames, double animation */
    /* update animation */
    davies->counter++;
    if (davies->counter >= davies->animation_delay) {
	davies->frame=0;
	if(davies->move){davies->frame += 16;}
	if(davies->hurt){davies->frame += 32;}
	//if((davies->hurt) && (!davies->move)){davies->frame=12;}
        //if (davies->hurt && davies->move && davies->frame > 48) {davies->frame = 16;}
        //if (davies->hurt && !davies->move && davies->frame > 48) {davies->frame = 16;}

        //if (!davies->hurt && davies->move && davies->frame > 32) {davies->frame = 16;}
        //if (!davies->hurt && !davies->move && davies->frame > 16) {davies->frame = 0;}
        sprite_set_offset(davies->sprite, davies->frame);
        davies->counter = 0;
    }

    /* update animation if moving  */
 /*   if (davies->move) {
        davies->counter++;
        if (davies->counter >= davies->animation_delay) {
            davies->frame = davies->frame + 16;
            if (davies->frame > 16) {
                davies->frame = 0;
            }
            sprite_set_offset(davies->sprite, davies->frame);
            davies->counter = 0;
        }
    }
    if (davies->hurt==1) {
        davies->counter++;
        if (davies->counter >= davies->animation_delay-2) {
            davies->frame = davies->frame + 32;
            if (davies->frame > 36) {
                davies->frame = 0;
            }
            sprite_set_offset(davies->sprite, davies->frame);
            davies->counter = 0;
        }
    }
    /* set on screen position */
    //yscroll = davies->y >>8;
    sprite_position(davies->sprite, davies->x >> 8, davies->y >> 8);
    return davies->y;
}




/* move the finlayson left or right returns if it is at edge of the screen */
int finlayson_left(struct Finlayson* finlayson) {
    /* face left */
    sprite_set_horizontal_flip(finlayson->sprite, 1);
    finlayson->move = 1;

    /* if we are at the left end, just scroll the screen */
    if ((finlayson->x >> 8) < finlayson->border) {
        return 1;
    } else {
        /* else move left */
        finlayson->x -= 256;
        return 0;
    }
}
int finlayson_right(struct Finlayson* finlayson) {
    /* face right */
    sprite_set_horizontal_flip(finlayson->sprite, 0);
    finlayson->move = 1;

    /* if we are at the right end, just scroll the screen */
    if ((finlayson->x >> 8) > (WIDTH - 16 - finlayson->border)) {
        return 1;
    } else {
        /* else move right */
        finlayson->x += 256;
        return 0;
    }
}

/* stop the finlayson from walking left/right */
void finlayson_stop(struct Finlayson* finlayson) {		//find
    finlayson->move = 0;
    finlayson->frame = 0;
    finlayson->counter = 7;
    sprite_set_offset(finlayson->sprite, finlayson->frame);
}

int finlayson_is_unstoppable(struct Finlayson* finlayson, struct Davies* davies, int numLives){
	davies->hurt = 0;
	if ((davies->x <= finlayson->x + 7169 && davies->x >= finlayson->x - 3585) && 	//min=7169->8192=max
	    (davies->y <= finlayson->y + 7169 && davies->y >= finlayson->y - 7169)){
		numLives--;
		davies->hurt = 1;
	}
        return numLives;	
}
/* update the finlayson */
void finlayson_update(struct Finlayson* finlayson, struct Davies* davies, int xscroll) {
    finlayson->y+=10;
//    if (finlayson->x+10000 > davies->x){
//	finlayson->x = finlayson->x =0;	//slowdown
//    }
//    if (!finlayson->x-10000 > davies->x){
//	finlayson->x = finlayson->x =0;	//slowdown
//    }
    if (finlayson->x < davies->x){
	finlayson->x = finlayson->x + 30 + 15;
    }
    else if (finlayson->x > davies->x){
	finlayson->x = finlayson->x - 30 - 15;
    }
    if (finlayson->y > davies->y){
	finlayson->y = finlayson->y - 60 - 15;
    }
    else if (finlayson->y < davies->y){
	finlayson->y = finlayson->y + 60 + 15;
    }

    /* set on screen position */
    sprite_position(finlayson->sprite, finlayson->x >> 8, finlayson->y >> 8);
}

/* just kill time */
/*void delay(unsigned int amount) {
    for (int i = 0; i < amount * 10; i++); 
}*/

void delay (unsigned int amount); //assembly

void set_text(char* str, int row, int col){
	int index = row * 32 + col;
	int missing = 32;
	volatile unsigned short* ptr = screen_block(24);
	while (*str){
		ptr[index] = *str - missing;
		index++;
		str++;
	}
}

//assembly call
//int finlayson_is_unstoppable(int numLives, struct Finlayson* finlayson, struct Davies* davies);
int finlaysonKill(int numLives, struct Davies* davies, struct Finlayson* finlayson);

/* the main function */
int main( ) {
    /* we set the mode to mode 0 with bg0 on */
    *display_control = MODE0 | BG0_ENABLE | BG1_ENABLE | SPRITE_ENABLE | SPRITE_MAP_1D; // added 2nd pipe

    setup_background();		/* setup the background 0 */
    sprite_clear();		/* clear all the sprites on screen now */

    /* setup the sprite image data */
    setup_sprite_image();
    char score[32] = "100";
    //set_text(score, 0, 0);  //maybe can't do it because it requres anotrher background.txt (text_background.h)

    /* create the sprites */
    struct Davies davies;
    davies_init(&davies);

    struct Finlayson finlayson;
    finlayson_init(&finlayson);

    /* set initial scroll to 0 */
    int xscroll = 0;
    int yscroll = 0;

    int speedOne = 1;
    int speedTwo = 0;
    int speedThree = 0;
    int currSpeed = 0;

    int numLives = 20;
    int bottomOfTheScreen = davies.y + 15000;
    int scrollCounter = 10;  // switches to faster scrolling when davies hits top
    /* loop forever */
    while (1) {
	//int moveIan = rand() %6-3;
	//finlayson.y += 1 >>8;

	numLives = finlayson_is_unstoppable(&finlayson, &davies, numLives);
	//numLives = finlaysonKill(numLives, &davies, &finlayson);

	if (numLives == 0){
		break;
	}

	if (davies.y > bottomOfTheScreen){
		break;
	}

	bottomOfTheScreen++;

	if(davies.y < yscroll){
		scrollCounter = 0; //health computer	
	}

        /* scroll with the arrow keys */
        if (button_pressed(BUTTON_DOWN)) {
            //yscroll++;
	    davies.falling = 1;
        }
        if (button_pressed(BUTTON_UP)) {
        }
        if (button_pressed(BUTTON_RIGHT)) {
            //xscroll++;
        }
        if (button_pressed(BUTTON_LEFT)) {
            //xscroll--;
        }
	if (scrollCounter <= 10){ //increase scroll speed
		yscroll = yscroll - 2;
		scrollCounter++;
	}	
	else{
		if (speedOne == 1){
			yscroll = yscroll - 1;
		}
		/*else if (speedTwo == 1){
			yscroll = yscroll - 2;
		}
		else if (speedThree == 1){
			yscroll = yscroll - 3;
		}
		currSpeed++;
		if (currSpeed > 500){
			speedThree = 1;
			speedTwo = 0;
		}
		else if (currSpeed > 100){
			speedTwo = 1;
			speedOne = 0;
		}*/
	}
	//yscroll%=257+1; //this allows for yscroll to always be in 1 to 256 and not go past 256
	yscroll%=256;
	//if(davies.y >>8 > (yscroll+180%256)){break;}

	int DavisPos = davies_update(&davies, &finlayson, xscroll, yscroll);
	finlayson_update(&finlayson, &davies, xscroll);
	//if(yscroll+150 <  ((davies.y >> 8)-32)){	//If character is On bottom
	    //yscroll=((davies.y >> 8)32)%160;
	//    yscroll+=2;
	//}
	//else if(yscroll+40 >= ((davies.y >> 8)-32)){		//if character is high
	    //yscroll=((davies.y >> 8)-32)%160;
	//   yscroll-=5;
	//}
	//while(yscroll+100 >= ((davies.y >> 8))){		//if character is high
	    //yscroll=((davies.y >> 8)-32)%160;
	//    yscroll-=5;
	//}
	//yscroll=yscroll%240;
	//yscroll = davies.y;
	//yscroll=yscroll%256;

        /* now the arrow keys move the koopa */
        if (button_pressed(BUTTON_RIGHT)) {
            if (davies_right(&davies)) {
                xscroll+=1;
            }
        } else if (button_pressed(BUTTON_LEFT)) {
            if (davies_left(&davies)) {
                xscroll-=1;
            }
        } else {
            davies_stop(&davies);
        }

        /* check for jumping */
        if (button_pressed(BUTTON_A)) {
        }
        if (button_pressed(BUTTON_B)) {
            if (button_pressed(BUTTON_RIGHT)) {
        	if (davies_right(&davies)) {
        	    xscroll+=2;
        	}
            } else if (button_pressed(BUTTON_LEFT)) {
        	if (davies_left(&davies)) {
        	    xscroll-=2;
        	}
	    }
        }

        /* wait for vblank before scrolling and moving sprites */
        //*bg0_x_scroll = xscroll;
        sprite_update_all();
        /* wait for vblank before scrolling */
        wait_vblank();
        *bg0_x_scroll = xscroll/2;
        *bg0_y_scroll = yscroll/2;
        *bg1_x_scroll = xscroll;
        *bg1_y_scroll = yscroll;

        /* delay some */
        delay(8500);
    }
}


/* the game boy advance uses "interrupts" to handle certain situations
 * for now we will ignore these */
void interrupt_ignore( ) {
    /* do nothing */
}


/* this table specifies which interrupts we handle which way
 * for now, we ignore all of them */
typedef void (*intrp)( );
const intrp IntrTable[13] = {
    interrupt_ignore,   /* V Blank interrupt */
    interrupt_ignore,   /* H Blank interrupt */
    interrupt_ignore,   /* V Counter interrupt */
    interrupt_ignore,   /* Timer 0 interrupt */
    interrupt_ignore,   /* Timer 1 interrupt */
    interrupt_ignore,   /* Timer 2 interrupt */
    interrupt_ignore,   /* Timer 3 interrupt */
    interrupt_ignore,   /* Serial communication interrupt */
    interrupt_ignore,   /* DMA 0 interrupt */
    interrupt_ignore,   /* DMA 1 interrupt */
    interrupt_ignore,   /* DMA 2 interrupt */
    interrupt_ignore,   /* DMA 3 interrupt */
    interrupt_ignore,   /* Key interrupt */
};

