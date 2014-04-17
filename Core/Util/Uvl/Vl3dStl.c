/*****************
	Vl3dTool.c
******************/

#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"



#include "Vl3dType.h"



static void	vl3d_write_stl( vl3d_type *vl, FILE *fp );





int
vl3dA_write_stl( vl3dA_type *avl, char *file )
{
FILE *fp;
int	i;

	if( (fp = fopen( file, "wb")) == NULL )
		return( -1 );


	fprintf( fp, "solid cube_corner\n" );

	for( i = 0 ; i < avl->nA ; i++ )
		vl3d_write_stl( &avl->a[i], fp );

	fprintf( fp, "\n" );

	fclose( fp );

	return( 1 );
}




static void
vl3d_write_stl( vl3d_type *vl, FILE *fp )
{
vec3d_type	p0,	p1;

	vl3d_points( vl, &p0, &p1 );
	fprintf( fp, "  facet normal  %.4f %.4f %.4f\n", 1.0F, 0.0F, 0.0F  );
	fprintf( fp, "    outer loop\n" );

	fprintf( fp, "      vertex   %.4f %.4f %.4f\n", (float)p0.x, (float)p0.y, (float)p0.z );
	fprintf( fp, "      vertex   %.4f %.4f %.4f\n", (float)p0.x, (float)p0.y, (float)p0.z );
	fprintf( fp, "      vertex   %.4f %.4f %.4f\n", (float)p1.x, (float)p1.y, (float)p1.z );

	fprintf( fp, "    endloop\n" );

	fprintf( fp, "  endfacet\n" );

}



