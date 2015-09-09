
/************************
 ***   GpFunction.c   ***
 ************************/
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include	"igp.h"


int
gp_ilog2( int a )
{
int     i;
    if( a < 0 )     a = -a;


	for( i = 0 ; a != 0 ; i++, a >>= 1 );

	return( i );
}


int
gp_bound2( int a )
{
int	i,	b;

	if( a < 0 )     a = -a;
 
	for( i = 0, b = 1; a >= b ; i++, b <<= 1 );

	return( b );
}


float
gpNormalDistribution( float average, float var, float x )
{
	float	t,	p;

	t = - (x - average)*( x-average) /( 2*var );

	p = exp( t  ) / sqrt( 2*M_PI *var);

	return( p );
}


/*********  gp_strsep  **********
* @Description	:	
* 
* @Param  char **sptr :  
* @Param  const char *delim :  
* @Return  char *  :  
*****************************************/
char *gp_strsep(char **sptr, const char *delim)
{
	char *start, *ret;
	start = ret = *sptr;
	if ((ret == NULL) || ret == '\0') {
	   return (NULL);
	}

	while (*ret != '\0' &&
		   strchr(delim, *ret) == NULL) {
		ret++;
	}
	if (*ret == '\0') {
		*sptr = NULL;
	} else {
	    *ret = '\0';
	    ret++;
	    *sptr = ret;
	}
	return (start);
}

/*********  *gp_strcasestr  **********
* @Description	:	finds a string in another, NOT case sensitice
* 
* @Param  const char *haystack :string to look at  
* @Param  const char *needle :  string to look for
* @Return  char*  :  pointer in haystack of needle
*****************************************/
char *gp_strcasestr(const char *haystack, const char *needle)
{
  int nlen, hlen;
  int ix;

  if (needle == NULL || haystack == NULL) return (NULL);
  nlen = strlen(needle);
  hlen = strlen(haystack);
 
  for (ix = 0; ix + nlen <= hlen; ix++) {
    if (gp_strnicmp(needle, haystack + ix, nlen) == 0) {
      return ((char *)haystack + ix);
    }
  }
  return (NULL);
}


char *gp_strlwr(char *szString)
{
	char *szResult = szString;
	if (szString)
	{
		char *szPtr = szString;
		while(*szPtr != '\0')
		{
			if (*szPtr >= 'A' && *szPtr <= 'Z')
			{
				*szPtr = (*szPtr) + ('a' - 'A');
			}
			szPtr++;
		}
	}
	return szResult;
} // gp_strlwr


/*
int gp_strnicmp(const char *szString1, const char *szString2, size_t count)
{
	int nResult = -1;
	if (szString1 && szString2 && count > 0)
	{
		char *szStr1 = malloc(count+1);
		char *szStr2 = malloc(count+1);
		strncpy(szStr1, szString1, count);
		strncpy(szStr2, szString2, count);
		nResult = strncmp(gp_strlwr(szStr1), gp_strlwr(szStr2), count);
		free (szStr1);
		free (szStr2);
	}
	return nResult;
} // gp_strnicmp
*/


char *
gp_strichr( char *s, int c )
{
char	*p;
int	c1;

	if( c >= 'A' )
		c1 = c- 'A' + 'a';
	else	c1 = c + 'A' - 'a';

	for( p = s ; *p != '\0' ; p++ ){
		if( *p == c || *p == c1 )
			return( p );
	}
	return( NULL );
}

int 
gp_strnicmp(const char *s1, const char *s2, int n)
{
char	*p1,	*p2,	a1,	a2;
int	i;

	for( i = 0, p1 = (char *)s1, p2 = (char *)s2 ; i < n ; i++, p1++, p2++ ){
		if( *p1 == *p2 )	continue;

		if( *p1 >= 'A' && *p1 <= 'Z' )
			a1 = (*p1) + ('a' - 'A');
		else	a1 = *p1;

		if( *p2 >= 'A' && *p2 <= 'Z' )
			a2 = (*p2) + ('a' - 'A');
		else	a2 = *p2;

		if( a1 != a2 )	return( 1 );
	}

	return 0;
} // gp_strnicmp


int 
gp_stricmp(const char *s1, const char *s2)
{
char	*p1,	*p2,	a1,	a2;
int	i;

	for( i = 0, p1 = (char *)s1, p2 = (char *)s2 ; *p1 != 0 && &p2 != 0 ; i++, p1++, p2++ ){
		if( *p1 == *p2 )	continue;

		if( *p1 >= 'A' && *p1 <= 'Z' )
			a1 = (*p1) + ('a' - 'A');
		else	a1 = *p1;

		if( *p2 >= 'A' && *p2 <= 'Z' )
			a2 = (*p2) + ('a' - 'A');
		else	a2 = *p2;

		if( a1 != a2 )	return( 1 );
	}

	if( *p1 != *p2 )	return( 1 );

	return 0;
} // gp_strnicmp


int 
gpStr_remove_end_apace( char *text )
{
char	*p,	*tp;


	for( p = tp = text ; *p != 0 ; p++ )
		if( *p != ' ' && *p != '\t' )
			tp = p+1;

	*tp = 0;

	return( tp - text );
}

int 
gpStr_remove_head_apace( char *text )
{
char	*p,	*tp;


	for( p = text ; *p != 0 ; p++ )
		if( *p != ' ' && *p != '\t' )
			break;

//	if( p == text )	return( 1 );


	for( tp = text ; *p != 0 ; p++, tp++ )
			*tp = *p;

	*tp = 0;

	return( tp - text );
}


u_short *
gpStr_char2short( char *text )
{
u_short	*wtext;
int	len,	i;

	len = strlen( text );
	if( len == 0 )	return( NULL );

	wtext = (u_short *)malloc( 2*(len + 1) );

	for( i = 0 ; i < len ; i++ )
		wtext[i] = text[i];
	wtext[i] = 0;

	return( wtext );
}




void
gpStr_sort( char *str[], int no  )
{
int	i,	j;
char	*s;

	for( i = 1 ; i < no-1 ; i++ )
		for( j = 0 ; j < no-i ; j++ )
			if( strcmp( str[j], str[j+1] ) > 0 ){
				s = str[j];
				str[j] = str[j+1];
				str[j+1] = s;
			}
}



int
gp_strtok_float( char *data, char *seps, float *a)
{
char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%f", a );
		return( 1 );
	}
	else	return( -1 );
}


int
gp_strtok_string( char *data, char *seps, char *a)
{
	char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%s", a );
		return( 1 );
	}
	else	return( -1 );
}

int
gp_strtok_int( char *data, char *seps, int *a)
{
char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%d", a );
		return( 1 );
	}
	else	return( -1 );
}

int
gp_strtok_hexa( char *data, char *seps, int *a)
{
	char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%x", a );
		return( 1 );
	}
	else	return( -1 );
}
int	
gpStr_replace( char *in, char *s0, char *s1, char *out )
{
int i,	n0;
char	*p;

	n0 = strlen( s0 );

	for( p = in, i = 0 ; *p != 0 ; p++, i++ )
		if( strncmp( p, s0, n0 ) == 0 )
			break;

	if( *p == 0 )	return( -1 );

	strncpy( out, in, i );
	out[i] = 0;
	strcat( out, s1 );

	strcat( out, p+n0 );

	return( 1 );
}



int
gpStr_strtok_float( char *data, char *seps, float *a )
{
	char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%f", a );
		return( 1 );
	}
	else	return( -1 );
}


int
gpStr_strtok_double( char *data, char *seps, double *a )
{
	char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%lf", a );
		return( 1 );
	}
	else	return( -1 );
}


int
gpStr_strtok_int( char *data, char *seps, int *a )
{
	char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%d", a );
		return( 1 );
	}
	else	return( -1 );
}


int
gpStr_strtok_hexa( char *data, char *seps, int *a)
{
	char *token;

	if(( token = strtok( data, seps )) != NULL ){
		sscanf( token, "%x", a );
		return( 1 );
	}
	else	return( -1 );
}

int
gpStr_strtok_string( char *data, char *seps, char *a )
{
	char *token;

	if(( token = strtok( data, seps )) != NULL ){
		strcpy( a, token );
		return( 1 );
	}
	else	return( -1 );
}

int
gpStr_strtok_skip( char *data, char *seps, int skip )
{
	char *token;
	int	i;


	for( i = 0 ; i < skip ; i++  ){
		if(( token = strtok( data, seps )) == NULL )
			return( -1 );
		data = NULL;
	}
	
	return( 1 );
}


int
gpStr_strtok_istring( char *data, char *seps, int iToken,  char *token )
{
int	ret;

	if( iToken == 0 ){
		ret = gpStr_strtok_string( data, seps, token );
		return( ret );
	}

	if( gpStr_strtok_skip( data, seps, iToken ) < 0 )
		return( -1 );

	ret = gpStr_strtok_string( NULL, seps, token );

	return( ret );
}


int
gpStr_strtok_ifloat( char *data, char *seps, int iToken, float *token )
{
	int	ret;

	if( iToken == 0 ){
		ret = gpStr_strtok_float( data, seps, token );
		return( ret );
	}

	if( gpStr_strtok_skip( data, seps, iToken ) < 0 )
		return( -1 );

	ret = gpStr_strtok_float( NULL, seps, token );

	return( 1 );
}



char *
 gpStr_strdup( char *p )
{
char	*s;
int	len;

	len = strlen( p );
	s = malloc( len +1 );
	strcpy( s, p );

	return( s );
}

int	
gpStr_indexOf( char *str, char a )
{
	char	*p;
	int	i;


	for( p = str, i = 0 ; *p != 0 ; p++, i++ ){
		if( *p  == a )
			return( i );
	}

	return( i );
}


int	
gpStr_lastIndexOf( char *str, char a )
{
	char	*p;
	int	i,	i0;

	i0 = -1;
	for( p = str, i = 0 ; *p != 0 ; p++, i++ ){
		if( *p  == a )
			i0 = i;
//			return( i );
	}

	return( i0 );
}





char *
gpStr_union( char *s[], int nS, char **data, int *bytes )
{
char	*sp,	*tp;
int	n,	i;

	for( i = 0, n = 0 ; i < nS && s[i] != NULL ; i++ ){
		n += strlen(s[i] );
	}


	*data = malloc( n + nS + 10 );

	tp = *data;
	for( i = 0 ; i < nS && s[i] != NULL ; i++ ){
		
		for( sp = s[i] ; *sp != 0 ; sp++ )
			*tp++ = *sp;

	}

	*tp = 0;

	*bytes = tp - *data;

	return( *data );
}


char *
gpStr_token( char *data, char *end, char *token, int token_len )
{
	char	*p;

	if( token_len < 0 )
		token_len = strlen( token );

	for( p = data ; p < end - token_len+1 ; p++ )
		if( strncmp( p, token, token_len) == 0 )
			return( p );

	return( NULL );
}

char *
gpStr_token_skip( char *data, char *end, char *token, int token_len )
{
	char	*p;

	if( token_len < 0 )
		token_len = strlen( token );

	for( p = data ; p < end - token_len+1 ; p++ )
		if( strncmp( p, token, token_len) == 0 )
			return( p + token_len );

	return( NULL );
}



void
gpStrA_replace_c( char *as[], int nS, char c1, char c2 )
{
	int	i;
	for( i = 0 ; i < nS ; i++ )
		gpStr_replace_c(as[i],  c1, c2 );
}

void
gpStr_replace_c( char *s, char c1, char c2 )
{
	char	*p;

	for( p = s ; *p != 0 ; p++ )
		if( *p == c1 )	*p = c2;
}



float
gpSeg_overlap( float b0, float e0, float b1, float e1 )
{
	if( b0 < b1 ){
		if( e0 < b1 )	return( 0 );
		if( e0 < e1 )
			return( e0 - b1 );
		else	return( e1 - b1 );
	}


	if( e1 < b0 )	return( 0 );
	if( e1 < e0 )
		return( e1 - b0 );
	else	return( e0 - b0 );
}