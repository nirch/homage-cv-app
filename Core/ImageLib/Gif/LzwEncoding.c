/**************************
 ***	LzwEncoding.c   ***
 **************************/
#include "lzw.h"

/******************* prototypes **********************/
static void output(int code, globalDataLzw * g);
static void flush_char(globalDataLzw * g);
static void char_out(int c, globalDataLzw * g);
static void cl_hash(long k, globalDataLzw * g);
static void cl_block (globalDataLzw * g);

static void bst_flush_char(globalDataLzw * g);
static void bst_char_out(int c, globalDataLzw * g);
static void bst_cl_block (globalDataLzw * g);
static void bst_output(int code, globalDataLzw * g);
/******************************************************/

bst_type * lzw_encoding(cst_type * cst, unsigned char codeLength)
{


/*
flush_char	==>	bst_flush_char
char_out	==>	bst_char_out
output		==>	bst_output
*/
long fcode;
int i = 0;
int c;
int ent;
int disp;
int hsize_reg;
int hshift;
int len;
unsigned char * data;
globalDataLzw * g;
globalDataLzw global;
int init_bits;
//	int init_bits = 9;
	init_bits = codeLength + 1;
	len = cst->BYTE_NO;
	data = cst->data;
	g = &global;
  /*
   * Set up the globals:  g_init_bits - initial number of bits
   *                      g_outfile   - pointer to output file
   */
	g->g_init_bits = init_bits;

//	g->g_outfile   = outfile;
	g->n_bits = init_bits;
  /* initialize 'compress' globals */
	g->maxbits = XV_BITS;
	g->maxmaxcode = 1 << XV_BITS;
	memset(g->htab, 0, sizeof(g->htab));
	memset(g->codetab, 0, sizeof(g->codetab));
	g->hsize = HSIZE;
	g->free_ent = 0;
	g->clear_flg = 0;
	g->in_count = 1;
	g->out_count = 0;
	g->cur_accum = 0;
	g->cur_bits = 0;
	
	//g->bst = bst_alloc(len * 2); Grisha 12.02.02
	g->bst = bst_alloc(len * 2 + 8);

  /*
   * Set up the necessary values
   */
	g->out_count = 0;
	g->clear_flg = 0;
	g->in_count = 1;
	g->maxcode = MAXCODE(g->n_bits);

	g->ClearCode = (1 << (init_bits - 1));
	g->EOFCode = g->ClearCode + 1;
	g->free_ent = g->ClearCode + 2;

	g->a_count = 0;
	ent = *data++;
	len--;
	hshift = 0;
	for ( fcode = (long) g->hsize;  fcode < 65536L; fcode *= 2L )
		hshift++;
	hshift = 8 - hshift;                /* set hash code range bound */

	hsize_reg = g->hsize;
	cl_hash((long)hsize_reg, g);            /* clear hash table */

	bst_output(g->ClearCode, g);
    
	while (len) 
	{
		c = *data++;
		len--;
	    g->in_count++;

	    fcode = (long) ( ( (long) c << g->maxbits) + ent);
	    i = (((int) c << hshift) ^ ent);    /* xor hashing */

	    if (g->htab[i] == fcode ) 
		{
			ent = g->codetab[i];
			continue;
		}
	    else if (g->htab[i] < 0)      /* empty slot */
			goto nomatch;

		disp = hsize_reg - i;           /* secondary hash (after G. Knott) */
		if ( i == 0 )
			disp = 1;

probe:
		if ( (i -= disp) < 0 )
			i += hsize_reg;

		if (g->htab[i] == fcode ) 
		{
			ent = g->codetab[i];
			continue;
		}

		if (g->htab[i] >= 0 ) 
			goto probe;

nomatch:
		bst_output(ent, g);
		g->out_count++;
		ent = c;

		if ( g->free_ent < g->maxmaxcode ) 
		{
			g->codetab[i] = g->free_ent++; /* code -> hashtable */
			g->htab[i] = fcode;
		}
		else
			bst_cl_block(g);
	}

  /* Put out the final code */
	bst_output(ent, g);
	g->out_count++;
	bst_output(g->EOFCode, g);
//	bst_realloc(g->bst, g->bst->BYTE_NO);
	return g->bst;
}



void compress1(int init_bits, FILE * outfile, cst_type * cst)
{
	long fcode;
	int i = 0;
	int c;
	int ent;
	int disp;
	int hsize_reg;
	int hshift;
	int len;
	unsigned char * data;
	globalDataLzw * g;
	globalDataLzw global;

	len = cst->BYTE_NO;
	data = cst->data;
	g = &global;

  /*
   * Set up the globals:  g_init_bits - initial number of bits
   *                      g_outfile   - pointer to output file
   */
	g->g_init_bits = init_bits;

	g->g_outfile   = outfile;
	g->n_bits = init_bits;
  /* initialize 'compress' globals */
	g->maxbits = XV_BITS;
	g->maxmaxcode = 1 << XV_BITS;
	memset(g->htab, 0, sizeof(g->htab));
	memset(g->codetab, 0, sizeof(g->codetab));
	g->hsize = HSIZE;
	g->free_ent = 0;
	g->clear_flg = 0;
	g->in_count = 1;
	g->out_count = 0;
	g->cur_accum = 0;
	g->cur_bits = 0;


  /*
   * Set up the necessary values
   */
	g->out_count = 0;
	g->clear_flg = 0;
	g->in_count = 1;
	g->maxcode = MAXCODE(g->n_bits);

	g->ClearCode = (1 << (init_bits - 1));
	g->EOFCode = g->ClearCode + 1;
	g->free_ent = g->ClearCode + 2;

	g->a_count = 0;
	ent = *data++;
	len--;
	hshift = 0;
	for ( fcode = (long) g->hsize;  fcode < 65536L; fcode *= 2L )
		hshift++;
	hshift = 8 - hshift;                /* set hash code range bound */

	hsize_reg = g->hsize;
	cl_hash((long)hsize_reg, g);            /* clear hash table */

	output(g->ClearCode, g);
    
	while (len) 
	{
		c = *data++;
		len--;
	    g->in_count++;

	    fcode = (long) ( ( (long) c << g->maxbits) + ent);
	    i = (((int) c << hshift) ^ ent);    /* xor hashing */

	    if (g->htab[i] == fcode ) 
		{
			ent = g->codetab[i];
			continue;
		}
	    else if (g->htab[i] < 0)      /* empty slot */
			goto nomatch;

		disp = hsize_reg - i;           /* secondary hash (after G. Knott) */
		if ( i == 0 )
			disp = 1;

probe:
		if ( (i -= disp) < 0 )
			i += hsize_reg;

		if (g->htab[i] == fcode ) 
		{
			ent = g->codetab[i];
			continue;
		}

		if (g->htab[i] >= 0 ) 
			goto probe;

nomatch:
		output(ent, g);
		g->out_count++;
		ent = c;

		if ( g->free_ent < g->maxmaxcode ) 
		{
			g->codetab[i] = g->free_ent++; /* code -> hashtable */
			g->htab[i] = fcode;
		}
		else
			cl_block(g);
	}

  /* Put out the final code */
	output(ent, g);
	g->out_count++;
	output(g->EOFCode, g);
}

static void cl_hash(long hsize, globalDataLzw * g)          /* reset code table */
{
	long * htab_p = g->htab + hsize;
	long i;
	long m1 = -1;

	i = hsize - 16;
	do 
	{                            /* might use Sys V memset(3) here */
	    *(htab_p-16) = m1;
		*(htab_p-15) = m1;
		*(htab_p-14) = m1;
		*(htab_p-13) = m1;
		*(htab_p-12) = m1;
		*(htab_p-11) = m1;
		*(htab_p-10) = m1;
		*(htab_p-9) = m1;
		*(htab_p-8) = m1;
		*(htab_p-7) = m1;
		*(htab_p-6) = m1;
		*(htab_p-5) = m1;
		*(htab_p-4) = m1;
		*(htab_p-3) = m1;
		*(htab_p-2) = m1;
		*(htab_p-1) = m1;
//		memset(htab_p - 16, -1, 64);
		htab_p -= 16;
	} while ((i -= 16) >= 0);

	for ( i += 16; i > 0; i-- )
		*--htab_p = m1;
}



static void output(int code, globalDataLzw * g)
{
	unsigned long masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
                                  0x001F, 0x003F, 0x007F, 0x00FF,
                                  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
                                  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };
	g->cur_accum &= masks[g->cur_bits];

	if (g->cur_bits > 0)
		g->cur_accum |= ((long)code << g->cur_bits);
	else
		g->cur_accum = code;
	
	g->cur_bits += g->n_bits;

	while( g->cur_bits >= 8 ) 
	{
		char_out( (int)(g->cur_accum & 0xff), g );
		g->cur_accum >>= 8;
		g->cur_bits -= 8;
	}

  /*
   * If the next entry is going to be too big for the code size,
   * then increase it, if possible.
   */

	if (g->free_ent > g->maxcode || g->clear_flg) 
	{
		if ( g->clear_flg ) 
		{
			g->maxcode = MAXCODE (g->n_bits = g->g_init_bits);
			g->clear_flg = 0;
		}
		else 
		{
			g->n_bits++;
			if ( g->n_bits == g->maxbits )
				g->maxcode = g->maxmaxcode;
			else
				g->maxcode = MAXCODE(g->n_bits);
		}
	}
	
	if ( code == g->EOFCode ) 
	{
    /* At EOF, write the rest of the buffer */
		while( g->cur_bits > 0 ) 
		{
			char_out( (int)(g->cur_accum & 0xff), g);
			g->cur_accum >>= 8;
			g->cur_bits -= 8;
		}
		flush_char(g);
		fflush( g->g_outfile );
	}
}

/********************************/
static void cl_block (globalDataLzw * g)             /* table clear for block compress */
{
  /* Clear out the hash table */

	cl_hash (g->hsize, g);
	g->free_ent = g->ClearCode + 2;
	g->clear_flg = 1;
	output(g->ClearCode, g);
}

static void char_out(int c, globalDataLzw * g)
{
	g->accum[ g->a_count++ ] = c;
	if ( g->a_count >= 254 ) 
		flush_char(g);
}

/*
 * Flush the packet to disk, and reset the accumulator
 */
static void flush_char(globalDataLzw * g)
{
	if( g->a_count > 0 ) 
	{
		fputc(g->a_count, g->g_outfile );
		fwrite(g->accum, 1, g->a_count, g->g_outfile);
		g->a_count = 0;
	}
}

/****************************** bst ********************************/

static void bst_output(int code, globalDataLzw * g)
{
/*
flush_char	==>	bst_flush_char
char_out	==>	bst_char_out
*/
	unsigned long masks[] = { 0x0000, 0x0001, 0x0003, 0x0007, 0x000F,
                                  0x001F, 0x003F, 0x007F, 0x00FF,
                                  0x01FF, 0x03FF, 0x07FF, 0x0FFF,
                                  0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF };
	g->cur_accum &= masks[g->cur_bits];

	if (g->cur_bits > 0)
		g->cur_accum |= ((long)code << g->cur_bits);
	else
		g->cur_accum = code;
	
	g->cur_bits += g->n_bits;

	while( g->cur_bits >= 8 ) 
	{
		bst_char_out( (int)(g->cur_accum & 0xff), g );
		g->cur_accum >>= 8;
		g->cur_bits -= 8;
	}

  /*
   * If the next entry is going to be too big for the code size,
   * then increase it, if possible.
   */

	if (g->free_ent > g->maxcode || g->clear_flg) 
	{
		if ( g->clear_flg ) 
		{
			g->maxcode = MAXCODE (g->n_bits = g->g_init_bits);
			g->clear_flg = 0;
		}
		else 
		{
			g->n_bits++;
			if ( g->n_bits == g->maxbits )
				g->maxcode = g->maxmaxcode;
			else
				g->maxcode = MAXCODE(g->n_bits);
		}
	}
	
	if ( code == g->EOFCode ) 
	{
    /* At EOF, write the rest of the buffer */
		while( g->cur_bits > 0 ) 
		{
			bst_char_out( (int)(g->cur_accum & 0xff), g);
			g->cur_accum >>= 8;
			g->cur_bits -= 8;
		}
		bst_flush_char(g);
//		fflush( g->g_outfile );
	}
}

/********************************/
static void bst_cl_block (globalDataLzw * g)             /* table clear for block compress */
{
  /* Clear out the hash table */

	cl_hash (g->hsize, g);
	g->free_ent = g->ClearCode + 2;
	g->clear_flg = 1;
	bst_output(g->ClearCode, g);
}

static void bst_char_out(int c, globalDataLzw * g)
{
	g->accum[ g->a_count++ ] = c;
	if ( g->a_count >= 254 ) 
		bst_flush_char(g);
}

/*
 * Flush the packet to disk, and reset the accumulator
 */
static void bst_flush_char(globalDataLzw * g)
{
	if( g->a_count > 0 ) 
	{
		bst_put_align_char(g->bst, g->a_count);
//		fputc(g->a_count, g->g_outfile );
		bst_put_buff(g->bst, g->accum, g->a_count);
//		fwrite(g->accum, 1, g->a_count, g->g_outfile);
		g->a_count = 0;
	}
}

