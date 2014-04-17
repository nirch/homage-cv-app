#include	<string.h>
#include	<stdio.h>
#include	<math.h>
#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "clapackD.lib" )
#else
#pragma comment( lib, "clapack.lib" )
#endif



extern "C" {
#include	"Uigp/igp.h"
#include	"umath.h"
#include	"Matrix.h"
}
#ifdef WIN32
#pragma warning(disable :4005)
#endif

// matrix_type * euclid(matrix_type * x, matrix_type * y, matrix_type * res)
// Compute L2 distance between rows of x to rows of y. If res is given,
// override its values and return it. Otherwise (res is NULL) allocate result matrix
matrix_type * euclid(matrix_type * x, matrix_type * y, matrix_type * res) {
	if (res==NULL)
		res = matrix_alloc(x->m,y->m,MT_DOUBLE);
	double sum = 0;
	double diff = 0;
	for (int i=0 ; i < x->m ; i++) {
		for (int j=0; j < y->m; j++) { 
			sum = 0;	
			for (int k=0; k < x->n ; k++) {
				diff = MATRIX_ELM(x,i,k)-MATRIX_ELM(y,j,k);
				sum += diff*diff;
			}
			MATRIX_ELM(res,i,j) = sqrt(sum);
		}
	}
	return res;
}

// randn - generate a double random number from Normal(0,1)
double randn() {
/* Two Gaussian random numbers generated from two uniform random numbers. Copyright (c) Tao Pang 1997. */
 	double pi,r1,r2;
	pi = 4*atan(1.0);
	r1 = -log(1-(double)rand()/RAND_MAX);
	r2 = 2*pi*((double)rand()/RAND_MAX);
	r1 = sqrt(2*r1);
	return r1*cos(r2);
	//*y = r1*sin(r2);
} 

// Generate a m x n matrix with standard normal distribution
matrix_type * matrix_randn(int m,int n) {
	matrix_type * res = matrix_alloc(m,n,MT_DOUBLE);
	for (int i=0; i < m; i++)
		for (int j=0; j < n ; j++)
			MATRIX_ELM(res,i,j) = randn();
	return res;
}

// Compute sum of matrix
double matrix_sum ( matrix_type *D) {
	// return sum of all matrix elements
	double sum = 0;
	for (int i=0 ; i< D->n ; i++)
		for (int j = 0 ; j < D->m ; j++)
			sum += MATRIX_ELM(D,i,j);
	return sum;
}


int matrix_sammons( matrix_type *D, int dim, int NumIter, int MaxHalves, double TolFun,matrix_type **P )
//	matrix_sammons - Compute Sammons mapping MDS embedding for points.
//  Inputs:
//			D - distance matrix between all pairs of points (NxN)
//			dim - required dimensionality
//			NumIter - maximal number of algorithm iterations
//			MaxHalves - maximal number of halving steps for line search
//			TolFun - relative tolerance termination criterion
//	Outputs:
//			P - Embeding matrix N x dim
//
// References  : [1] Sammon, John W. Jr., "A Nonlinear Mapping for Data
//                   Structure Analysis", IEEE Transactions on Computers,
//                   vol. C-18, no. 5, pp 401-409, May 1969.
{
//////////////////////	
// Initialization
//////////////////////
	int i,j;
   int		N = D->m;
   double    D_sum = matrix_sum(D);
   double	scale = 0.5 / D_sum;
   matrix_type * D_temp = NULL;
   D_temp = matrix_copy(D,D_temp);
   // Good Default parameters
   // NumIter = 500;
   // MaxHalves = 20;
   // TolFun = 1e-9;

   int i_step;
   
   // D     = D + eye(N);
   for ( i = 0 ; i < D_temp->n ; i++)
	   MATRIX_ELM(D_temp,i,i) += 1;

   // Dinv  = 1 ./ D;
   matrix_type * Dinv = matrix_alloc(N,N,MT_DOUBLE);
   for (i = 0 ; i < N; i++)
	   for (j= 0 ; j < N ; j++)
		   MATRIX_ELM(Dinv,i,j) = 1/MATRIX_ELM(D_temp,i,j);
 
   // y = randn(N, dim);
   matrix_type * y = matrix_randn(N,dim);

   // d     = euclid(y,y) + eye(N);
   matrix_type * d = euclid(y,y,NULL);
   for (i = 0 ; i < N ; i++)
	   MATRIX_ELM(d,i,i) += 1;

   // dinv  = 1./d;
   matrix_type * dinv = matrix_alloc(N,N,MT_DOUBLE);
    for (i = 0 ; i < N; i++)
	   for (j= 0 ; j < N ; j++)
		   MATRIX_ELM(dinv,i,j) = 1/MATRIX_ELM(d,i,j);
   //matrix_inverse(d,dinv);
    
   //delta = D - d;
   matrix_type * delta = matrix_alloc(N,N,MT_DOUBLE);
   for (i = 0 ; i < N; i++)
	   for (j= 0 ; j < N ; j++)
		   MATRIX_ELM(delta,i,j) = MATRIX_ELM(D_temp,i,j)-MATRIX_ELM(d,i,j);

   //E     = sum(sum((delta.^2).*Dinv));
   double E = 0;
   for (i = 0 ; i < N; i++)
	   for (j= 0 ; j < N ; j++)
		   E += MATRIX_ELM(delta,i,j)*MATRIX_ELM(delta,i,j)*MATRIX_ELM(Dinv,i,j);

   /////////////////////////
   // Iterations
   ////////////////////////
   matrix_type * delta_sumrow = matrix_alloc(N,1,MT_DOUBLE);
   matrix_type * g = matrix_alloc(N,dim,MT_DOUBLE);
   matrix_type * dinv3 = matrix_alloc(N,N,MT_DOUBLE);
   matrix_type * dinv3_sumrow = matrix_alloc(N,1,MT_DOUBLE);
   matrix_type * y2 = matrix_alloc(N,dim,MT_DOUBLE);
   matrix_type * tmp1 = matrix_alloc(N,dim,MT_DOUBLE);
   matrix_type * tmp2 = matrix_alloc(N,dim,MT_DOUBLE);
   matrix_type * H = matrix_alloc(N,dim,MT_DOUBLE);
   matrix_type * s = matrix_alloc(N,dim,MT_DOUBLE);
   matrix_type * y_old = matrix_alloc(N,dim,MT_DOUBLE);
   double sum, elm, E_new;


   for (int i_iter = 0 ; i_iter < NumIter ; i_iter++) {

		// Compute gradient, Hessian and search direction (note it is actually
        // 1/4 of the gradient and Hessian, but the step size is just the ratio
        // of the gradient and the diagonal of the Hessian so it doesn't
        // matter).

		// delta    = dinv - Dinv;
		for (i = 0 ; i < N; i++)
			for (j= 0 ; j < N ; j++)
				MATRIX_ELM(delta,i,j) = MATRIX_ELM(dinv,i,j)-MATRIX_ELM(Dinv,i,j);

		// deltaone = delta * one;
		for (i = 0 ; i < N; i++) {
			sum =0;
			for (j = 0 ; j < N ; j++)
				sum += MATRIX_ELM(delta,i,j);
			MATRIX_ELM(delta_sumrow,i,0) = sum;
		}

		// g        = delta * y - y .* deltaone;
		matrix_multiply(delta,y,g);
		for (i = 0 ; i < N ; i++)
			for (j= 0 ; j < dim ; j++)
				MATRIX_ELM(g,i,j) -= MATRIX_ELM(y,i,j)*MATRIX_ELM(delta_sumrow,i,0);

		// dinv3    = dinv .^ 3;
		for (i = 0 ; i < N; i++)
			for (j= 0 ; j < N ; j++) {
				elm = MATRIX_ELM(dinv,i,j);
				MATRIX_ELM(dinv3,i,j) = elm*elm*elm;
			}

		// y2       = y .^ 2;
		for (i = 0 ; i < N ; i++)
			for (j= 0 ; j < dim ; j++) {
				elm = MATRIX_ELM(y,i,j);
				MATRIX_ELM(y2,i,j) = elm*elm;
			}

		// Compute H				
        // H  = dinv3 * y2 - deltaone - 2 * y .* (dinv3 * y) + y2 .* (dinv3 * one);
		matrix_multiply(dinv3,y2,tmp1);
		matrix_multiply(dinv3,y,tmp2);

		for (i = 0 ; i < N; i++) {
			sum =0;
			for (j = 0 ; j < N ; j++)
				sum += MATRIX_ELM(dinv3,i,j);
			MATRIX_ELM(dinv3_sumrow,i,0) = sum;
		}

		for (i=0 ; i < N ; i++)
			for (j=0; j < dim ; j++)
				MATRIX_ELM(H,i,j) = MATRIX_ELM(tmp1,i,j)-MATRIX_ELM(delta_sumrow,i,0) -2*MATRIX_ELM(y,i,j)*MATRIX_ELM(tmp2,i,j) + MATRIX_ELM(y2,i,j) * MATRIX_ELM(dinv3_sumrow,i,0);

		// s        = -g(:) ./ abs(H(:));
		for (i=0 ; i < N ; i++)
			for (j=0; j < dim ; j++)
				MATRIX_ELM(s,i,j) = -MATRIX_ELM(g,i,j) / abs(MATRIX_ELM(H,i,j));

        // y_old    = y;
		matrix_copy(y,y_old);

		// Use step-halving procedure to ensure progress is made
		for (i_step = 0 ; i_step < MaxHalves ; i_step++) {
			// y(:) = y_old(:) + s;
			for (i=0 ; i < N ; i++)
				for (j=0; j < dim ; j++)
					MATRIX_ELM(y,i,j) = MATRIX_ELM(y_old,i,j) + MATRIX_ELM(s,i,j);
			// d     = euclid(y, y) + eye(N);
			d = euclid(y,y,d);
			for (i=0 ; i < N ; i++)
				MATRIX_ELM(d,i,i) += 1;
			// dinv  = 1 ./ d;
			for (i = 0 ; i < N; i++)
				for (j= 0 ; j < N ; j++)
					MATRIX_ELM(dinv,i,j) = 1/MATRIX_ELM(d,i,j);
			//matrix_inverse(d,dinv);
			// delta = D - d;
			for (i = 0 ; i < N; i++)
				for (j= 0 ; j < N ; j++)
					MATRIX_ELM(delta,i,j) = MATRIX_ELM(D_temp,i,j)-MATRIX_ELM(d,i,j);

			// E_new = sum(sum((delta .^ 2) .* Dinv));
			E_new = 0;
			for (i = 0 ; i < N; i++)
				for (j= 0 ; j < N ; j++)
					E_new += MATRIX_ELM(delta,i,j)*MATRIX_ELM(delta,i,j)*MATRIX_ELM(Dinv,i,j);
            
            if (E_new < E)
                break;
			else {
                // s = 0.5*s;
				for (i=0 ; i < N ; i++)
					for (j=0; j < dim ; j++)
						MATRIX_ELM(s,i,j) /= 2;
			}
		}

		// Bomb out if too many halving steps are required
        if (i_step == MaxHalves)
            printf("MaxHalves exceeded. Sammon mapping may not converge...\n");
        
        // Evaluate termination criterion
        if (abs((E - E_new) / E) < TolFun)
			break;

		E = E_new;
        printf("E = %f\n",E*scale);
	}
	// Fiddle stress to match the original Sammon paper
    E = E * scale;
	matrix_copy(y,*P);

	// Destroy matrices
	matrix_destroy(D_temp);
	matrix_destroy(Dinv);
	matrix_destroy(d);
	matrix_destroy(dinv);
	matrix_destroy(delta);
	matrix_destroy(delta_sumrow);
	matrix_destroy(g);
	matrix_destroy(dinv3);
	matrix_destroy(dinv3_sumrow);
	matrix_destroy(y2);
	matrix_destroy(tmp1);
	matrix_destroy(H);
	matrix_destroy(s);
	matrix_destroy(y_old);
	matrix_destroy(y);
	
	return 0;
}


