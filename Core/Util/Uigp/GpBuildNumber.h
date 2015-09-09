#ifndef __GP_BUILD_H__
#define __GP_BUILD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "igp.h"

#define	gp_buildNumber()		gp_buildNumber_encode( __DATE__, __TIME__ )

u_int	gp_buildNumber_encode( char *date_, char *time_ );

void	gp_buildNumber_decode( u_int buildNo, char *pBuild );



//int		gp_get_build_number();
//void	gp_get_build_number_string(char *pString, long lNum);
//void	gp_decode_build_number(char *pBuild, unsigned long lBuild);

#ifdef __cplusplus
}
#endif

#endif // __VIM_BUILD_H__
