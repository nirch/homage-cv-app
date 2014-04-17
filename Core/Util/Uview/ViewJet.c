/***********************
 ***    ViewJet.c    ***
 ***********************/
#include	<math.h>
#include	"Uvec/Vec3d.h"
#include	"Ujet2/Jet2Type.h"
#include	"Ujet2/Jet23Type.h"

#include	"ViewType.h"




void
view_per_jmap22( view_type *vw, float x0, float y0,  jet2 *f, jmap22 *Q )
{
jet2	nf, nf1,	uf,	vf;
vec3d	p0;

	p0.x = x0 - vw->cp.x;
	p0.y = y0 - vw->cp.y;
	p0.z = J00(*f) - vw->cp.z;

	J00( nf ) = vw->N.x * p0.x + vw->N.y * p0.y + vw->N.z * p0.z;

	J10( nf ) = vw->N.x 	+ vw->N.z * J10( *f );
	J01( nf ) = vw->N.y	+ vw->N.z * J01( *f );

	J20( nf ) = vw->N.z * J20( *f );
	J11( nf ) = vw->N.z * J11( *f );
	J02( nf ) = vw->N.z * J02( *f );

	divide_jet2( &nf, &nf1 );



	J00( uf ) = vw->dis*( vw->U.x * p0.x + vw->U.y * p0.y + vw->U.z * p0.z);

	J10( uf ) = vw->dis*( -vw->U.x 	- vw->U.z * J10(*f) );
	J01( uf ) = vw->dis*( -vw->U.y	- vw->U.z * J01(*f) );

	J20( uf ) = vw->dis*( -vw->U.z * J20(*f) );
	J11( uf ) = vw->dis*( -vw->U.z * J11(*f) );
	J02( uf ) = vw->dis*( -vw->U.z * J02(*f) );

	product_jet2( &uf, &nf1, &Q->f1 );




	J00( vf ) = vw->dis*( vw->V.x * p0.x + vw->V.y * p0.y + vw->V.z * p0.z);

	J10( vf ) = vw->dis*( -vw->V.x 	- vw->V.z * J10(*f) );
	J01( vf ) = vw->dis*( -vw->V.y	- vw->V.z * J01(*f) );

	J20( vf ) = vw->dis*( -vw->V.z * J20(*f) );
	J11( vf ) = vw->dis*( -vw->V.z * J11(*f) );
	J02( vf ) = vw->dis*( -vw->V.z * J02(*f) );

	product_jet2( &vf, &nf1, &Q->f2 );
}




void
view_per_jmap23( view_type *vw, float x0, float y0, jet23 *f, jmap23 *Q )
{
jet23	nf, nf1,	uf,	vf;
vec3d	p0;

	p0.x = x0 - vw->cp.x;
	p0.y = y0 - vw->cp.y;
	p0.z = J00(*f) - vw->cp.z;


	J00( nf ) = vw->N.x * p0.x + vw->N.y * p0.y + vw->N.z * p0.z;

	J10( nf ) = vw->N.x 	+ vw->N.z * J10( *f );
	J01( nf ) = vw->N.y	+ vw->N.z * J01( *f );

	J20( nf ) = vw->N.z * J20( *f );
	J11( nf ) = vw->N.z * J11( *f );
	J02( nf ) = vw->N.z * J02( *f );

	J30( nf ) = vw->N.z * J30( *f );
	J21( nf ) = vw->N.z * J21( *f );
	J12( nf ) = vw->N.z * J12( *f );
	J03( nf ) = vw->N.z * J03( *f );

	divide_jet23( &nf, &nf1 );



	J00( uf ) = vw->dis*( vw->U.x * p0.x + vw->U.y * p0.y + vw->U.z * p0.z);

	J10( uf ) = vw->dis*( vw->U.x 	+ vw->U.z * J10(*f) );
	J01( uf ) = vw->dis*( vw->U.y	+ vw->U.z * J01(*f) );

	J20( uf ) = vw->dis*( vw->U.z * J20(*f) );
	J11( uf ) = vw->dis*( vw->U.z * J11(*f) );
	J02( uf ) = vw->dis*( vw->U.z * J02(*f) );

	J30( uf ) = vw->dis*( vw->U.z * J30(*f) );
	J21( uf ) = vw->dis*( vw->U.z * J21(*f) );
	J12( uf ) = vw->dis*( vw->U.z * J12(*f) );
	J03( uf ) = vw->dis*( vw->U.z * J03(*f) );

	product_jet23( &uf, &nf1, &Q->f1 );




	J00( vf ) = vw->dis*( vw->V.x * p0.x + vw->V.y * p0.y + vw->V.z * p0.z);

	J10( vf ) = vw->dis*( vw->V.x 	+ vw->V.z * J10(*f) );
	J01( vf ) = vw->dis*( vw->V.y	+ vw->V.z * J01(*f) );

	J20( vf ) = vw->dis*( vw->V.z * J20(*f) );
	J11( vf ) = vw->dis*( vw->V.z * J11(*f) );
	J02( vf ) = vw->dis*( vw->V.z * J02(*f) );

	J30( vf ) = vw->dis*( vw->V.z * J30(*f) );
	J21( vf ) = vw->dis*( vw->V.z * J21(*f) );
	J12( vf ) = vw->dis*( vw->V.z * J12(*f) );
	J03( vf ) = vw->dis*( vw->V.z * J03(*f) );

	product_jet23( &vf, &nf1, &Q->f2 );
}



void
view_per_z_jet2( view_type *vw, float x0, float y0, jet23 *f, jet2 *z )
{
vec3d	p0;

	p0.x = x0 - vw->cp.x;
	p0.y = y0 - vw->cp.y;
	p0.z = J00(*f) - vw->cp.z;


	J00( *z ) = p0.x *J10(*f) + p0.y *J01(*f) - p0.z;

	J10( *z ) = p0.x *2*J20(*f) + J10(*f) +
			p0.y *J11(*f) +
			- J10(*f);

	J01( *z ) = p0.x *J11(*f) +
			p0.y *2*J02(*f) + J01(*f) +
			- J01(*f);

	J20( *z ) =  p0.x *3*J30(*f) + 2*J20(*f) +
			p0.y*J21(*f) +
			-J20(*f);
	J11( *z ) = p0.x *2*J21(*f) + J11(*f) +
			p0.y*2*J12(*f) + J11(*f) +
			-J11(*f);
	J02( *z ) = p0.x *J12(*f) +
			p0.y*3*J03(*f) + 2*J02(*f) +
			-J02(*f);
}



void
view_per3_jmap23( view_type *vw, jet23 *X, jet23 *Y, jet23 *Z, jmap23 *Q )
{
jet23	nf, nf1,	uf,	vf;
vec3d	p0;

	p0.x = J00(*X) - vw->cp.x;
	p0.y = J00(*Y) - vw->cp.y;
	p0.z = J00(*Z) - vw->cp.z;


	J00( nf ) = vw->N.x * p0.x + vw->N.y * p0.y + vw->N.z * p0.z;

	J10( nf ) = vw->N.x*J10(*X) + vw->N.y *J10(*Y)	+ vw->N.z * J10(*Z);
	J01( nf ) = vw->N.x*J01(*X) + vw->N.y *J01(*Y)	+ vw->N.z * J01(*Z);

	J20( nf ) = vw->N.x*J20(*X) + vw->N.y *J20(*Y)	+ vw->N.z * J20(*Z);
	J11( nf ) = vw->N.x*J11(*X) + vw->N.y *J11(*Y)	+ vw->N.z * J11(*Z);
	J02( nf ) = vw->N.x*J02(*X) + vw->N.y *J02(*Y)	+ vw->N.z * J02(*Z);

	J30( nf ) = vw->N.x*J30(*X) + vw->N.y *J30(*Y)	+ vw->N.z * J30(*Z);
	J21( nf ) = vw->N.x*J21(*X) + vw->N.y *J21(*Y)	+ vw->N.z * J21(*Z);
	J12( nf ) = vw->N.x*J12(*X) + vw->N.y *J12(*Y)	+ vw->N.z * J12(*Z);
	J03( nf ) = vw->N.x*J03(*X) + vw->N.y *J03(*Y)	+ vw->N.z * J03(*Z);

	divide_jet23( &nf, &nf1 );



	J00( uf ) = vw->dis*( vw->U.x * p0.x + vw->U.y * p0.y + vw->U.z * p0.z);

	J10( uf ) = vw->dis*(vw->U.x*J10(*X)+vw->U.y*J10(*Y)+vw->U.z*J10(*Z) );
	J01( uf ) = vw->dis*(vw->U.x*J01(*X)+vw->U.y*J01(*Y)+vw->U.z*J01(*Z) );

	J20( uf ) = vw->dis*(vw->U.x*J20(*X)+vw->U.y*J20(*Y)+vw->U.z*J20(*Z) );
	J11( uf ) = vw->dis*(vw->U.x*J11(*X)+vw->U.y*J11(*Y)+vw->U.z*J11(*Z) );
	J02( uf ) = vw->dis*(vw->U.x*J02(*X)+vw->U.y*J02(*Y)+vw->U.z*J02(*Z) );

	J30( uf ) = vw->dis*(vw->U.x*J30(*X)+vw->U.y*J30(*Y)+vw->U.z*J30(*Z) );
	J21( uf ) = vw->dis*(vw->U.x*J21(*X)+vw->U.y*J21(*Y)+vw->U.z*J21(*Z) );
	J12( uf ) = vw->dis*(vw->U.x*J12(*X)+vw->U.y*J12(*Y)+vw->U.z*J12(*Z) );
	J03( uf ) = vw->dis*(vw->U.x*J03(*X)+vw->U.y*J03(*Y)+vw->U.z*J03(*Z) );


	product_jet23( &uf, &nf1, &Q->f1 );




	J00( vf ) = vw->dis*( vw->V.x * p0.x + vw->V.y * p0.y + vw->V.z * p0.z);

	J10( vf ) = vw->dis*(vw->V.x*J10(*X)+vw->V.y*J10(*Y)+vw->V.z*J10(*Z) );
	J01( vf ) = vw->dis*(vw->V.x*J01(*X)+vw->V.y*J01(*Y)+vw->V.z*J01(*Z) );

	J20( vf ) = vw->dis*(vw->V.x*J20(*X)+vw->V.y*J20(*Y)+vw->V.z*J20(*Z) );
	J11( vf ) = vw->dis*(vw->V.x*J11(*X)+vw->V.y*J11(*Y)+vw->V.z*J11(*Z) );
	J02( vf ) = vw->dis*(vw->V.x*J02(*X)+vw->V.y*J02(*Y)+vw->V.z*J02(*Z) );

	J30( vf ) = vw->dis*(vw->V.x*J30(*X)+vw->V.y*J30(*Y)+vw->V.z*J30(*Z) );
	J21( vf ) = vw->dis*(vw->V.x*J21(*X)+vw->V.y*J21(*Y)+vw->V.z*J21(*Z) );
	J12( vf ) = vw->dis*(vw->V.x*J12(*X)+vw->V.y*J12(*Y)+vw->V.z*J12(*Z) );
	J03( vf ) = vw->dis*(vw->V.x*J03(*X)+vw->V.y*J03(*Y)+vw->V.z*J03(*Z) );


	product_jet23( &vf, &nf1, &Q->f2 );
}





void
view_per3_z_jet2( view_type *vw, jet23 *X, jet23 *Y, jet23 *Z, jet2 *z )
{
jmap23	F;
vec3d	p0;

	view_per3_jmap23( vw, X, Y, Z, &F );
	jacobian_jmap23( &F, z );
	return;

	p0.x = J00(*X) - vw->cp.x;
	p0.y = J00(*Y) - vw->cp.y;
	p0.z = J00(*Z) - vw->cp.z;


	J00( *z ) = p0.x *J10(*Z) + p0.y *J01(*Z) - p0.z;

	J10( *z ) = p0.x *2*J20(*Z) + J10(*Z) +
			p0.y *J11(*Z) +
			- J10(*Z);

	J01( *z ) = p0.x *J11(*Z) +
			p0.y *2*J02(*Z) + J01(*Z) +
			- J01(*Z);

	J20( *z ) =  p0.x *3*J30(*Z) + 2*J20(*Z) +
			p0.y*J21(*Z) +
			-J20(*Z);
	J11( *z ) = p0.x *2*J21(*Z) + J11(*Z) +
			p0.y*2*J12(*Z) + J11(*Z) +
			-J11(*Z);
	J02( *z ) = p0.x *J12(*Z) +
			p0.y*3*J03(*Z) + 2*J02(*Z) +
			-J02(*Z);
}
