
#include	<math.h>
#include	<string.h>

#include	"ImageType/ImageType.h"
#include	"Image2Tool.h"


//#define	RC	32



#define	RC	512



image_type *
imageS_bilateral( image_type *sim, float sigma_x, float sigma_i, image_type *im )
{
	float	g[RC],	gi[512+1];
	int	nG,	nGi;
	int	i,	j,	n,	k,	ni,	ki,	di;
	short	*sp,	*sp0,	*sp00,	*tp;

	int	align;
	float	sum,	wSum,	w;


	nG = (int)(3*sigma_x);
	if( nG < 1 )	nG = 1;
	guasian( nG, sigma_x, g );

	nGi = 256;
	guasian( nGi, sigma_i, gi );



//	im = image_recreate( im, sim->height, sim->width, 2, 1 );

	im = image_reallocL( sim, im );


	align = im->width - ( 2*nG +1);

	sp = sim->data_s;
	tp = im->data_s;
	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < im->column ; j++, sp++, tp++ ){


			sum = wSum = 0;
			sp00 = sp - nG*sim->width - nG;
			for( k = 0, ki = i -nG ; k < 2*nG+1 ; k++, ki++, sp00 += im->width ){
				if( ki < 0 || ki >= im->height )	continue;
				sp0 = sp00;
				for( n = 0, ni = j -nG ; n < 2*nG+1 ; n++, ni++, sp0++ ){
					if( ni < 0 || ni >= im->width )	continue;

					di = ( (256 <<4) + *sp0 - *sp)>>4;

					w = g[k]*g[n] * gi[di];

					sum += *sp0 * w;
					wSum += w;
				}
			}

			*tp = sum / wSum;
		}
	}

	return( im );
}



#ifdef _ASM_
image_type *
imageS_bilateral_asm( image_type *sim, float sigma_x, float sigma_i, image_type *im )
{
	float	g[RC],	gi[512+1];
	int		agi[512+1];
	int		*g2,	*gp;
	int	nG,	nGi,	nG2;
	int	i,	j;
	short	*sp,	*sp0,	*sp00,	*tp,*g3,*gg,*Buff,*Buff1,*Buff2;

	int	align;
	int	temp;
	static unsigned char LUT1[16*8+16];
	unsigned char *LUT;


	nG = (int)(3*sigma_x);

	if( nG < 1 )	nG = 1;
	guasian( nG, sigma_x, g );
	Buff=(short*)malloc( (sim->column+nG*2)* (sim->row+nG*2)*sizeof(short) );//nahum
	g2 = (int *)malloc( (nG*2+1)*(2*nG+1)*sizeof(int) );
	g3=  (short*)malloc(32*31*sizeof(short)+8);  //nahum

	temp=(int)g3;
	temp=((temp>>4)<<4)+16;
	gg=(short*)temp;
	memset(Buff,0,(sim->column+nG*2)* (sim->row+nG*2)*sizeof(short));

	temp=(int)LUT1;
	temp=((temp>>4)<<4)+16;
	LUT=(unsigned char*)temp;

	nG2 = 2*nG+1;
	for( i = 0, gp = g2 ; i < nG2 ; i++ )
		for( j = 0 ; j < nG2 ; j++, gp++ )
			*gp = g[i]*g[j]*4096;
	gp=g2;
	tp=gg;
	for(i=0;i<31;i++)
	{
		for(j=0;j<31;j++)
		{
			*tp++ =(short)*gp++;
		}
		*tp++=0;
	}


	nGi = 256;
	guasian( nGi, sigma_i, gi );

	for( i = 0 ; i < 2*nGi+1 ; i++ )
		agi[i] = gi[i]*256;

	for( i=0;i<16;i++)  //nahum
		LUT[15-i]=(unsigned char)(agi[256-i]);
	for( i=16;i<32;i+=2)
	{
		LUT[i]=0;
		LUT[i+1]=1;
	}
	for( i=32;i<48;i+=2)
	{
		LUT[i]=15;
		LUT[i+1]=0;
	}



//	im = image_recreate( im, sim->height, sim->width, 2, 1 );

	im = image_reallocL( sim, im );


	align = im->width - ( 2*nG +1);

	sp = sim->data_s;
	tp = im->data_s;

	sp0=sp;
	Buff1=Buff+nG*(sim->column+nG*2)+nG;
	for( i = 0 ; i < im->row ; i++ )
	{
		memcpy(Buff1,sp0,sim->column*sizeof(short));
		Buff1+=(sim->column+nG*2);
		sp0+=sim->column;
	}


	sp = sim->data_s;
	tp = im->data_s;
	temp=(sim->column+nG*2);

	Buff1=Buff+temp*nG+nG;
	Buff2=Buff+3*temp;


	for( i = 0 ; i < im->row ; i++ )
	{
		sp0 =Buff1+i*temp;
		sp00=Buff2+i*temp;
		for( j = 0 ; j < im->column ; j++, sp0++, tp++,sp00++ )
		{
			//	sum = wSum = 0;

			__asm
			{
				mov esi,dword ptr[sp00]
				mov edx,dword ptr[sp0]
				mov ebx,dword ptr[temp]
				add ebx,ebx
					mov ecx,25    // 31 -6  first 3 and last 3 rows are zeros
					mov edi,dword ptr[gg]
				add edi,3*64  // first 3 rows are zeros

					mov eax,[edx]  // *sp0

				movd   xmm5,eax // [*sp0]
					pshuflw xmm5,xmm5,0
					punpcklqdq xmm5,xmm5  
					pxor mm0,mm0   //Wsum
					pxor mm1,mm1    // sum
					mov eax,LUT


loop_on_row_filter:
				//1
				movdqu xmm0,dword ptr[esi] // [ *sp00[0] *sp00[1]  *sp00[2] *sp00[3]  *sp00[4] *sp00[5]  *sp00[6] *sp00[7] ]

				//2
				movdqa xmm6,[eax+2*16] //  [ 15 15 15 15 15 15 15 15 ]

				//3
				movdqu xmm1,dword ptr[esi+16]// [ *sp00[8] *sp00[9]  *sp00[10] *sp00[11]  *sp00[12] *sp00[13]  *sp00[14] *sp00[15] ]

				//4
				movdqa [eax+4*16],xmm0     //store  *sp0[0]
					psubw  xmm0,xmm5     //   *sp0-*sp
					//5	
					psraw xmm0,4        //  (*sp0-*sp)>>4
					movdqu xmm2,dword ptr[esi+2*16]// [ *sp00[16] *sp00[17]  *sp00[18] *sp00[19]  *sp00[20] *sp00[21]  *sp00[22] *sp00[23] ]
				//6
				movdqa [eax+5*16],xmm1   //store  *sp0[1]
					pabsw xmm0,xmm0      // ABS{  (*sp0[0]-*sp)>>4}
					psubw  xmm1,xmm5     //   *sp0[1]-*sp
					//7
					movdqa  xmm7,xmm6
					psubw  xmm6,xmm0    // 16 - ABS{  (*sp0[0]-*sp)>>4}
					psraw xmm1,4         //(*sp0[1]-*sp)>>4
					movdqu xmm3,dword ptr[esi+3*16]// [ *sp00[16] *sp00[17]  *sp00[18] *sp00[19]  *sp00[20] *sp00[21]  *sp00[22] *sp00[23] ]

				//8
				pabsw xmm1,xmm1     // ABS{  (*sp0[1]-*sp+4906)>>4 
					movdqa [eax+6*16],xmm2 //store  *sp0[2]
					movdqa xmm0,xmm7     // xmm0 =16 
					psubw xmm2,xmm5      //*sp0[2]-*sp)>>4
					//9
					psubw  xmm7,xmm1    // 16 - ABS{  (*sp0[1]-*sp)>>4 }
					movdqa  xmm1,xmm0
					psraw xmm2,4        //(*sp0[2]-*sp+4906)>>4
					movdqa xmm4,[eax]   // load LUT
				//10
				pabsw xmm2,xmm2     //  ABS{  (*sp0[2]-*sp)>>4 }
					movdqa [eax+7*16],xmm3 //store  *sp0[3]
					psubw  xmm3,xmm5    //*sp0[3]-*sp
					//11
					packsswb xmm6,xmm7
					psubw  xmm0,xmm2    // 16 - ABS{  (*sp0[2]-*sp)>>4 

					//12
					psraw xmm3,4        //(*sp0[3]-*sp)>>4
					pabsw xmm3,xmm3    //  ABS{  (*sp0[3]-*sp)>>4 }
					pshufb xmm4,xmm6    
					movdqa xmm6,[eax]   // load LUT

				//13
				movdqa xmm7,xmm4    // xmm7 first agi[0,1] ready
					psubw  xmm1,xmm3    // 16 - ABS{  (*sp0[3]-*sp+)>>4}
					pxor  xmm2,xmm2
					movdqa  xmm3,dword ptr[edi]  // gg[0]

				//14
				punpcklbw  xmm7,xmm2   // agi[0] ready
					packsswb xmm0,xmm1
					movdqa  xmm1,dword ptr[edi+1*16]//gg[1]
				//15
				punpckhbw xmm4,xmm2   // agi[1] ready 
					pshufb xmm6,xmm0    // xmm6  agi[2,3] ready
					pmullw xmm7,xmm3         // w0  
					movdqa  xmm3,dword ptr[edi+2*16]//gg[2]
				//16
				movdqa  xmm0,xmm6
					punpcklbw xmm6,xmm2  // agi[2] ready 
					pmullw xmm4,xmm1   // w1 
					movdqa  xmm1,dword ptr[edi+3*16]//gg[3]
				//17
				punpckhbw xmm0,xmm2  // agi[3] ready 
					pmullw xmm6,xmm3     // w2
					movdqa xmm2,[eax+4*16]  // sp0[0]
				//18
				pmullw xmm0,xmm1    // w3 
					movdqa xmm3,[eax+5*16]  // sp0[1]
				movdqa xmm1,xmm7
					//19
					paddw xmm1,xmm4                 // w0+w1
					//  movdqa xmm4,[eax+6*16]  // sp0[2]
					pmaddwd xmm7,xmm2      // w0*sp0
					//20
					paddw xmm1,xmm6                 // w0+w1+w2
					pmaddwd xmm4,xmm3     // w1*sp1
					movdqa xmm3,[eax+7*16]  // sp0[3]
				//21
				paddw xmm1,xmm0                 // w0+w1+w2+w3
					pmaddwd xmm6, [eax+6*16]   // w2*sp2

				//22
				pmaddwd xmm0,xmm3     // w3*sp3
					movdqa xmm2,xmm1
					punpckhqdq xmm1,xmm1
					//23
					paddw xmm2,xmm1      //  // w0+w1+w2+w3
					pxor  xmm3,xmm3
					paddd xmm4,xmm7      // w0*sp0[0]+w1*sp0[1]
					//24
					movdqa xmm1,xmm2
					paddd xmm4,xmm6      // w0*sp0[0]+w1*sp0[1]+w2*sp0[2]
					punpcklwd xmm2,xmm3  // xmm2   // w0 

					//25
					paddd xmm4,xmm0  
					movdqa  xmm3,xmm2
					punpckhqdq xmm2,xmm2
					//26
					movdqa xmm0,xmm4
					punpckhqdq xmm4,xmm4
					paddd xmm3,xmm2
					//27
					paddd xmm4,xmm0
					movdq2q mm3,xmm3
					add esi,ebx       //advence one row
					//28
					movdq2q mm4,xmm4
					paddd mm0,mm3  
					add edi,64     // advence one row
					//29
					paddd mm1,mm4    // sum*w updated
					sub ecx,1

					jne loop_on_row_filter
					movq mm3,mm0
					punpckhdq mm0,mm0
					movq mm4,mm1
					punpckhdq mm1,mm1
					paddd mm0,mm3
					paddd mm1,mm4

					movd ebx,mm0
					movd eax,mm1

					xor edx,edx
					div ebx
					mov ecx,dword ptr[tp]
				mov dword ptr[ecx],eax
			}//asm

		} // end j
	} //end i
	__asm
	{
		emms
	}   



	free( g2 );
//	free(g3); //nahum
	free(Buff);//nahum

	return( im );
}
#endif


#ifdef _DEBUG
void
image2_bilateral_step( image_type *sim, image_type *im, float beta, float sigma_x, float lamda )
{
image_type	*cim;
float	w[5];
int	wMat[50],	r,	*fp;
int	i,	j,	n,	m;
int	tmp;
short	*sp,	*sp0,	*tp,	*cp,	t0;

int	align,	align0;
int	k;

	guasian( 2, sigma_x, w );
	fp = &wMat[0];
	for( i = 0 ; i < 5 ; i++ )
		for( j = 0 ; j < 5 ; j++, fp += 2 ){

			if( i != 2 || j != 2 )
				*fp = lamda *w[i] * w[j] * 256;
			else	*fp = 0;
			*(fp+1) = - *fp;

		}


	cim = image_make_copy( sim, NULL );

	align = im->column - 5;
	align0 = 2*im->column + 2;

	for( i = 2; i < im->row-2 ; i++ ) {
		sp = (short*)IMAGE_PIXEL( im, i, 2 );
		cp = (short*)IMAGE_PIXEL( cim, i, 2 );
		tp = (short*)IMAGE_PIXEL( sim, i, 2 );
		for( j = 2; j < im->column-2 ; j++ ){


			fp = &wMat[0];
			sp0 = cp - align0;

			t0 = *cp;
			for( m = 0, r = 0 ; m < 5 ; m++, sp0 += align ){
				for( n = 0 ; n < 5 ; n++, sp0++, fp += 2 ){

					tmp = t0 - *(sp0);

//					if( tmp < 0 )	r -= *fp;
//					else	if( tmp > 0 )
//								r += *fp;


					k = ( tmp >> 31 ) &0x01;
					r += *(fp+k);

				}
			}
 
			r >>= 4;



			tmp = *sp++;
			tmp =  *tp -  ( tmp + r) *beta;
			*tp++ = tmp;

			cp++;
		}
	}


	image_destroy( cim, 1 );
}
#endif


