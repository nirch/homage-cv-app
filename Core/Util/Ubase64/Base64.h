

#ifndef _BASE64_H_
#define _BASE64_H_


int	base64_decode( u_char *input, int inBytes, u_char **data, int *bytes );

int	base64_encode( u_char *input, int inBytes, u_char **data, int *bytes );


#endif