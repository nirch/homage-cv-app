/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __XML_PUT_H__
#define __XML_PUT_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "Ucst/CstType.h"
#include "Uvec/Vec3d.h"
#include "Uvec/Vec2d.h"


void		xmlPut_tag_string( FILE *fp, int align, char *name, char *value );

void		xmlPut_tag_int( FILE *fp, int align, char *name, int value);

void		xmlPut_tag_float( FILE *fp, int align, char *name, float value );

void		xmlPut_tag_double( FILE *fp, int align, char *name, double value );

void		xmlPut_tag_open( FILE *fp, int align, char *name );

void		xmlPut_tag_close( FILE *fp, int align, char *name );

void		xmlPut_tag_closeA( FILE *fp );


void		xmlPut_tag_open_att( FILE *fp, int align, char *name );

void		xmlPut_att_string( FILE *fp, char *name, char *value );

void		xmlPut_att_int( FILE *fp, char *name, int value, char *unit );

void		xmlPut_att_float( FILE *fp, char *name, float value, char *unit );

void		xmlPut_tag_close_att( FILE *fp );

void		xmlPut_tag( FILE *fp, int align, struct xmlTag_type *cTag );


void		xmlPut_tag_vec3f( FILE *fp, int algin, char *name, vec3f_type *p );

void		xmlPut_tag_vec2f( FILE *fp, int algin, char *name, vec2f_type *p );




	// XmlPutCst.c
void		xmlCst_tag_string( cst_type *cst, int align, char *name, char *value );

void		xmlCst_tag_int( cst_type *cst, int align, char *name, int value );

void		xmlCst_tag_float( cst_type *cst, int align, char *name, float value );

void		xmlCst_tag_double( cst_type *cst, int align, char *name, double value );

void		xmlCst_tag_open( cst_type *cst, int align, char *name );

void		xmlCst_tag_close( cst_type *cst, int align, char *name );

void		xmlCst_tag_open_att( cst_type *cst, int align, char *name );

void		xmlCst_att_string( cst_type *cst, char *name, char *value );

void		xmlCst_att_int( cst_type *cst, char *name, int value, char *unit );

void		xmlCst_att_float( cst_type *cst, char *name, float value, char *unit );

void		xmlCst_tag_close_att( cst_type *cst );

void		xmlCst_tag_closeA( cst_type *cst );

void		xmlCst_tag_vec3f( cst_type *cst, int algin, char *name, vec3f_type *p );

void		xmlCst_tag_vec2f( cst_type *cst, int algin, char *name, vec2f_type *p );











#ifdef __cplusplus
}
#endif
#endif 
