/**************************
 ***	ImageSubtruct.c	***
 *************************/

#include	"ImageType/ImageType.h"
#include	"Umath/LT2Type.h"



image_type *
image1_interpolation( image_type *sim,
					 float x0, float y0, int width, int height,
					 image_type *im )
{
u_char	*sp0,	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
int	s;

	if( im != NULL && (im->column != width || im->row != height) ){
		image_destroy( im, 1 );
		im = NULL;
	}
	if( im == NULL )
		im = image_create( height, width, 1, 1, NULL );



	tp = (u_char *)im->data;
	for( i = 0, y = y0 ; i < im->row ; i++, y++ ){

		if( y < 0 || y >= sim->row-1){
			for( j = 0 ; j < im->column ; j++ )
				*tp++ = 0;
			continue;
		}


		iy = y;
		dy = y - iy;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		for( j = 0, x = x0 ; j < im->column ; j++, x++ ){
			if( x < 0 || x >= sim->column-1 ){
				*tp++ = 0;
				continue;
			}

			ix = x;
			dx = x - ix;

			sp = sp0 + ix;

			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}

image_type *
image1_interpolation_1( image_type *sim,
					 float x0, float y0, int width, int height,
					 image_type *im )
{
u_char	*sp0,	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
int	s;

	if( im != NULL && (im->column != width || im->row != height) ){
		image_destroy( im, 1 );
		im = NULL;
	}
	if( im == NULL )
		im = image_create( height, width, 1, 1, NULL );



	dy = y0 - (int)y0;
	if( y0 < 0 )	dy = -dy;

	dx = x0 - (int)x0;
	if( x0 < 0 )	dx = -dx;


	tp = (u_char *)im->data;
	for( i = 0, y = y0 ; i < im->row ; i++, y++ ){

		if( y < 0 || y > sim->row-1){
			for( j = 0 ; j < im->column ; j++ )
				*tp++ = 0;
			continue;
		}


		iy = y;
//		dy = y - iy;

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		for( j = 0, x = x0 ; j < im->column ; j++, x++ ){
			if( x < 0 || x > sim->column-1 ){
				*tp++ = 0;
				continue;
			}

			ix = x;
//			dx = x - ix;

			sp = sp0 + ix;

			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}



image_type *
image1_interpolationS( image_type *sim,
					 float x0, float y0, int width, int height,
					 float Dx, float Dy, float Ds,
					 image_type *im )
{
u_char	*sp0,	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;//,	ds;
int	ix,	iy;
float	s1,	s2;
int	s;

	if( im != NULL && (im->column != width || im->row != height) ){
		image_destroy( im, 1 );
		im = NULL;
	}
	if( im == NULL )
		im = image_create( height, width, 1, 1, NULL );


	tp = (u_char *)im->data;
	for( i = 0, y = y0 ; i < im->row ; i++ ){

		y = y0 + ( 1 + Ds ) * (i+0.5) + Dy;


		if( y < 0.5 || y > sim->row-1){
			for( j = 0 ; j < im->column ; j++ )
				*tp++ = 0;
			continue;
		}





		iy = y-0.5;
		dy = y - (iy+0.5);

		sp0 = IMAGE_PIXEL( sim, iy, 0 );

		for( j = 0, x = x0 ; j < im->column ; j++ ){

			x = x0 + ( 1 + Ds ) * (j+0.5) + Dx;

			if( x < 0.5 || x > sim->column-1 ){
				*tp++ = 0;
				continue;
			}

			
			ix = x-0.5;
			dx = x - (ix+0.5);

			sp = sp0 + ix;

			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}



int image1_interpolation_lt2_test_in( image_type *sim, lt2_type *lt,
								int x0, int y0,
								int width, int height )
{
vec2f_type	p;

	p.x = LT2_F1( *lt, 0, 0 ) + x0;
	p.y = LT2_F2( *lt, 0, 0 ) + y0;

	if( p.x < 0 || p.x >= sim->width || p.y < 0 || p.y >= sim->height )
		return( -1 );

	p.x = LT2_F1( *lt, width, 0 ) + x0;
	p.y = LT2_F2( *lt, width, 0 ) + y0;

	if( p.x < 0 || p.x >= sim->width || p.y < 0 || p.y >= sim->height )
		return( -1 );



	p.x = LT2_F1( *lt, width, height ) + x0;
	p.y = LT2_F2( *lt, width, height ) + y0;

	if( p.x < 0 || p.x >= sim->width || p.y < 0 || p.y >= sim->height )
		return( -1 );


	p.x = LT2_F1( *lt, 0, height ) + x0;
	p.y = LT2_F2( *lt, 0, height ) + y0;

	if( p.x < 0 || p.x >= sim->width || p.y < 0 || p.y >= sim->height )
		return( -1 );

	return( 1 );

}


//#define _USE_ASM
#ifndef _USE_ASM
image_type *
image1_interpolation_lt2( image_type *sim, lt2_type *lt,
						 int x0, int y0,
						 int width, int height,
						 image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;
	float	x,	y,	dx,	dy;
	int	ix,	iy;
	float	s1,	s2;
	int	s;
	lt2_type	lt0;

	im = image_recreate( im, height, width, 1, 1 );



	lt0.b0 = lt0.b1 = 0;

	tp = (u_char *)im->data;
	for( i = 0 ; i < im->height ; i++ ){
		lt0.a0 = lt->a0;
		lt0.c0 = 0.5*lt->a0 + (i+0.5)*lt->b0 + lt->c0 + x0;

		lt0.a1 = lt->a1;
		lt0.c1 = 0.5*lt->a1 + (i+0.5)*lt->b1 + lt->c1 + y0;

		x = lt0.c0 - lt0.a0;
		y = lt0.c1 - lt0.a1;

		for( j = 0 ; j < im->width ; j++ ){

			x += lt0.a0;
			y += lt0.a1;


			ix = x-0.5;
			iy = y-0.5;


			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				continue;
			}


			sp = IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			s1 = *sp + dx * (  *(sp+1) - *sp );
			sp += sim->column;
			s2 = *sp + dx * (*(sp+1) - *sp );

			s = s1 + dy * (s2 - s1 ) + 0.5;


			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}
#else

image_type *
image1_interpolation_lt2( image_type *sim, lt2_type *lt,
						 int x0, int y0,
						 int width, int height,
						 image_type *im )
{
	u_char	*tp;



	lt2_type	lt0;



	float static jj1[128];

	int cmpx=sim->column;
	int cmpy=sim->row;
	int data= (int)sim->data;
	int k,k1;
	int a=0;
	float *jj,xx,yy,hold0,hold1,hold2,hold3,half=.5;

	k=(width>>2);

	k1=width&3;
	if(k1)k++;

	im = image_recreate( im, height, width, 1, 1 );
	lt0.b0 = lt0.b1 = 0;
	tp = (u_char *)im->data;
	hold0=lt->b0;
	hold1=lt->b1;
	hold2=lt->a0;
	hold3=lt->a1;

	//     temp1=temp;
	jj=jj1;
	xx=0.5*hold2 + 0.5*hold0 + lt->c0 + x0 -.5;
	yy=0.5*hold3 + 0.5*hold1 + lt->c1 + y0 -.5;


	__asm
	{

		mov eax,dword ptr[tp]
		movd mm6,eax


			mov ebx,dword ptr[jj]
		and ebx,0xfffffff0
			add ebx,16
			mov dword ptr[jj],ebx


			mov edx,1
			movd xmm0,edx
			pshufd xmm0,xmm0,0
			cvtdq2ps xmm0,xmm0  // 1
			movdqa xmm2,xmm0

			mov edx,hold0
			movd xmm1,edx
			pshufd xmm1,xmm1,0  // lt->b0
			mulps  xmm0,xmm1    // xmm0= [ lt->b0,lt->b0,lt->b0,lt->b0]
			movdqa [ebx],xmm0 

			mov edx,hold1
			movd xmm1,edx
			pshufd xmm1,xmm1,0  // lt->b1
			mulps  xmm1,xmm2    // xmm1= [ lt->b1,lt->b1,lt->b1,lt->b1]
			movdqa [ebx+16],xmm1 

			pxor xmm2,xmm2
			mov edx,0
			pinsrw xmm2,edx,0
			mov edx,1
			pinsrw xmm2,edx,2
			mov edx,2
			pinsrw xmm2,edx,4
			mov edx,3
			pinsrw xmm2,edx,6
			cvtdq2ps xmm2,xmm2
			movaps  xmm6,xmm2

			mov edx,hold2
			movd xmm3,edx
			pshufd xmm3,xmm3,0  
			mulps  xmm2,xmm3    //  xmm2= [ 0 ,lt->a0,2*lt->a0,3*lt->a0]
			movdqa [ebx+2*16],xmm2 

			mov edx,4
			movd xmm4,edx
			pshufd xmm4,xmm4,0
			cvtdq2ps xmm4,xmm4 // 4
			mulps xmm3,xmm4    //  xmm3= [ 4*lt->a0,4*lt->a0,4*lt->a0,4*lt->a0]
			movaps [ebx+4*16],xmm3


			mov edx,hold3
			movd xmm5,edx
			pshufd xmm5,xmm5,0  
			mulps  xmm4,xmm5    //  xmm4= [ 4*lt->a1,4*lt->a1,4*lt->a1,4*lt->a1]
			mulps  xmm5,xmm6    //  xmm5= [ 0 ,lt->a1,2*lt->a1,3*lt->a1]
			movdqa [ebx+3*16],xmm5
			movaps [ebx+5*16],xmm4





			mov edx,dword ptr[xx]
		movd xmm6,edx
			pshufd xmm6,xmm6,0 // xx
			movaps [ebx+6*16],xmm6      //xx
			mov edx,dword ptr[yy]
		movd xmm7,edx
			pshufd xmm7,xmm7,0 // yy
			movaps [ebx+7*16],xmm7   // yy

			mov eax,dword ptr[cmpx]
		movd xmm0,eax

			pshufd xmm0,xmm0,0
			movdqa xmm2,xmm0
			cvtdq2ps xmm0,xmm0  
			movaps [ebx+8*16],xmm0    // cmpx float
			mov eax,dword ptr[cmpy]
		movd xmm1,eax

			pshufd xmm1,xmm1,0
			cvtdq2ps xmm1,xmm1  
			movaps [ebx+9*16],xmm1  // cmpy  float

			mov eax,32768
			movd xmm0,eax
			pshufd xmm0,xmm0,0
			cvtdq2ps xmm0,xmm0 
			movaps [ebx+10*16],xmm0    // -32768
			mov eax,dword ptr[half]
		movd xmm1,eax
			pshufd xmm1,xmm1,0
			movaps [ebx+11*16],xmm1
			movaps [ebx+12*16],xmm2   // width integer
			mov esi,dword ptr[data]
		movd xmm5,esi
			pshufd xmm5,xmm5,0
			movaps [ebx+13*16],xmm5  // data


			mov edx,dword ptr[jj]
		mov ebx,dword ptr[height]
		mov ecx,dword ptr[k]
		mov esi,dword ptr[cmpx]
		add ecx,1  // ecx=k+1
			pxor mm7,mm7  


loop_on_rows:

		push ebx
			push ecx

			movaps xmm6 ,[edx+6*16]
		movaps xmm7 ,[edx+7*16]
		movaps xmm0,xmm6  //sx= [ xx xx xx xx]
			movaps xmm1,xmm7  //sy= [ yy yy yy yy]
			addps  xmm0,[edx+2*16]  // sx+= [0 1 2 3 ]*lt->a0
		addps  xmm1,[edx+3*16]  // sy+= [0 1 2 3 ]*lt->a1
		addps xmm6,[edx]     // xx+=[1 1 1 1 ]*lt->b0
		addps xmm7,[edx+16]  // yy+=[1 1 1 1 ]*lt->b1
		movaps [edx+6*16],xmm6
			movaps [edx+7*16],xmm7
			movd eax,mm6  // tp

loop_on_cols:
		push ecx

			cvttps2dq xmm6,xmm0            // xmm6= ix(integer)

			xorps xmm5,xmm5                // xmm5= 0
			movaps xmm4,[edx+13*16]        // xmm4=data data data data

		push eax
			subps  xmm5,xmm0                //xmm5= float(-x)
			cvttps2dq xmm7,xmm1            // xmm7= iy(integer)


			movaps  [edx+14*16],xmm0
			movaps xmm0,[edx+16*12 ]       // width width width width

		xorps xmm2,xmm2  
			paddd xmm4,xmm6                // xmm4=ix+data

			cvtdq2ps xmm6,xmm6             // xmm6= ix(real)
			// xmm2=0
			pmaxsw xmm7,xmm2

			pmaddwd  xmm0,xmm7            // xmm0=iy*width

			movaps  xmm3,xmm5             //  xmm3=-x float
			psrad   xmm5,31               //  xmm5=mask if x is negtive then FFFF

			subps  xmm2,xmm1                //xmm2= -y

			cvtdq2ps xmm7,xmm7             // xmm7= iy(real)

			addps  xmm6,xmm3                // xmm6=-dx ; xmm3=-x
			paddd  xmm0,xmm4                // adress ready


			movaps [edx+16*16],xmm6
			movaps  xmm4,xmm2
			psrad   xmm2,31                  // mask if y is negtive then FFFF
			movdqa  [edx+15*16],xmm0         // adress ready
			movdqa  xmm0,[edx+14*16]          // restore original values

		addps  xmm7,xmm4                // xmm7=-dy ; xmm4=-y 
			pand   xmm5,xmm2                // xmm5=mask

			movaps [edx+17*16],xmm7
			addps xmm3, [edx+8*16]          // xmm3=col-x 
		mov eax,[edx+15*16]             // first adresss

		addps xmm4, [edx+9*16]          // xmm4=row-y 



		addps xmm0,[edx+4*16]           // sx+=[ 4 4 4 4]*lt->a0

		addps xmm1,[edx+5*16]           // sy+=[ 4 4 4 4]*lt->a1

		psrad xmm3,31
			mov  ebx,[edx+15*16+4]             // second adresss
		pxor  xmm2,xmm2

			movd mm0,[eax]


		movd mm1,[ebx]
		psrad xmm4,31
			pandn xmm3,xmm5

			mov ecx,[edx+15*16+2*4]
		pandn xmm4,xmm3      // final mask

			punpcklbw mm0,mm7    // [s[0] , s[0+1] , X ,X ]

			mov edi,[edx+15*16+3*4]
		punpcklbw mm1,mm7     // [s[1] , s[1+1] , X ,X ]
			movdqa  [edx+15*16],xmm4    // store mask

			movd mm2,[ecx] 
		punpckldq mm0,mm1    //  [s[0] , s[0+1] ,s[1] ,s[1+1]



			movd mm3,[edi]


		punpcklbw mm2,mm7     // [s[2] , s[2+1] , X ,X ]

			movq2dq xmm2,mm0     //xmm2= [s[0] , s[0+1] ,s[1] ,s[1+1] , X ,X ,X, X
			movd mm0,[eax+esi]

		punpcklbw mm3,mm7     // [s[3] , s[3+1] , X ,X ]
			pxor xmm5,xmm5

			movd mm1,[ebx+esi]
		pshuflw xmm2,xmm2,216

			punpckldq mm2,mm3
			movd mm3,[ecx+esi]   
		punpcklwd xmm2,xmm5

			punpcklbw mm0,mm7    // [s[0] , s[0+1] , X ,X ]
			movq2dq xmm3,mm2     //xmm3= [s[2] , s[2+1] ,s[3] ,s[3+1] , X ,X ,X, X
			movd mm2,[edi+esi] 


		punpcklbw mm1,mm7     // [s[1] , s[1+1] , X ,X ]
			pshuflw xmm3,xmm3,216

			punpcklwd xmm3,xmm5

			movdqa xmm5,xmm2
			punpcklqdq xmm2,xmm3  //xmm2= [s0[0] ,  s0[1] , s0[2] , s0[3]]

			punpckhqdq xmm5,xmm3  //xmm5= [s1[0] ,  s1[1] , s1[2] , s1[3]]
			cvtdq2ps xmm2,xmm2    //  c0 

			punpckldq  mm0,mm1


			punpcklbw mm3,mm7     // [s[2] , s[2+1] , X ,X ]
			movq2dq xmm3,mm0
			cvtdq2ps xmm5,xmm5   //c1

			punpcklbw mm2,mm7     // [s[3] , s[3+1] , X ,X ]
			movaps xmm4,xmm2    // xmm4 =c0

			pshuflw xmm3,xmm3,216
			pxor xmm7,xmm7

			punpckldq mm3,mm2  
			subps xmm2,xmm5       // c0 -c1
			movaps xmm5,[edx+16*16]  // xmm5=-dx


		movq2dq xmm6,mm3
			punpcklwd xmm3,xmm7

			pshuflw xmm6,xmm6,216

			mulps  xmm2,xmm5      // xmm2= dx*(c1-c0)
			punpcklwd xmm6,xmm7
			movdqa   xmm7,xmm3

			punpcklqdq xmm3,xmm6

			cvtdq2ps xmm3,xmm3
			punpckhqdq xmm7,xmm6

			cvtdq2ps xmm7,xmm7
			addps  xmm2,xmm4    // xmm2=s1
			// movaps [edx+18*16],xmm2 // s1


			movaps  xmm6,xmm3   // xmm6 c0
			subps  xmm3,xmm7     // xmm3=c0-c1
			movaps xmm7,[edx+17*16] // xmm7=-dy

		mulps   xmm3,xmm5    // xmm3=dx*(c1-c0)
			movaps  xmm4,[edx+15*16]  // mask

		addps   xmm3,xmm6    // xmm3= s2
			// movaps [edx+19*16],xmm3 // s2

			movaps  xmm6,xmm2
			subps   xmm2,xmm3

			addps   xmm6,[edx+11*16] // add .5
		mulps   xmm2,xmm7     // dy(s2-s1)
			//  movaps [edx+20*16],xmm2 // dy*(s2-s1)
			pxor     xmm7,xmm7
			addps   xmm6,xmm2     // result
			// movaps [edx+21*16],xmm6 // dy*(s2-s1)
			pand    xmm6,xmm4
			pop eax

			cvttps2dq xmm6,xmm6

			packuswb  xmm6,xmm7

			packuswb  xmm6,xmm6

			movd dword ptr[eax],xmm6
			add eax,4

			pop ecx

			sub ecx,1
			jne loop_on_cols

			movd eax,mm6
			add eax,dword ptr[width]
		movd mm6,eax
			pop ecx
			pop ebx
			sub ebx,1
			jne loop_on_rows

			emms


	}
	/*
	for( i = 0 ; i < im->height ; i++ ){
	lt0.a0 = lt->a0;
	lt0.c0 = 0.5*lt->a0 + (i+0.5)*lt->b0 + lt->c0 + x0;

	lt0.a1 = lt->a1;
	lt0.c1 = 0.5*lt->a1 + (i+0.5)*lt->b1 + lt->c1 + y0;

	x = lt0.c0 - lt0.a0;
	y = lt0.c1 - lt0.a1;

	for( j = 0 ; j < im->width ; j++ ){


	x += lt0.a0;
	y += lt0.a1;


	ix = x-0.5;
	iy = y-0.5;

	if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
	*tp++ = 0;
	continue;
	}


	sp = IMAGE_PIXEL( sim, iy, ix );

	dx = x - (ix+0.5);
	dy = y - (iy+0.5);


	s1 = *sp + dx * (  *(sp+1) - *sp );
	sp += sim->column;
	s2 = *sp + dx * (*(sp+1) - *sp );

	s = s1 + dy * (s2 - s1 ) + 0.5;


	*tp++ = PUSH_TO_RANGE(s, 0, 255 );
	}
	} */

	return( im );
}
#endif




image_type *
image1_interpolation_lt2I( image_type *sim, lt2_type *lt,
						  int x0, int y0,
						  int width, int height,
						  image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;
	float	x,	y;
	int	ix,	iy;
	int	s1,	s2;
	int	s;
	lt2_type	lt0;
	int	px,	py,	dx,	dy;

	im = image_recreate( im, height, width, 1, 1 );



	lt0.b0 = lt0.b1 = 0;

	tp = (u_char *)im->data;
	for( i = 0 ; i < im->height ; i++ ){
		lt0.a0 = lt->a0;
		lt0.c0 = 0.5*lt->a0 + (i+0.5)*lt->b0 + lt->c0 + x0;

		lt0.a1 = lt->a1;
		lt0.c1 = 0.5*lt->a1 + (i+0.5)*lt->b1 + lt->c1 + y0;

		x = lt0.c0 - lt0.a0;
		y = lt0.c1 - lt0.a1;

		for( j = 0 ; j < im->width ; j++ ){

			x += lt0.a0;
			y += lt0.a1;

			px = x * 256;
			py = y * 256;

			ix = ( px - 128)>>8; //x-0.5;
			iy = ( py - 128)>>8; //y-0.5;



			if( py < 128 || iy >= sim->row-1 ||  px < 128 || ix >= sim->column ){
				*tp++ = 0;
				continue;
			}


			sp = IMAGE_PIXEL( sim, iy, ix );

			//			dx = x - (ix+0.5);
			//			dy = y - (iy+0.5);

			dx = px - (ix<<8) - 128;
			dy = py - (iy<<8) - 128;


			s1 = (*sp <<8) + dx * (  *(sp+1) - *sp );
			sp += sim->column;
			s2 = (*sp<<8) + dx * (*(sp+1) - *sp );


			s = ((s1<<8) + dy * (s2 - s1 ))>>16;


			//			*tp++ = PUSH_TO_RANGE(s, 0, 255 );

			*tp++ = s;
		}
	}

	return( im );
}


#ifdef _AA_
image_type *
image1_interpolation_lt2O( image_type *sim, lt2_type *lt,
					int x0, int y0,
					int width, int height,
					image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
int	s,	no;


	im = image_recreate( im, height, width, 1, 1 );


	tp = (u_char *)im->data;
	for( i = 0, no = 0 ; i < im->row ; i++ ){

		for( j = 0 ; j < im->column ; j++ ){

			x = LT2_F1( *lt, j+0.5, i+0.5 ) + x0;
			y = LT2_F2( *lt, j+0.5, i+0.5 ) + y0;



			ix = x-0.5;
			iy = y-0.5;
	
			if( y < 0.5 || iy >= sim->row-1 ||  x < 0.5 || ix >= sim->column-1 ){
				ix = PUSH_TO_RANGE( ix, 0, sim->width-2 );
				iy = PUSH_TO_RANGE( iy, 0, sim->height-2 );
			}
			else	no++;




			//if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
			//	*tp++ = 0;
			//	continue;
			//}


			sp = IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			dx = PUSH_TO_RANGE( dx, 0, 1 );
			dy = PUSH_TO_RANGE( dy, 0, 1 );


			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}
#endif

int 
image1_interpolation_lt2_N( image_type *sim, lt2_type *lt,
					int x0, int y0,
					int width, int height,
					image_type **im )
{
u_char	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
int	s,	no;


	*im = image_recreate( *im, height, width, 1, 1 );


	tp = (u_char *)(*im)->data;
	for( i = 0, no = 0 ; i < (*im)->row ; i++ ){

		for( j = 0 ; j < (*im)->column ; j++ ){

			x = LT2_F1( *lt, j+0.5, i+0.5 ) + x0;
			y = LT2_F2( *lt, j+0.5, i+0.5 ) + y0;



			ix = x-0.5;
			iy = y-0.5;
	
			if( y < 0.5 || iy >= sim->row-1 ||  x < 0.5 || ix >= sim->column-1 ){
				ix = PUSH_TO_RANGE( ix, 0, sim->width-2 );
				iy = PUSH_TO_RANGE( iy, 0, sim->height-2 );
			}
			else	no++;




			//if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
			//	*tp++ = 0;
			//	continue;
			//}


			sp = IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			dx = PUSH_TO_RANGE( dx, 0, 1 );
			dy = PUSH_TO_RANGE( dy, 0, 1 );


			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( 100*no/ (height* width)  );
}

int
image1M_interpolation_lt2( image_type *sim, lt2_type *lt,
						  int x0, int y0,
						  int width, int height,
						  image_type **im, image_type **mim )
{
	u_char	*sp,	*tp,	*mp;
	int	i,	j;
	float	x,	y,	dx,	dy;//,	ds;
	int	ix,	iy;
	float	s1,	s2;
	int	s;
	int	no;


	*im = image_recreate( *im, height, width, 1, 1 );
	*mim = image_recreate( *mim, height, width, 1, 1 );


	tp = (u_char *)(*im)->data;
	mp = (u_char *)(*mim)->data;

	for( i = 0, no = 0 ; i < (*im)->row ; i++ ){

		for( j = 0 ; j < (*im)->column ; j++ ){

			x = LT2_F1( *lt, j+0.5, i+0.5 ) + x0;
			y = LT2_F2( *lt, j+0.5, i+0.5 ) + y0;



			ix = x-0.5;
			iy = y-0.5;

			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				*mp++ = 0;
				continue;
			}


			sp = IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );

			*mp = 256;

			no++;
		}
	}

	return( 100*no/ (height* width)  );
}


float
image1_interpolation_lt2_ssd( image_type *sim, lt2_type *lt,
							 int x0, int y0,
							 image_type *im, int j1, int i1,
							 int width, int height, float *av, float *ssd )
{
	u_char	*sp,	*tp;
	int	i,	j;
	float	x,	y,	dx,	dy;
	int	ix,	iy;
	float	s1,	s2;
	int	s;
	float	tmp,	sum,	sum1;
	int	nSum,	align;
	lt2_type	lt0;


	sum = 0;
	sum1 = 0;
	nSum = 0;

	tp = (u_char *)im->data;
	tp = IMAGE_PIXEL( im, i1, j1 );
	align = im->width - width;


	for( i = 0 ; i < height ; i++, tp += align ){

		lt0.a0 = lt->a0;
		lt0.c0 = 0.5*lt->a0 + (i+0.5)*lt->b0 + lt->c0 + x0;

		lt0.a1 = lt->a1;
		lt0.c1 = 0.5*lt->a1 + (i+0.5)*lt->b1 + lt->c1 + y0;

		x = lt0.c0 - lt0.a0;
		y = lt0.c1 - lt0.a1;

		for( j = 0 ; j < width ; j++, tp++ ){

			x += lt0.a0;
			y += lt0.a1;


			ix = x-0.5;
			iy = y-0.5;

			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				continue;
			}


			sp = IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			s1 = *sp + dx * (  *(sp+1) - *sp );
			sp += sim->column;
			s2 = *sp + dx * (*(sp+1) - *sp );

			s = s1 + dy * (s2 - s1 ) + 0.5;

			tmp = s  - (int) (*tp);
			sum += tmp*tmp;
			sum1 += tmp;

			nSum ++;
		}
	}

	//	sum /= nSum;
	*ssd = sum/nSum;
	*av = sum1/nSum;

	return( *ssd );
}


float
image1_interpolation_lt2_ssdO( image_type *sim, lt2_type *lt,
							int x0, int y0,
							image_type *im, int j1, int i1,
							int width, int height, float *av, float *ssd )
{
u_char	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;
int	ix,	iy;
float	s1,	s2;
int	s;
float	tmp,	sum,	sum1;
int	nSum,	align;


	sum = 0;
	sum1 = 0;
	nSum = 0;

	tp = (u_char *)im->data;
	tp = IMAGE_PIXEL( im, i1, j1 );
	align = im->width - width;


	for( i = 0 ; i < height ; i++, tp += align ){

		for( j = 0 ; j < width ; j++, tp++ ){

			x = LT2_F1( *lt, j+0.5, i+0.5 ) + x0;
			y = LT2_F2( *lt, j+0.5, i+0.5 ) + y0;



			ix = x-0.5;
			iy = y-0.5;


			//			if( iy < 0 || iy > sim->row-1 ||  ix < 0 || ix > sim->column-1 ){
			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				continue;
			}


			sp = IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;

			tmp = s  - (int) (*tp);
			sum += tmp*tmp;
			sum1 += tmp;

			nSum ++;
		}
	}

//	sum /= nSum;
	*ssd = sum/nSum;
	*av = sum1/nSum;

	return( *ssd );
}



image_type *
image1_interpolation_Alt2( image_type *sim, lt2A_type *alt, float f, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;
float	x,	y,	dx,	dy;//,	ds;
int	ix,	iy;
float	s1,	s2;
int	s;


	im = image_recreate( im, sim->height, sim->width, 1, 1 );


	tp = (u_char *)im->data;
	for( i = 0 ; i < im->row ; i++ ){

		for( j = 0 ; j < im->column ; j++ ){


			lt2A_value( alt, (j+0.5)/f, (i+0.5)/f, &x, &y );

			x *= f;
			y *= f;



			ix = x-0.5;
			iy = y-0.5;


			//			if( iy < 0 || iy > sim->row-1 ||  ix < 0 || ix > sim->column-1 ){
			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				continue;
			}


			sp = IMAGE_PIXEL( sim, iy, ix );

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			s1 = (1-dx)* (*sp) + dx * (*(sp+1));
			sp += sim->column;
			s2 = (1-dx)* (*sp) + dx * (*(sp+1));

			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = PUSH_TO_RANGE(s, 0, 255 );
		}
	}

	return( im );
}
