/*******************
 *** LnWrite.c   ***
 *******************/

#include	<string.h>

#include	"PlnType.h"



static int	plnA_read( plnA_type **apl, FILE *fp );

//static int	pln_read( pln_type **pl,  FILE *fp );
static int	plnA_read_cl( plnA_type **apl, FILE *fp );




//#define	PL_VERSION	2

#ifdef _AA_
int
plnF_read( plnF_type **vpl, char *file )
{
FILE	*fp;
int	i,	version,	nV;
char	signature[64];
int	pose;


	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	pose = ftell( fp );
	if( fscanf(fp, "%s  %d", signature, &version ) != 2 ){
		fclose( fp );
		return( -1 );
	}


	if( strcmp( signature, "PLV") != 0 ){
		fscanf(fp, "%d", &nV );
	}
	else 
		if( strcmp( signature, "PL") != 0 ){
			fseek( fp, pose, SEEK_SET );
			nV = 1;
		}


#ifdef _AA_
	if( strcmp( signature, "PLV") != 0 ){
		fclose( fp );
		if( strcmp( signature, "PL") != 0 ){
			plnA_type	*apl;
			plnA_read( file, &apl );
			*vpl = plnF_alloc( 1 );
			plnF_add( *vpl, apl, apl->iFrame );

			return( 1 );
		}

		return( NULL );
	}

	fscanf(fp, "%d", &nV );
#endif;


	*vpl = plnF_alloc( nV );


	plnA_type *apl;
	for( i = 0 ; i < nV ; i++ ){
		plnA_read( &apl, fp );

		plnF_add( *vpl, apl, apl->iFrame );
	}

	fclose( fp );

	return( 1 );
}
#endif

int
plnF_read( plnF_type **vpl, char *file )
{
	FILE	*fp;
	int	version,	nV;
	char	signature[64];
	int	pose,	i;


	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	pose = ftell( fp );
	if( fscanf(fp, "%s  %d  %d", signature, &version, &nV ) != 3 ){
		fclose( fp );
		return( -1 );
	}

	if( strcmp( signature, "CL") == 0 ){
		*vpl = plnF_alloc( 1 );

		fseek( fp, pose, SEEK_SET );
		plnA_read_cl( &(*vpl)->a[0], fp );
		(*vpl)->nA = 1;
		fclose( fp );
		return( 1 );
	}


	if( strcmp( signature, "PLV") != 0 ){

		if( strcmp( signature, "PL") != 0 ){
			fclose( fp );
			return( -1 );
		}

		fseek( fp, pose, SEEK_SET );
		nV = 1;
	}
//#endif

	//if( strcmp( signature, "PLV") != 0 ){
	//	fclose( fp );
	//	return( -1 );
	//}


	*vpl = plnF_alloc( nV );


	plnA_type *apl;
	for( i = 0 ; i < nV ; i++ ){
		int ret = plnA_read( &apl, fp );
		if( ret == -1 )
			break;

		if( ret < 0 )
			continue;

		plnF_add( *vpl, apl, apl->iFrame );
	}


	fclose( fp );


	return( 1 );
}



int
plnA_read( plnA_type **apl, char *file )
{
	FILE	*fp;
	int	version,	nV;
	char	signature[64];
	int	pose;


	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	pose = ftell( fp );
	if( fscanf(fp, "%s  %d  %d", signature, &version, &nV ) != 3 ){
		fclose( fp );
		return( -1 );
	}

	if( strcmp( signature, "CL") == 0 ){
		fseek( fp, pose, SEEK_SET );
		plnA_read_cl( apl, fp );
		fclose( fp );
		return( 1 );
	}
		

	if( strcmp( signature, "PLV") != 0 ){

		if( strcmp( signature, "PL") != 0 ){
			fclose( fp );
			return( -1 );
		}

		fseek( fp, pose, SEEK_SET );
	}

	int ret = plnA_read( apl, fp );

	fclose( fp );

	return( ret );
}




#ifdef _AA_
int
plnA_read( char *file, plnA_type **apl )
{
	int	i,	version,	nV;
	char	signature[64];
	FILE	*fp;

	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );


	if( strcmp( signature, "PLV") != 0 ){
		fscanf(fp, "%d", &nV );
	}
	else 
		if( strcmp( signature, "PL") != 0 ){
			fseek( fp, pose, SEEK_SET );
			nV = 1;
		}



	int ret = plnA_read( apl, fp );


	fclose( fp );

	return( ret );

}
#endif

static int
plnA_read( plnA_type **apl, FILE *fp )
{
	int	i,	version,	nPl;
	char	signature[64],	buf[256];
	pln_type	*pl;



	if( fscanf(fp, "%s  %d", signature, &version ) != 2 )
		return( -1 );


	if( strcmp( signature, "PL") != 0 )
		return( -1 );

	int iFrame = 0;
	if( version >=2 )
		fscanf(fp, "%d", &iFrame );
	
	fscanf(fp, "%d", &nPl );

	//if( nPl <= 0 )
	//	return( -2 );



	*apl = plnA_alloc( nPl+10 );


	if( version >= 4 )
		fscanf( fp, "%s  %f %f %f %f", buf, &(*apl)->p.x, &(*apl)->p.y, &(*apl)->scale, &(*apl)->angle );


	//if( nPl <= 0 ){
	//	plnA_destroy( *apl );
	//	return( -2 );
	//	return( 1 )
	//}


	(*apl)->iFrame = iFrame;

	for( i = 0 ; i < nPl ; i++ ){
		if( pln_read( &pl, fp ) < 0 )
			break;

		(*apl)->a[(*apl)->nA++ ] = pl;
	}


	return( 1 );
}


static int
plnA_read_cl( plnA_type **apl, FILE *fp )
{
	int	i,	version,	nPl;
	char	signature[64];
	pln_type	*pl;



	if( fscanf(fp, "%s  %d   %d", signature, &version, &nPl ) != 3 )
		return( NULL );

	if( strcmp( signature, "CL") != 0 )
		return( NULL );


	int iFrame = 0;
	nPl = 1;
	*apl = plnA_alloc( nPl+10 );

	(*apl)->iFrame = iFrame;



#ifdef _AA_
contour
	0
	transform
	416.389282  667.000000  1.000000   0.000000
#endif
	char	buf[256];
	int	type;
	fscanf(fp, "%s  %d", buf, &type );
	fscanf( fp, "%s  %f %f %f %f", buf, &(*apl)->p.x, &(*apl)->p.y, &(*apl)->scale, &(*apl)->angle );


	for( i = 0 ;  ; i++ ){
		if( pln_read( &pl, fp ) < 0 )
			break;

		pl->ctr.x += (*apl)->p.x;
		pl->ctr.y += (*apl)->p.y;

		//(*apl)->a[(*apl)->nA++ ] = pl;

		plnA_add( *apl, pl );
	}


	return( 1 );
}




int
pln_read( pln_type **pl,  FILE *fp )
{
ln_type	*l;
int	no,	type;
char	line[512];

	while( 1 ){
		if( fgets( line, 512, fp ) == NULL )
			return( -1 );

		if( gp_strnicmp(line , "contour", 7) == 0 )
			return( -2 );
		
		if( gp_strnicmp(line , "polylink", 8) == 0 )
			break;
	}


	fscanf( fp, "%d", &no );

	*pl = pln_alloc( no );



	fscanf( fp, "%d   %d", &(*pl)->state, &type );

	if( type & 0x02 )	fscanf( fp, "%X   %X", &(*pl)->color[0].val, &(*pl)->color[1].val );
	if( type & 0x04 )	fscanf( fp, "%d", &(*pl)->group );


	if( type & 0x08 )
		fscanf( fp, "  %f  %f   %f", &(*pl)->qulity, &(*pl)->height, &(*pl)->size );


	fscanf( fp, "%f  %f", &(*pl)->ctr.x, &(*pl)->ctr.y );

	int	anchor;
	for( l = (*pl)->link ; l != NULL ; l = LN_NEXT(l) ){
		fscanf( fp, "%f  %f  %f", &l->v.x, &l->v.y, &l->a );
		if( type == 1 )
			fscanf( fp, "%d", &anchor );
		ln_set_aux( l );
	}

	(*pl)->len = lnL_length( (*pl)->link );

	return( 1 );
}

#ifdef _AA_
static pln_type *
pln_read( FILE *fp )
{
pln_type *pl;
	ln_type	*l;
	int	no,	type;
	char	line[512];

	while( 1 ){
		if( fgets( line, 512, fp ) == NULL )
			return( NULL );
		if( gp_strnicmp(line , "polylink", 8) == 0 )
			break;
	}



	fscanf( fp, "%d", &no );

	pl = pln_alloc( no );



	fscanf( fp, "%d   %d", &pl->state, &type );

	fscanf( fp, "%f  %f", &pl->ctr.x, &pl->ctr.y );

	for( l = pl->link ; l != NULL ; l = LN_NEXT(l) ){
		fscanf( fp, "%f  %f  %f", &l->v.x, &l->v.y, &l->a );
		ln_set_aux( l );
	}

	pl->len = lnL_length( pl->link );

	return( pl );
}
#endif