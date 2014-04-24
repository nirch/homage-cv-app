#pragma once

#include <iostream>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <Windows.h>

/*
_inline
void printSSESupport()
{
	if (OSCheckSSESupport())
		std::cout << "SYSTEM Support SSE  test = success !" << std::endl ;
	else
		std::cout << "SYSTEM Support SSE  test = fail !" << std::endl ;
	if (OSCheckSSE2Support())
		std::cout << "SYSTEM Support SSE2 test = success !" << std::endl ;
	else
		std::cout << "SYSTEM Support SSE2 test = fail !" << std::endl ;
	if (OSCheckSSE3Support())
		std::cout << "SYSTEM Support SSE3 test = success !" << std::endl ;
	else
		std::cout << "SYSTEM Support SSE3 test = fail !" << std::endl ;
}
*/

/*
	size - MUST be a multiple of 8
	*im - input image
	*cim - not used
*/



static void
image2_convolution_x_MMX5( image_type *im, float C[], image_type *cim )
{
//short	*sp,	*tp,	*p;
int	i,	j;
short ker[32];
__declspec(align(16)) __m128i  walker[5],tmp;
__declspec(align(16)) __m128i  convker[5] ;
//__declspec(align(16)) unsigned short *sp;
//__declspec(align(16)) unsigned short *tp;
__declspec(align(16)) short *sp;
__declspec(align(16)) short *tp;


	for( i = 0 ; i < 5 ; i++ )
		ker[i] = C[i] * ( 1<<15);


	for( i=0 ; i < 5 ; i++ ){
		_mm_insert_epi16 (convker[i], ker[i],0);
		_mm_insert_epi16 (convker[i], ker[i],1);
		_mm_insert_epi16 (convker[i], ker[i],2);
		_mm_insert_epi16 (convker[i], ker[i],3);
		_mm_insert_epi16 (convker[i], ker[i],4);
		_mm_insert_epi16 (convker[i], ker[i],5);
		_mm_insert_epi16 (convker[i], ker[i],6);
		_mm_insert_epi16 (convker[i], ker[i],7);
	}	


	for( i = 0 ; i < im->row ; i++ ){
		sp = (short *)IMAGE_PIXEL( im, i, 0 );
		tp = (short *)IMAGE_PIXEL( cim, i, 0 );


		*tp++ = *sp;
		*tp++ = (*sp+1);

		for( j = 0 ; j < im->column ; j += 8, sp += 8, tp+= 8 ){
			walker[0] = _mm_loadu_si128 ((__m128i *)sp);
			walker[1] = _mm_loadu_si128 ((__m128i *)sp+1);
			walker[2] = _mm_loadu_si128 ((__m128i *)sp+2);
			walker[3] = _mm_loadu_si128 ((__m128i *)sp+3);
			walker[4] = _mm_loadu_si128 ((__m128i *)sp+4);
        
			tmp = _mm_mullo_epi16 (walker[0],convker[0]); //mul the 5 pixel mask
			walker[0] = _mm_mullo_epi16 (walker[1],convker[1]); //mul the 5 pixel mask
			tmp = _mm_add_epi16( tmp, walker[0]);

			walker[0] = _mm_mullo_epi16 (walker[2],convker[2]); //mul the 5 pixel mask
			tmp = _mm_add_epi16( tmp, walker[0]);

			walker[0] = _mm_mullo_epi16 (walker[3],convker[1]); //mul the 5 pixel mask
			tmp = _mm_add_epi16( tmp, walker[0]);

			walker[0] = _mm_mullo_epi16 (walker[4],convker[0]); //mul the 5 pixel mask
			tmp = _mm_add_epi16( tmp, walker[0]);

			tmp = _mm_srai_epi16 (tmp,15);
			_mm_storeu_si128((__m128i *)(tp),tmp);
		}

		
		sp += 2;
		*tp -= 8;
		*sp -= 8;
		*tp++ = *sp++;
		*tp++ = *sp++;
	}
}



#define ker1 498
#define ker2 7168
#define ker3 17435
/*__declspec(align(16)) float convKer[8] = {0.015199625,
										  0.21875173,
0.53209728,
0.21875173,
0.015199625,
0,0,0};
*/

_inline 
int image2_convolution_x( unsigned short *im, unsigned int size, unsigned short *outim )
{
	DWORD startTime= GetTickCount();
	unsigned int j=0;
	unsigned int i=0; 
	__declspec(align(16)) __m128i  walker[5],tmp;
	__declspec(align(16)) __m128i  convker[5] ;
	__declspec(align(16)) unsigned short * im_start = im;
	__declspec(align(16)) unsigned short * oim_start = outim;
	oim_start +=2;
	//register unsigned short sum=0;
	unsigned short ker[5] ={ker1,ker2,ker3,ker2,ker1};
	
	//convker = _mm_load_si128((__m128i *)convKer);
	for (i=0;i<3;i++)
	{
		_mm_insert_epi16 (convker[i], ker[i],0);
		_mm_insert_epi16 (convker[i], ker[i],1);
		_mm_insert_epi16 (convker[i], ker[i],2);
		_mm_insert_epi16 (convker[i], ker[i],3);
		_mm_insert_epi16 (convker[i], ker[i],4);
		_mm_insert_epi16 (convker[i], ker[i],5);
		_mm_insert_epi16 (convker[i], ker[i],6);
		_mm_insert_epi16 (convker[i], ker[i],7);
	}	
	
	for (i =2 ; i < size-8 ; i+=6,oim_start+=6)
	{
		walker[0] = _mm_loadu_si128 ((__m128i *)im);
		walker[1] = _mm_loadu_si128 ((__m128i *)im+1);
		walker[2] = _mm_loadu_si128 ((__m128i *)im+2);
		walker[3] = _mm_loadu_si128 ((__m128i *)im+3);
		walker[4] = _mm_loadu_si128 ((__m128i *)im+4);
        
		walker[0] = _mm_mullo_epi16 (walker[0],convker[0]); //mul the 5 pixel mask
		walker[1] = _mm_mullo_epi16 (walker[1],convker[1]); //mul the 5 pixel mask
		walker[2] = _mm_mullo_epi16 (walker[2],convker[2]); //mul the 5 pixel mask
		walker[3] = _mm_mullo_epi16 (walker[3],convker[1]); //mul the 5 pixel mask
		walker[4] = _mm_mullo_epi16 (walker[4],convker[0]); //mul the 5 pixel mask
		
		tmp = _mm_add_epi16 (walker[0],walker[1]);
		walker[0]= _mm_add_epi16 (walker[2],walker[3]);
		walker[1]= _mm_add_epi16 (walker[0],walker[4]);
		tmp = _mm_add_epi16 (tmp,walker[1]);
		tmp = _mm_srai_epi16 (tmp,15);
		 _mm_storeu_si128((__m128i *)(oim_start),tmp);
		/*sum +=_mm_extract_epi16 (tmp, 0);
		sum +=_mm_extract_epi16 (tmp, 1);
		sum +=_mm_extract_epi16 (tmp, 2);
		sum +=_mm_extract_epi16 (tmp, 3);
		sum +=_mm_extract_epi16 (tmp, 4);*/
		//*(outim+i) = tmp.m128i_u16[0] + tmp.m128i_u16[1] + tmp.m128i_u16[2] + tmp.m128i_u16[3] +tmp.m128i_u16[4];
		//*(oim_start+i) = sum;
		/*rez.m128i_u16[j++]=sum;
		//if (j==8)
		{
			 _mm_store_si128((__m128i *)(oim_start),rez);
			 oim_start+=8;
			j=0;
		}
		++im;*/
	}
	DWORD endTime= GetTickCount();	
	std::cout << "conv-x time = " <<endTime-startTime<<std::endl;
	return 0;
}



/*
	This function rotate the matrix on its diagonal axis and 
	call's image2_convolution_x (R,C)=>(C,R)
*/
_inline 
int image2_convolution_y( unsigned short *im, unsigned int size,unsigned short *outim)
{
	
	short j=0;
	__declspec(align(16)) __m128i tmpSwaperLoader;
	__declspec(align(16)) __m128i tmpSwaperStorer;

	
	__declspec(align(16)) __m128i swaperIndex; //index of the current vector location
	__declspec(align(16)) __m128i swaperLocation; //index of the needed swap locations
	__declspec(align(16)) __m128i rowSizeConst;   //the row const size
	__declspec(align(16)) __m128i vectConst;      //the vector size

	

	for (j=0 ; j<8 ; j++)
	{
		swaperIndex.m128i_u16[j]=j;
		rowSizeConst.m128i_u16[j] = 1024;
		vectConst.m128i_u16[j] = 8;
	}
	
	swaperLocation =  _mm_add_epi16 (swaperIndex, rowSizeConst);

	for (unsigned int index = 0; index < size; index +=8)
	{
		//1. load the 8 values of the row into temp vector
			tmpSwaperLoader = _mm_loadu_si128 ((__m128i *)(im+index));
			
		//2. for each value do swap with the correspondent value
			for (j=0 ; j<8 ; j++)
			{
				tmpSwaperStorer.m128i_u16[j] = *(im+index+j);
				*(im+index+j)=tmpSwaperLoader.m128i_u16[j];
			}
		//3. swap values for the x axis from the storer vector
			 _mm_store_si128((__m128i *)(im+index),tmpSwaperStorer);
		//calculate and advance to next iteration
		swaperIndex =  _mm_add_epi16 (vectConst , swaperIndex);  
		swaperLocation =  _mm_add_epi16 (swaperIndex, rowSizeConst);
	}
	
	// do an X axis convolution
	image2_convolution_x(im,size,outim);

	//finally swap back
	for (j=0 ; j<8 ; j++)
		swaperIndex.m128i_u16[j]=j; //swaperIndex back to start

	swaperLocation =  _mm_add_epi16 (swaperIndex, rowSizeConst);

	for (unsigned int index = 0; index < size; index +=8)
	{
		//1. load the 8 values of the row into temp vector
			tmpSwaperLoader = _mm_loadu_si128 ((__m128i *)(im+index));
			
		//2. for each value do swap with the correspondent value
			for (j=0 ; j<8 ; j++)
			{
				tmpSwaperStorer.m128i_u16[j] = *(im+index+j);
				*(im+index+j)= tmpSwaperLoader.m128i_u16[j];
			}
		//3. swap values for the x axis from the storer vector
			 _mm_store_si128((__m128i *)(im+index),tmpSwaperStorer);
		//calculate and advance to next iteration
		swaperIndex =  _mm_add_epi16 (vectConst , swaperIndex);  
		swaperLocation =  _mm_add_epi16 (swaperIndex, rowSizeConst);
	}
	
	return 0;
}


//worse in performance
//_inline 
//int image2_convolution_y2( unsigned short *im, unsigned int size)
//{
//	DWORD startTime= GetTickCount();
//	short j=0;
//	
//	__declspec(align(16)) __m128i  walker,tmp;
//	
////	__declspec(align(16)) __m128i swaperIndex; //index of the current vector location
////	__declspec(align(16)) __m128i swaperLocation; //index of the needed Y locations
////	__declspec(align(16)) __m128i rowSizeConst;   //the row const size
////	__declspec(align(16)) __m128i vectConst;      //the vector size
//
//	__declspec(align(16)) __m128i  convker ;
//	__declspec(align(16)) unsigned short * im_start = im;
//	unsigned short sum=0;
//	__declspec(align(16)) short convKer[8] = {2,3,4,5,6,1,1,1};
//	convker = _mm_load_si128((__m128i *)convKer);
//
//	/*for (j=0 ; j<8 ; j++)
//	{
//		swaperIndex.m128i_u16[j]=j;
//		rowSizeConst.m128i_u16[j] = 1024;
//		vectConst.m128i_u16[j] = 8;
//	}*/
//	
//	im_start = im;
//	walker.m128i_u16[0] = *im;
//	walker.m128i_u16[1] = *(im+1024);
//	walker.m128i_u16[2] = *(im+2048);
//	walker.m128i_u16[3] = *(im+3072);
//	walker.m128i_u16[4] = *(im+4096);
//	tmp = _mm_mullo_epi16 (walker,convker); //mul the 5 pixel mask 
//	sum = tmp.m128i_u16[0] + tmp.m128i_u16[1] + tmp.m128i_u16[2] + tmp.m128i_u16[3] +tmp.m128i_u16[4];
//	*im = sum;
//	im +=1024;
//	for (unsigned int col =0; col < 1024; col++)
//	{
//		
//		im += col;
//		for (j=4 ; j< 1024 ; j++)
//		{
//			walker.m128i_u16[0]= walker.m128i_u16[1];
//			walker.m128i_u16[1]= walker.m128i_u16[2];
//			walker.m128i_u16[2]= walker.m128i_u16[3];
//			walker.m128i_u16[4] = *(im+4096);
//			tmp = _mm_mullo_epi16 (walker,convker); //mul the 5 pixel mask 
//			sum = tmp.m128i_u16[0] + tmp.m128i_u16[1] + tmp.m128i_u16[2] + tmp.m128i_u16[3] +tmp.m128i_u16[4];
//			*im = sum;
//			im +=1024;
//			
//		}
//		im = im_start;
//		
//	}
//	
//	DWORD endTime= GetTickCount();	
//	std::cout << "conv-y2 time = " <<endTime-startTime<<std::endl;
//	return 0;
//}

_inline 
int image2_convolution( unsigned short *im, unsigned int size,unsigned short *outim)
{
	int ret=0;
//#ifdef _TIME_TRACER
	DWORD startTime = GetTickCount();
//#endif

	ret = image2_convolution_x(im,size,outim);
	ret = image2_convolution_y(im,size,outim);

//#ifdef _TIME_TRACER
	DWORD finishTime = GetTickCount();
	std::cout << " image2_convolution time is : " << finishTime - startTime << " ms" << std::endl;
//#endif	

	return ret;
}


