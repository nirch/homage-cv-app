#ifndef		_LZW_CODE_DECODE_
#define		_LZW_CODE_DECODE_

#include "Ucst/CstType.h"
#include "Ubst/BstType.h"
#include <stdio.h>
#include <string.h>

#define XV_BITS 12
#define HSIZE  5003
#define MAXCODE(n_bits)     ( (1 << (n_bits)) - 1)

/********************** prototypes ************************/

bst_type * lzw_encoding(cst_type * cst, unsigned char c);
bst_type * lzw_decoding(cst_type * cst);
void compress1(int n, FILE * fp, cst_type * cst);

typedef struct globalDataLzw
{
	bst_type * bst;

	int g_init_bits;
	FILE *g_outfile;
	int maxbits;
	int maxmaxcode;
	long  htab [HSIZE];
	unsigned short codetab [HSIZE];
	int free_ent;
	int clear_flg;
	long in_count;            /* length of input */
	long out_count;           /* # of codes output (for debugging) */
	unsigned long cur_accum;
	int cur_bits;
	int n_bits;                    /* number of bits/code */
	int maxcode;                   /* maximum code, given n_bits */
	int ClearCode;
	int EOFCode;
	char accum[ 256 ];
	int a_count;
	long hsize;
} globalDataLzw;

#endif

