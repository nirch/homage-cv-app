/**********************
 ***	 M64Type.h	***
 **********************/
  
#ifndef		_M64_TYPE_
#define		_M64_TYPE_

#include	"Uigp/igp.h"

#ifndef __SYMBIAN32__


#ifdef WIN32
typedef _int64	m64_type;
#else
typedef long long m64_type;
#endif



#define	M64_BIT( i )	((m64_type)1<<i)
#define	M64_SET( m64, i )		( m64 |= ((m64_type)1<<i) )
#define	M64_IS_SET( m64, i )	( m64 & ((m64_type)1<<i) )


#endif


#endif
