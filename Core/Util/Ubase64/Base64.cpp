
/*********************
 ***	Base64.c   ***
 *********************/
#include	"Uigp/igp.h"
#include	"Base64.h"


static const unsigned char alphabet_[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char pad_ = '=';

static const int max_columns_ = 72;




int
base64_decode( u_char *input, int inBytes, u_char **data, int *bytes )
{
int	i;
int	decoder_[256];
int	member_[256];




	for( i = 0 ; i < 256 ; i++ )
		member_[i] = 0;
	
	for( i = 0 ; i < sizeof (alphabet_) ; i++ ){
		decoder_[alphabet_[i]] = i;
		member_[alphabet_[i]] = 1;
	}



	*bytes = ((inBytes + 3) / 4) * 3 + 1 ;
	*data = (u_char *)malloc( *bytes + 1 );

	int char_count = 0;
	int bits = 0;

	u_char	*tp = *data;
	u_char	*sp = input;
	
	for( i = 0 ; i < inBytes; i++, sp++ ){

		if( *sp == pad_)	break;

		if( member_[ *sp ] == 0 )
			continue;

		bits += decoder_[ *sp ];
		char_count++;

		if( char_count < 4 ){
			bits <<= 6;
			continue;
		}


		*tp++ = bits >> 16;
        *tp++ = (bits >> 8) & 0xff;
        *tp++ = bits & 0xff;
		bits = 0;
		char_count = 0;
	}

	if( *sp == pad_ ){
		if( char_count == 2 ){
			*tp++ = bits >> 10;
			char_count = 0;
		}
		if( char_count == 3 ){
			*tp++ = bits >> 16;
			*tp++ = (bits >> 8) & 0xff;
			char_count = 0;
        }
	}

	*tp = 0;
	*bytes = tp - (*data );

	if( char_count > 0 )	return( 2 );

	return( 1 );
}






int
base64_encode( u_char *input, int inBytes, u_char **data, int *bytes )
{

	unsigned char* result = 0;

	size_t length = ((inBytes + 2) / 3) * 4;
	size_t num_lines = length / max_columns_ + 1;
	length += num_lines + 1;
	result = (unsigned char *) malloc(length);


	int char_count = 0;
	int bits = 0;
	size_t pos = 0;


	for (int i = 0; i < inBytes; ++i ){
		if (input[i] > 255) {
			delete[] result;
			return 0;
		}

		bits += input[i];
		char_count++;

		if (char_count == 3)
		{
			result[pos++] = alphabet_[bits >> 18];
			result[pos++] = alphabet_[(bits >> 12) & 0x3f];
			result[pos++] = alphabet_[(bits >> 6) & 0x3f];
			result[pos++] = alphabet_[bits & 0x3f];
			bits = 0;
			char_count = 0;
		}
		else
		{
			bits <<= 8;
		}
	}


	if (char_count != 0)
	{
		bits <<= (16 - (8 * char_count));
		result[pos++] = alphabet_[bits >> 18];
		result[pos++] = alphabet_[(bits >> 12) & 0x3f];
		if (char_count == 1)
		{
			result[pos++] = pad_;
			result[pos++] = pad_;
		}
		else
		{
			result[pos++] = alphabet_[(bits >> 6) & 0x3f];
			result[pos++] = pad_;
		}
	}

	result[pos] = 0;
	*bytes = pos;
	*data = result;
	return( 1 );
}



