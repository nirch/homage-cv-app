/***********************
 ***	Pt2dTool.c   ***
 ***********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix2Type.h"

#include	"Pt2dType.h"



int
pt2dA_write_to_file( pt2dA_type *av, char *file )
{
FILE	*fp;
pt2d_type	*pt;
int	i;

	if( (fp = fopen( file, "wb" ) ) == NULL )
		return( -1 );


	if( av->type == PT2D_2 ){
		if( av->axis == PT2D_AXIS_XY )
			fprintf( fp, "PT2-XY   %d\n", av->nP );
		else
			fprintf( fp, "PT2-YX   %d\n", av->nP );

		for( i = 0 ; i < av->nP ; i++ ){
			pt = &av->p[i];
			fprintf( fp, "%f %f\n", pt->p.x, pt->p.y );
		}
	}


	if( av->type == PT2D_4 ){
		if( av->axis == PT2D_AXIS_XY )
			fprintf( fp, "PT4-XY   %d\n", av->nP );
		else
			fprintf( fp, "PT4-YX   %d\n", av->nP );


		for( i = 0 ; i < av->nP ; i++ ){
			pt = &av->p[i];
			fprintf( fp, "%f %f %f %d \n", pt->p.x, pt->p.y, pt->r, pt->group );
		}
	}


	if( av->type == PT2D_4V ){
		if( av->axis == PT2D_AXIS_XY )
			fprintf( fp, "PT4V-XY   %d\n", av->nP );
		else
			fprintf( fp, "PT4V-YX   %d\n", av->nP );

		for( i = 0 ; i < av->nP ; i++ ){
			pt = &av->p[i];
			fprintf( fp, "%f %f %f %f  %f  %d \n", pt->p.x, pt->p.y, pt->n.x, pt->n.y, pt->r, pt->group );
		}
	}


	fclose( fp );

	return( 1 );
}


int
pt2dA_read_from_file( pt2dA_type **av, char *file )
{
FILE	*fp;
pt2d_type	*pt;
char	signature[64];
int	i,	N;


	if( (fp = fopen( file, "rb" ) ) == NULL )
		return( -1 );


	fscanf( fp, "%s   %d\n", signature, &N );

	*av = pt2dA_alloc( N );



	if( gp_strnicmp( signature, "PT4V", 4) == 0 ){
		(*av)->type = PT2D_4V;

		if( gp_stricmp( &signature[4], "-XY") == 0 )
			(*av)->axis = PT2D_AXIS_XY;

		if( gp_stricmp( &signature[4], "-YX") == 0 )
				(*av)->axis = PT2D_AXIS_YX;


		for( i = 0 ; i < N ; i++ ){
			pt = &(*av)->p[i];
			if( fscanf( fp, "%f %f %f %f  %f %d",
				&pt->p.x, &pt->p.y, &pt->n.x, &pt->n.y, 
				&pt->r, &pt->group ) == EOF )
				break;
			pt->f = pt->r;

			pt->id = 0;
		}


		fclose( fp );

		(*av)->nP = i;

		pt2dA_nGroup( *av );

		return( 1 );
	}


	if( gp_strnicmp( signature, "PT4", 3) == 0 ){
		(*av)->type = PT2D_4;

		if( gp_stricmp( &signature[3], "-XY") == 0 )
			(*av)->axis = PT2D_AXIS_XY;

		if( gp_stricmp( &signature[3], "-YX") == 0 )
			(*av)->axis = PT2D_AXIS_YX;


		for( i = 0 ; i < N ; i++ ){
			pt = &(*av)->p[i];
			if( fscanf( fp, "%f %f %f %d", &pt->p.x, &pt->p.y, &pt->r, &pt->group ) == EOF )
				break;
			pt->f = pt->r;
			pt->id = 0;
		}

		fclose( fp );

		(*av)->nP = i;

		pt2dA_nGroup( *av );

		return( 1 );
	}


	if( gp_strnicmp( signature, "PT2", 3) == 0 ){
		(*av)->type = PT2D_2;

		if( gp_stricmp( &signature[3], "-XY") == 0 )
			(*av)->axis = PT2D_AXIS_XY;

		if( gp_stricmp( &signature[3], "-YX") == 0 )
			(*av)->axis = PT2D_AXIS_YX;


		for( i = 0 ; i < N ; i++ ){
			pt = &(*av)->p[i];
			if( fscanf( fp, "%f %f", &pt->p.x, &pt->p.y ) == EOF )
				break;
			 pt->r = 0;
			 pt->group = 0;
			 pt->id = 0;
		}


		fclose( fp );

		(*av)->nP = i;

		pt2dA_nGroup( *av );

		return( 1 );
	}





	return( -1 );
}




int
pt2dA_write_to_file_V( vec2f_type p[], int nP, char *file )
{
	pt2dA_type *apt = pt2dA_from_point( p, nP );

	pt2dA_write_to_file( apt, file );

	pt2dA_destroy( apt );

	return( 1 );
}



void	
pt2dA_dump( pt2dA_type *apt, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".pt", file ) < 0 )
		return;


	pt2dA_write_to_file( apt, file );
}


void	
pt2dA_dump_V( vec2f_type av[], int nV, char *prefix, int index, char *suffix )
{
	char	file[256];

	if( gpDump_filename( prefix, index, suffix, ".pt", file ) < 0 )
		return;


	pt2dA_write_to_file_V( av, nV, file );
}