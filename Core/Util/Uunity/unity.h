/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef _UNITY_

#define _UNITY_

#include	"Ujet1/Jet1Type.h"
#include	"Ujet2/Jet2Type.h"
#include	"Ujet2/Jet23Type.h"


typedef struct unity_type {
	float	t;
	float	h;
	float	a,	b;
} unity_type;



void	unity_init( unity_type u[], int no );

float	unity_weight( float t, unity_type u[], int no );



	/* UnityS2.c */

void	unity2_set_paramter( float h, float e );

float	unity2_value( float x );

void    unity2_jet1( float x, jet1_type *v );

int	unity2_jet2( jet2 *u, float x, float y );


	/* UnityS3.c */

void	unity3_set_paramter( float h, float e );

float	unity3_value( float x );

void	unity3_jet1( float x, jet1_type *v );

int	unity3_jet23( jet23 *u, float x, float y );


	/* UnityS4.c */

void	unity4_set_paramter( float h, float e );

float	unity4_value( float x );

void	unity4_jet1( float x, jet1_type *v );

int	unity4_jet23( jet23 *u, float x, float y );


#endif /* #ifndef _UNITY_ */
