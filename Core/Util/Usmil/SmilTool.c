#include	<stdlib.h>
#include	<string.h>
#include	"SmilType.h"









smil_type *
smil_alloc()
{
smil_type	*smil;

	smil = ( smil_type *)malloc( sizeof( smil_type) );
	memset( smil, 0, sizeof(smil_type));
	return( smil );
}



smilRegion_type	*
smilRegion_alloc()
{
smilRegion_type	*reg;

	reg = ( smilRegion_type *)malloc( sizeof( smilRegion_type) );
	memset( reg, 0, sizeof(smilRegion_type));
	return( reg );
}




smilPar_type *
smilPar_alloc()
{
smilPar_type	*par;

	par = ( smilPar_type *)malloc( sizeof( smilPar_type) );
	memset( par, 0, sizeof(smilPar_type));
	return( par );
}


smilSeq_type *
smilSeq_alloc()
{
smilSeq_type	*seq;

	seq = ( smilSeq_type *)malloc( sizeof( smilSeq_type) );
	memset( seq, 0, sizeof(smilSeq_type));
	return( seq );
}


smilAttach_type *
smilAttach_alloc()
{
smilAttach_type	*attach;

	attach = ( smilAttach_type *)malloc( sizeof( smilAttach_type) );
	memset( attach, 0, sizeof(smilAttach_type));
	return( attach );
}


smilParam_type *
smilParam_alloc()
{
smilParam_type	*prm;

	prm = ( smilParam_type *)malloc( sizeof( smilParam_type) );
	memset( prm, 0, sizeof(smilParam_type));
	return( prm );
}

void
smil_destroy( smil_type *smil )
{
int	i;

	for( i = 0 ; i < smil->regionNo ; i++ )
		free( smil->region[i] );
	free( smil->region );


	for( i = 0 ; i < smil->seqNo ; i++ )
		smilSeq_destroy( smil->seq[i] );
	free( smil->seq );

	free( smil );
}



void
smilSeq_destroy( smilSeq_type *seq )
{
int	i;

	for( i = 0 ; i < seq->parNo ; i++ )
		smilPar_destroy( seq->par[i] );
	free( seq->par );

	free( seq );
}


void
smilPar_destroy( smilPar_type *par )
{
int	i;

	for( i = 0 ; i < par->attachNo ; i++ )
		smilAttach_destroy( par->attach[i] );
//		free( par->attach[i] );
	free( par->attach );

	free( par );
}

void
smilAttach_destroy( smilAttach_type *att )
{
int	i;

	for( i = 0 ; i < att->paramNo ; i++ )
		free( att->param[i] );
	free( att->param );

	free( att );
}

smilAttach_type	*
smil_get_attach( smil_type *smil, char *id, char *location )
{
smilSeq_type	*seq;
smilPar_type	*par;
int	i,	j,	k;

	for( i = 0 ; i < smil->seqNo ; i++ ){
		seq = smil->seq[i];
		for( j = 0 ; j < seq->parNo ; j++ ){
			par = seq->par[j];
			for( k = 0 ; k < par->attachNo ; k++ ){
				if( par->attach[k]->reference == SMIL_ATTACH_ID ){
					if( id != NULL && gp_stricmp( par->attach[k]->src, id) == 0 )
						return( par->attach[k] );
					continue;
				}

				if( location != NULL && gp_stricmp( par->attach[k]->src, location ) == 0 )
						return( par->attach[k] );
			}
		}
	}


	return( NULL );
}

smilRegion_type	*
smil_get_region( smil_type *smil, char *regionId )
{
int	i;

	for( i = 0 ; i < smil->regionNo ; i++ )
		if( gp_stricmp( smil->region[i]->id, regionId ) == 0)
			return( smil->region[i] );

	return( NULL );
}


int
smil_get_srcNames( smil_type *smil, char *aFile[], int NO )
{
smilSeq_type	*seq;
smilPar_type	*par;
int	i,	j,	k,	no;

	for( i = 0, no = 0 ; i < smil->seqNo ; i++ ){
		seq = smil->seq[i];
		for( j = 0 ; j < seq->parNo ; j++ ){
			par = seq->par[j];
			for( k = 0 ; k < par->attachNo ; k++ ){
				aFile[no++] = strdup( par->attach[k]->src );
				if( no >= NO )	return( no );
			}
		}
	}

	return( no );
}


int
smil_get_attachs( smil_type *smil, smilAttach_type *attach[], int NO )
{
smilSeq_type	*seq;
smilPar_type	*par;
int	i,	j,	k,	no;

	for( i = 0, no = 0 ; i < smil->seqNo ; i++ ){
		seq = smil->seq[i];
		for( j = 0 ; j < seq->parNo ; j++ ){
			par = seq->par[j];
			for( k = 0 ; k < par->attachNo ; k++ ){
				attach[no++] = par->attach[k];
				if( no >= NO )	return( no );
			}
		}
	}

	return( no );
}









smil_type *
smil_create( int regionNo, int seqNo )
{
smil_type	*smil;

	smil = smil_alloc();

	smil->regionNo = 0;
	smil->region = ( smilRegion_type **) malloc( sizeof(smilRegion_type * ) * regionNo );

	smil->seqNo = 0;
	smil->seq = ( smilSeq_type **) malloc( sizeof(smilSeq_type * ) * seqNo );

	return( smil );
}


smilRegion_type *
smil_add_region( smil_type *smil, char *id, int width, int height, int top, int left )
{
smilRegion_type	*reg;

	reg = smilRegion_alloc();

	strncpy( reg->id, id, MAX_REGION_ID );
	reg->width = width;
	reg->height = height;
	reg->top = top;
	reg->left = left;

	smil->region[ smil->regionNo++ ] = reg;

	return( reg );
}
	

smilSeq_type *
smil_add_seq( smil_type *smil, int parNo )
{
smilSeq_type	*seq;
	
	seq = smilSeq_alloc();

	seq->parNo = 0;
	seq->par = ( smilPar_type **) malloc( sizeof(smilPar_type * ) * parNo );

	smil->seq[ smil->seqNo++ ] = seq;



	return( seq );
}


smilPar_type *
smilSeq_add_par( smilSeq_type *seq, int duration, int attachNo )
{
smilPar_type	*par;

	par = smilPar_alloc();

	par->duration = duration;
	
	par->attach  = ( smilAttach_type **) malloc( sizeof(smilAttach_type * ) * attachNo );
	par->attachNo = 0;

	seq->par[ seq->parNo++ ] = par;

	return( par );
}


smilAttach_type *
smilPar_add_attach( smilPar_type *par, int type, char *id, char *location, char *regId )
{
smilAttach_type	*attach;

	attach = smilAttach_alloc();

	attach->type = type;

	if( id != NULL ){
		strncpy( attach->src, id, MAX_SRC_NAME );
		attach->reference = SMIL_ATTACH_ID;
	}
	else	{
		strncpy( attach->src, location, MAX_SRC_NAME );
		attach->reference = SMIL_ATTACH_LOACTION;
	}

	if( regId != NULL )
		strncpy( attach->regId, regId, MAX_REGION_ID );

	
	par->attach[ par->attachNo++ ] = attach;

	return( attach );
}


int
smilAttach_set_src( smilAttach_type *attach, char *id, char *location )
{
	if( attach->reference == SMIL_ATTACH_ID && id != NULL){
		strncpy(attach->src, id, MAX_SRC_NAME );
		return( 1 );
	}


	if( attach->reference == SMIL_ATTACH_LOACTION && location != NULL){
		strncpy(attach->src, location, MAX_SRC_NAME );
		return( 1 );
	}

	if( id != NULL ){
		strncpy(attach->src, id, MAX_SRC_NAME );
		attach->reference = SMIL_ATTACH_ID;
		return( 1 );
	}


	if( location != NULL){
		strncpy(attach->src, location, MAX_SRC_NAME );
		attach->reference = SMIL_ATTACH_LOACTION;
		return( 1 );
	}

	return( -1 );
}