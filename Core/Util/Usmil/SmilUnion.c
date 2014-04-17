#include	<stdlib.h>
#include	<string.h>
#include	"SmilType.h"








void	smil_append( smil_type *s, smil_type *s1, int index  );

void	smilSeq_append( smilSeq_type *seq, smilSeq_type	*seq1, int index );

smilPar_type *	smilPar_copy( smilPar_type *par, int index );

smilAttach_type *	smilAttach_copy( smilAttach_type *att, int index );






smil_type	*
smil_union( smil_type *s0, smil_type *s1 )
{
smil_type	*s;

	s = smil_create( s0->regionNo + s1->regionNo, 1 );

	s->width = s0->width;
	s->height = s0->height;

	if( s->width < s1->width )	s->width = s1->width;
	if( s->height < s1->height )	s->height = s1->height;


	smil_add_seq( s, s0->seq[0]->parNo + s1->seq[0]->parNo );

	smil_append( s, s0, 0 );
	smil_append( s, s1, 1 );

	return( s );
}


void
smil_append( smil_type *s, smil_type *s1, int index )
{
smilRegion_type *r;
char	id[256];
int	top,	left,	i;

	for( i = 0 ; i < s1->regionNo ; i++ ){
		top = ( s->height - s1->height)/2;
		left = ( s->width - s1->width)/2;

		r = s1->region[i];
		sprintf( id, "%s%d", r->id, index );
		smil_add_region( s, id, r->width, r->height, r->top + top, r->left + left );
	}

	smilSeq_append( s->seq[0], s1->seq[0], index );
}


void
smilSeq_append( smilSeq_type *seq, smilSeq_type	*seq1, int index )
{
int	i;

	for( i = 0 ; i < seq1->parNo ; i++ ){
		seq->par[ seq->parNo++] = smilPar_copy( seq1->par[i], index );
	}
}


smilPar_type *
smilPar_copy( smilPar_type *par, int index )
{
smilPar_type	*p;
int	i;

	p = smilPar_alloc();

	p->duration = par->duration;
	
	p->attach  = ( smilAttach_type **) malloc( sizeof(smilAttach_type * ) * par->attachNo );
	p->attachNo = 0;


	for( i = 0 ; i < par->attachNo ; i++ )
		p->attach[ p->attachNo++ ] = smilAttach_copy( par->attach[i], index );

	return( p );
}


smilAttach_type *
smilAttach_copy( smilAttach_type *att, int index )
{
smilAttach_type	*attach;
char	regId[256];

	attach = smilAttach_alloc();

	attach->type = att->type;
	attach->height = att->height;
	attach->width = att->width;
	attach->reference = att->reference;

	strncpy( attach->src, att->src, MAX_SRC_NAME );

	sprintf( regId, "%s%d",  att->regId, index );

	strncpy( attach->regId, regId, MAX_REGION_ID );


	return( attach );
}

