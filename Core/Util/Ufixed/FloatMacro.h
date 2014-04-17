#ifndef __FLOATMACRO_H
#define __FLOATMACRO_H

#if defined(SCALARVERSIONLONG) || defined (SCALARVERSIONSHORT)
	#if defined(SCALARVERSIONLONG)
		typedef long FIXED;
		#define FP_PLACES 8	/*number of places to the right of the binary point*/
	#elif defined(SCALARVERSIONSHORT)
		typedef short FIXED;
		#define FP_PLACES 4	/*number of places to the right of the binary point*/
	#endif
	#define FP_SCALEFACTOR (1<<FP_PLACES) /*integer to fixed point ratio*/
	#define DECIMAL_PRECISION_FACTOR 100 /* for printing values */

	/* constants */	
	#define FP_ONEHALF		(1<<(FP_PLACES-1))	/* 0.5 */
	#define FP_ONE			(1<<(FP_PLACES))	/* 1.0 */
	#define FP_TWO 			(1<<(FP_PLACES+1))	/* 2.0 */
	#define FP_FOUR			(1<<(FP_PLACES+2))	/* 4.0 */
	#define FP_ONEANDHALF	(FP_ONE+FP_ONEHALF)	/* 1.5 */
	
	/* conversions */
	#define ITOFX(x)		((x)<<FP_PLACES)		/* integer to fixed point */
	#define FTOFX(x)		((FIXED)(x*FP_SCALEFACTOR)) /* float to fixed */
	
	#define FXTOI(x)		((int)(x)>>FP_PLACES)	/* fixed to integer */
	#define FXTOS(x)		((short)(x)>>FP_PLACES)	/* fixed to short */
	#define FXTOL(x)		((long)(x)>>FP_PLACES)	/* fixed to long */
	#define FXTOF(x)		(((float)(x))/FP_SCALEFACTOR)	/* fixed to float */

	/* operators */
	#define MULFX(x,y)		(((x)*(y))>>FP_PLACES)	/* Multiply a fixed by a fixed */
	#define DIVFX(x,y)		(((x)<<FP_PLACES)/(y))	/* Divide a fixed by a fixed */
	
	/* convert fixed to ascii*/
	#define SPRINTFX(buffer,x) 	sprintf(buffer,"%ld.%ld",FXTOI(x)\
									DECIMAL_PRECISIONFACTOR*(unsigned long)\
									((x)&(FP_SCALEFACTOR-1))>>FP_PLACES)
#else
	typedef float FIXED;
	#define FP_ONEHALF		0.5F
	#define FP_ONE			1.0F
	#define FP_TWO 			2.0F
	#define FP_FOUR			4.0F
	#define FP_ONEANDHALF	1.5F

	/* conversions */
	#define ITOFX(x)		((float)(x))		/* integer to fixed point */
	#define FTOFX(x)		(x)					/* float to fixed */
	
	#define FXTOI(x)		((int)(x))			/* fixed to integer */
	#define FXTOS(x)		((short)(x))		/* fixed to short */
	#define FXTOL(x)		((long)(x))			/* fixed to long */
	#define FXTOF(x)		(x)					/* fixed to float */
	
	/* operators */
	#define MULFX(x,y)		((x)*(y))			/* Multiply a fixed by a fixed */
	#define DIVFX(x,y)		((x)/(y))			/* Divide a fixed by a fixed */
	
	/* convert fixed to ascii*/
	#define SPRINTFX(buffer,x) 	sprintf(buffer,"%ld.%ld",FXTOI(x)\
									(int)DECIMAL_PRECISIONFACTOR*(x)
#endif

#define FXADDITOFX(fx,i)	((fx) + ITOFX(i))		/* FIXED = FX+I */
#define IADDITOFX(fx,i)		(FXTOI(FXADDITOFX(fx,i))	/* INT = FX+I */

#define INDEXOFCELL(agp,offset)	IADDITOFX(agp->bgp+offset,agp->inv_bgp)\
								/* j = (v0.y + agp->bgp)*agp->inv_bgp */


#endif //__FLOATMACRO_H