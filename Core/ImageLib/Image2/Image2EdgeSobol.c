 /*****************************
 ***	Image2EdgeSobol.c	***
 ******************************/
#include <string.h>
#include	"ImageType/ImageType.h"
#include	"Image2Tool.h"

//#define _USE_ASM

#ifndef _USE_ASM
image_type *
image2_gridient_sobol( image_type *sim, image_type *tim )
{
short	*sp,	*sp0,	*sp2;
short	*tp;
int	i,	j;
int	cx,	cy;



	if( tim == NULL ){
		tim = image_alloc( sim->row, sim->column, 2, IMAGE_TYPE_S12, 1 );
		image2_const( tim, 0 );
	}


	sp = sim->data_s;
	tp = tim->data_s;



	
	sp += sim->column+1;
	tp += 2*(sim->column+1);

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 4 ){

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++ ){
			

			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));


			cx >>= 3;
			cy >>= 3;

			*tp++ = cx;
			*tp++ = cy;
		}
	}

	return( tim );
}



image_type *
image2_gridient_sobol_value( image_type *sim, image_type *tim )
{
	short	*sp,	*sp0,	*sp2;
	short	*tp;
	int	i,	j;
	int	cx,	cy,	val;


	if( tim == NULL ){
		tim = image_alloc( sim->column, sim->row, 1, IMAGE_TYPE_S12, 1 );
		image1_const( tim, 0 );
	}


	sp = sim->data_s;
	tp = tim->data_s;




	sp += sim->column+1;
	tp += sim->column+1;

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 2 ){

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++ ){


			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));

			cx >>= 3;
			cy >>= 3;


			val = ABS(cx) + ABS(cy );

//			*tp++ = PUSH_TO_RANGE( val, 0, 256 );
			*tp++ = val;
		}
	}

	return( tim );
}
#endif

image_type *
image2_gridient_sobol3( image_type *sim, image_type *tim )
{
	short	*sp,	*sp0,	*sp2;
	short	*tp;
	int	i,	j;
	int	cx,	cy;


	tim = image_realloc( tim, sim->row, sim->column, 3, IMAGE_TYPE_S12, 1 );



	sp = sim->data_s;
	tp = tim->data_s;




	sp += sim->column+1;
	tp += 3*(sim->column+1);

	for( i = 1 ; i < sim->row-1 ; i++, sp += 2, tp += 6 ){

		sp0 = sp - sim->column;
		sp2 = sp + sim->column;
		for( j = 1 ; j < sim->column-1 ; j++, sp++, sp0++, sp2++ ){


			cx = ((*(sp0+1) - *(sp0-1)) + 2*(*(sp+1) -  *(sp-1)) + (*(sp2+1) - *(sp2-1)));
			cy = ((*(sp2-1) - *(sp0-1)) + 2*(*(sp2+0) - *(sp0+0)) + (*(sp2+1) - *(sp0+1)));


			cx >>= 3;
			cy >>= 3;

			*tp++ = cx;
			*tp++ = cy;
			*tp++ = hypotA( cx, cy );
		}
	}

	return( tim );
}


#ifdef _USE_ASM
image_type *
image2_gridient_sobol( image_type *sim, image_type *tim )
{
	short	*sp;
	short	*tp,*p,*q;
	int	i,	width,height;




	if( tim == NULL ){
		tim = image_alloc( sim->row, sim->column, 2, IMAGE_TYPE_S12, 1 );
		image2_const( tim, 0 );
	}

	width=sim->column;
	height=sim->row;

	sp = (u_short *)sim->data;
	tp = (u_short *)tim->data;


	p=sp;
	q=tp+width*2;
	*q=0;
	*(q+1)=0;
	memset(tp,0,width*4);
	memset(tp+(height-1)*width,0,width*4);
	for(i=1;i<height-1;i++)
	{
		__asm
		{

			mov edx,p
				mov ecx,width
				add ecx,ecx
				lea esi,[edx+ecx];
			lea ebx,[esi+ecx];
			shr ecx,4
				mov edi,q
				add edi,4


loop_on_one_row:


			movdqu xmm0,[edx] 


			movdqu xmm1,[edx+4]

			movdqu xmm2,[esi]


			movdqu  xmm3,[esi+4]



			movdqa xmm6,xmm1
				psubw xmm1,xmm0   // A


				movdqu xmm4,[ebx]
			paddw xmm0,xmm6   //B

				movdqu xmm5,[ebx+4]
			psubw xmm3,xmm2   //C


				movdqu xmm6,[edx+2] 
			psllw xmm3,1   // 2*C
				add edx,16


				movdqu xmm7,[ebx+2]
			movdqa xmm2,xmm5
				psubw  xmm5,xmm4    // D

				paddw  xmm3,xmm1  // A+2*C
				paddw  xmm4,xmm2    //E


				psllw  xmm6,1  //F
				paddw  xmm5,xmm3  // A+2*C+D


				paddw  xmm7,xmm7    //G
				paddw  xmm6,xmm0   // (B+F)
				psraw xmm5,3    // cx>>3


				paddw  xmm7,xmm4   // (E+G)
				movdqa xmm2,xmm5


				//pabsw   xmm5,xmm5  //abs(cx) 

				add ebx,16
				psubw xmm7,xmm6    // (E+G)-(B+F)


				psraw xmm7,3    // cy>>3
				add esi,16

				punpcklwd xmm5,xmm7

				// pabsw xmm7,xmm7   // abs cy

				// paddw xmm7,xmm5   // [abs(cy)+abs(cx)]
				movdqu[edi],xmm5


				punpckhwd xmm2,xmm7 

				movdqu[edi+16],xmm2


				add edi,32
				sub ecx,1
				jne loop_on_one_row
		}

		p+=width;
		q+=width*2;
		*(q-2)=0;
		*(q-1)=0;
		*q=0;
		*(q+1)=0;

	}

	return tim;

}


image_type *
image2_gridient_sobol_value( image_type *sim, image_type *tim )
{
	u_short	*sp,*tp,	*p,*q;
	int	i,	width,	height;

	width=sim->column;

	height=sim->row;

	if( tim == NULL ){
		tim = image_alloc( sim->column, sim->row, 1, IMAGE_TYPE_S12, 1 );
		image2_const( tim, 0 );
	}
	sp = (u_short *)sim->data;
	tp = (u_short *)tim->data;


	p=sp;
	q=tp+width;
	*q=0;
	memset(tp,0,width*2);

	for(i=1;i<height-1;i++)
	{
		__asm
		{

			mov edx,p
				mov ecx,width
				add ecx,ecx
				lea esi,[edx+ecx];
			lea ebx,[esi+ecx];
			shr ecx,4
				mov edi,q
				add edi,2


loop_on_one_row:


			movdqu xmm0,[edx] 


			movdqu xmm1,[edx+4]

			movdqu xmm2,[esi]


			movdqu  xmm3,[esi+4]



			movdqa xmm6,xmm1
				psubw xmm1,xmm0   // A


				movdqu xmm4,[ebx]
			paddw xmm0,xmm6   //B

				movdqu xmm5,[ebx+4]
			psubw xmm3,xmm2   //C


				movdqu xmm6,[edx+2] 
			psllw xmm3,1   // 2*C
				add edx,16


				movdqu xmm7,[ebx+2]
			movdqa xmm2,xmm5
				psubw  xmm5,xmm4    // D

				paddw  xmm3,xmm1  // A+2*C
				paddw  xmm4,xmm2    //E


				psllw  xmm6,1  //F
				paddw  xmm5,xmm3  // A+2*C+D

				paddw  xmm7,xmm7    //G
				paddw  xmm6,xmm0   // (B+F)
				psraw xmm5,3    // cx>>3


				paddw  xmm7,xmm4   // (E+G)
				pabsw   xmm5,xmm5  //abs(cx) 

				add ebx,16
				psubw xmm7,xmm6    // (E+G)-(B+F)


				psraw xmm7,3    // cy>>3
				add esi,16

				pabsw xmm7,xmm7   // abs cy

				paddw xmm7,xmm5   // [abs(cy)+abs(cx)]





				movdqu[edi],xmm7
				add edi,16
				sub ecx,1
				jne loop_on_one_row
		}

		p+=width;
		q+=width;
		*(q-1)=0;
		*q=0;

	}
	memset(tp+(height-1)*width,0,width*2);


	return( tim );
}

#endif