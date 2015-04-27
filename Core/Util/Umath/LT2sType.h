/**********************
 ***   Lt2sType.h   ***
 **********************/
#ifndef	_LT2S_TYPE_
#define _LT2S_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include	"Lt2Type.h"


// similarity transformation; special case of linear transform 



typedef struct lt2s_type {
    union {
        struct {
			union {
				struct {
					float	x;
					float	y;
				};
				vec2f_type p;
			};

			float	r;	// rotate
			float	s;	// scale
        };

        float a[4];
    };

}	lt2s_type;


//#define	QR_X( qr )	((qr).a[0])
//#define	QR_Y( qr )	((qr).a[1])
//#define	QR_S( qr )	((qr).a[2])
//#define	QR_R( qr )	((qr).a[3])


	// Lt2sTool.c
void	lt2s_clear( lt2s_type *lt );

void	lt2s_from_lt2( lt2_type *lt, lt2s_type *qr );

void	lt2s_to_lt2( lt2s_type *qr, lt2_type *lt );

void	lt2s_max_diff( lt2s_type *lt1, lt2s_type *lt2, lt2s_type *m );

void	lt2s_print( lt2s_type *qr, FILE *fp );

void	lt2sA_print( lt2s_type qr[], int nQr, char *title, FILE *fp );


#ifdef __cplusplus
}
#endif

#endif