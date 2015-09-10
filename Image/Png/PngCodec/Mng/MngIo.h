/********************
 ***	MngIo.h   ***
 ********************/
#ifndef	_MNG_IO_
#define _MNG_IO_

#include	"ImageType/ImageType.h"
#include	"Ugio/GioType.h"


typedef struct mngIo_type {
	
	int	column;

	int	row;

	int	frame_i;	// index of current frame

	int	frame_no;

	int BgColor;

	long	end_header_location;
	
	// graphic control
	int	transparent_flag;
	int	transparent_index;
	int	disposal_method;
	int	delay_time;
	int	prev_delay_time;
	int	avrageDelay;

	int	frameTime;
	int	duration;

	int	*ftell_delayTime;

	float	dev;	//variabllty

	int	sizeLimit;	// for write
	int	Fdither;	// 24to8 conversion with dithering

	palette_type	*palette;

	image_type	*im;	// current image
	image_type	*im8;	// current index image

	gio_type *gio;

	FILE	*fp;	// for write 

} mngIo_type;


typedef struct MHDR_chunk_type {

	unsigned char Frame_width[4];         //4 bytes (unsigned integer).
	unsigned char Frame_height[4];        //4 bytes (unsigned integer).
    unsigned char Ticks_per_second[4];    //4 bytes (unsigned integer).
    unsigned char Nominal_layer_count[4]; //4 bytes (unsigned integer).
    unsigned char Nominal_frame_count[4]; //4 bytes (unsigned integer).
    unsigned char Nominal_play_time[4];   //4 bytes (unsigned integer).
    unsigned char Simplicity_profile[4];  //4 bytes:(unsigned integer).

} MHDR_chunk_type;


typedef struct FRAM_chunk_type {

	unsigned char	Framing_mode;
	unsigned char	Separator; //0
	unsigned char	Change_interframe_delay;
	unsigned char	Change_timeout_and_termination;
	unsigned char	Change_layer_clipping_boundaries;
	unsigned char	Change_sync_id_list;
	unsigned char	Interframe_delay[4];
	//unsigned char	Timeout[4]; //0x7fffffff

} FRAM_chunk_type;


typedef struct BACK_chunk_type {

	unsigned char	Red[2];
	unsigned char	Green[2]; 
	unsigned char	Blue[2];
	//unsigned char	Mandatory_background;

} BACK_chunk_type;


typedef struct DEFI_chunk_type {

	unsigned char	Object_id[2];         //2 bytes frame_i
	unsigned char	Do_not_show;	   //1 byte = 0		
	unsigned char	Concrete_flag;	   //1 byte = 0		

} DEFI_chunk_type;


typedef struct TERM_chunk_type {

	unsigned char	Termination_action;
	unsigned char	Action_after_iterations;
	unsigned char	Delay[4];
	unsigned char	Iteration_max[4];

} TERM_chunk_type;

// Delta-PNG
typedef struct DHDR_chunk_type {

	unsigned char	Object_id[2];
	unsigned char	Image_type;
	unsigned char	Delta_type;
	unsigned char	Block_width[4];
	unsigned char	Block_height[4];
	unsigned char	Block_X_location[4];
	unsigned char	Block_Y_location[4];

} DHDR_chunk_type;


#endif
