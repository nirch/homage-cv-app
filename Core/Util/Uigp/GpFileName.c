//#include	<stdio.h>

#include	<string.h>


#include	"igp.h"






int
gp_filename_extand( char *file1, char *file2 )
{
int	len1,	len2,	k,	k2;


	len1 = strlen( file1 );
	len2 = strlen( file2 );

	if( len2 == 1 ){
		if( strncmp( file2, ".", 1 ) )	return( 0 );
		k2 = 0;
	}
	else	{
		if( strncmp( &file2[len2-2], "/.", 2 ) )	return( 0 );
		k2 = len2-1;
	}

	for( k = len1-1 ; k >= 0 && file1[k] != '/' ; k-- );

	strcpy( &file2[k2], &file1[k+1] );

	return( 1 );
}


int
gp_string_cut_extension( char *s1, char *extansion, char *s2 )
{
int	len,	n;
	len = strlen( s1 );
	n = strlen( extansion );

	strcpy( s2, s1 );

	if( strcmp( &s1[len-n], extansion ) != 0 )
		return( 0 );

	s2[len-n] = '\0';
	return( 1 );
}





/*********  gp_filename_split  **********
* @Description	:  splits a filename (dir/subdir/file.name.ext) 
*					into it's directory (dir/subdir) it's base name (file.name)
*					and it's extension (.ext)
* 
* @Param   char *fname :  path
* @Param  char *dir :  buffer for the directory
* @Param  char *name :  buffer for the base name
* @Param  char *extension :  buffer for the extension
* @Return  void  :  
*****************************************/
void gp_filename_split( char *fname, char *dir, char *name, char *extension )
{
	if( dir != NULL )
		gp_filename_extract_dir(fname, dir, 256 );

	if( name != NULL )
		gp_filename_extract_name(fname, name, 256 );

	if( extension != NULL )
		gp_filename_extract_extension(fname, extension, 256 );	

}




int 
gpFilename_dir(char *fname, char *dir )
{
char *sp,	*tp,	*p;
int	i;

	p = NULL;
	for( sp = fname, i = 0 ; *sp != 0 ; sp++, i++ ){
		if( *sp == '/' || *sp == '\\' )
			p = sp;
	}

	if( p == NULL ){
		dir[0] = '.';
		dir[1] = '\0';
		return( 1 );
	}


	if(	i > 256 )
		return( -1 );



	for( sp = fname, tp = dir ; sp != p ; sp++, tp++ )
		*tp = *sp;

	*tp = '\0';

	return( 1 );
}




/*********  gp_filename_extract_dir  **********
* @Description	:  gets the directory of the filename
* 
* @Param  char *fname :  path
* @Param  char *dir :  buffer for the directory
* @Param  int len :  length of that buffer
* @Return  void  :  
*****************************************/
void gp_filename_extract_dir(char *fname, char *dir, int len)
{
char *slash, *backslash, *end;
int	dirlength;


	if ( len == 1)
	{
		dir = '\0';
		return;
	}

	// get dir delimiter
	slash = strrchr(fname, (int)'/');
	backslash = strrchr(fname, (int)'\\');

	// get dir delimiter
	end = NULL;
	if ( slash == NULL )
	{
		if ( backslash == NULL)
		{
			end = NULL;
		}		
		else
			end = backslash + 1;
	}
	else if ( backslash == NULL)
	{
		end = slash + 1;
	}
	else
	{
		if ( (int)(slash - backslash) > 0)
			end = slash + 1;
		else
			end = backslash + 1;
	}


	if ( end != NULL)
	{
		dirlength = (int)(end - fname);
		if ( len > 1 && dirlength > len)
			dirlength = len;
		dirlength--;
		strncpy(dir, fname, dirlength);
		dir[dirlength] = '\0';
	}
	else
	{
		strcpy(dir, ".");
	}
	
	
}

void gpFilename_name( char *fname, char *name )
{
	gp_filename_extract_name( fname,name, 256 );

}

/*********  gp_filename_extract_name  **********
* @Description	:  gets the filename (without extension) out of the name
* 
* @Param   char *fname :a name to analyze  
* @Param  char *name :  result filename
* @Param  int len :  length of the name buffer
* @Return  void  :  
*****************************************/

void gp_filename_extract_name( char *fname, char *name, int len )
{
int	fnameLength,  nameLength;
char *lastpoint;
char *slash, *backslash, *start = NULL;
	
	if ( len == 1 )
	{
		name[0] = '\0';
		return;
	}
	fnameLength = strlen( fname );	

	// get dir delimiter
	slash = strrchr(fname, (int)'/');
	backslash = strrchr(fname, (int)'\\');
	// get extension delimiter
	lastpoint = strrchr(fname,(int)'.');
	//firstpoint = strchr(fname,(int)'.');
	


	// get dir delimiter
	if ( slash == NULL )
	{
		if ( backslash == NULL)
			start = fname ;
		else
			start = backslash + 1;
	}
	else if ( backslash == NULL)
	{
		start = slash + 1;
	}
	else
	{
		if ( (int)(slash - backslash) > 0)
			start = slash + 1;
		else
			start = backslash + 1;
	}

	if ( lastpoint == NULL || lastpoint < start)
	{
		nameLength = fnameLength - (int)(start - fname);
	}
	else
	{		
		nameLength = (int)(lastpoint - start);
	}


	if ( len > 1 && nameLength > len - 1)
		nameLength = len - 1;

	if( nameLength > 0)
	{
		strncpy(name,start, nameLength);
		name[nameLength] = '\0';
	}
	else
		name[0] = '\0';

}


void gpFilename_fullname( char *fname, char *name )
{
	gp_filename_extract_FullName( fname,name, 256 );

}


void gp_filename_extract_FullName( char *fname, char *name, int len )
{
int	fnameLength;
char	*start,	*p;

	name[0] = 0;
	fnameLength = strlen( fname );
	if( fnameLength <= 0 )	return;
	

	start = fname;
	for( p = &fname[fnameLength-1] ; p != start ; p-- )
		if( *p == '/' || *p == '\\' ){
			start = p+1;
			break;
		}


	strcpy( name, start);
}


void gpFilename_extension( char *fname, char *extension )
{
	gp_filename_extract_extension( fname ,extension, 256 );

}

/*********  gp_filename_extract_extension  **********
* @Description	:  gets the extension out of the filename
* 
* @Param   char *fname :  the filename
* @Param  char *extension :  buffer for the extension (".txt")
* @Param  int len :  length of the buffer
* @Return  void  :  
*****************************************/
void gp_filename_extract_extension( char *fname, char *extension, int maxLen )
{
int len,	i;
char *p,	*tp;
/*
	start = strrchr(fname,(int)'.');
	if ( start == NULL || len == 1)
	{
		extension[0] = '\0';
	}
	else
	{
		n = strlen(start);
		if ( len > 1 && n > len - 1)
			n = len - 1;
		strncpy(extension, start, n);
		extension[n] = '\0';
	}
*/

	extension[0] = '\0';

	len = strlen(fname);
	if( len == 0 )	return;

	for( p = &fname[len-1] ; p != fname && *p != '.' ; p-- );

	if( p == fname && *p != '.' )	return;

	for( tp = extension, i = 0 ; *p != 0 && i < maxLen; p++ )
		*tp++ = *p;

	*tp = 0;
}


void
gpFilename_remove_extension( char *fname )
{
	int len;
	char *p;

	len = strlen(fname);
	for( p = &fname[len-1] ; p != fname && *p != '\\' && *p != '/' ; p-- ){
		if( *p == '.' ){
			*p = 0;
			return;
		}
	}
}

void
gp_filename_remove_extension( char *fname )
{
int len;
char *p;

	len = strlen(fname);
	for( p = &fname[len-1] ; p != fname && *p != '\\' && *p != '/' ; p-- ){
		if( *p == '.' ){
			*p = 0;
			return;
		}
	}
}


int gpFilename_compare_extension( char *fname, char *extension )
{
int len;
char *p;


	len = strlen(fname);
	if( len == 0 )	return( -1 );

	for( p = &fname[len-1] ; p != fname && *p != '.' ; p-- );

	if( *p != '.' )	return( -1 );
	
	if( gp_stricmp( p, extension ) != 0 )	return( -1 );

	return( 1 );
}






void
gp_filename_force_extension( char *fname, char *extension )
{
/*
char	dir[256],	name[256],	ext[256];

	gp_filename_split( file, dir, name, ext );

	sprintf( file, "%s/%s%s", dir, name, extension );
*/

int len,	i;
char *p,	*sp;

	len = strlen(fname);
	for( p = &fname[len-1] ; p != fname && *p != '.' ; p-- );

	if( p == fname )
		p = &fname[len];


	for( sp = extension, i = 0 ; *sp != 0 ; sp++ )
		*p++ = *sp;

	*p = 0;
}

void
gpFilename_force_extension( char *fname1, char *extension, char *file )
{
	int len,	i;
	char *p,	*sp;

	strcpy( file, fname1 );

	len = strlen(file);
	for( p = &file[len-1] ; p != file && *p != '.' ; p-- );

	if( p == file )
		p = &file[len];


	for( sp = extension, i = 0 ; *sp != 0 ; sp++ )
		*p++ = *sp;

	*p = 0;
}

//void
//gpFilename_force_extension( char *fname, char *extension, char *file )
//{
//	char *tp,	*sp,	*p;
//
//	p = NULL;
//	for( sp = &fname[0], tp = &file[0] ; *sp != 0 ; sp++ ){
//		if(  *sp == '.' )	p = tp;
//		*tp++ =  *sp;
//	}
//
//	if( p != NULL )	tp = p;
//
//
//	for( sp = extension; *sp != 0 ; sp++ )
//		*tp++ = *sp;
//
//	*tp = 0;
//}


void
gpFilename_force_dir( char *inFile, char *dir, char *file )
{
char	fname[256];

	gp_filename_extract_FullName( inFile, fname, 256 );

	sprintf( file, "%s/%s", dir, fname );
}


void 
gp_filename_slash( char *fname )
{
char	*sp,	*tp;

	if( fname[0] == '\\' && fname[1] == '\\' )
		sp = &fname[2];
	else	sp = fname;


	for( tp = sp ; *sp != 0 ; sp++ ){
		if( *sp != '\\' && *sp != '/' ){
			*tp++ = *sp;
			continue;
		}
		*tp++ = '/';

		if( *(sp+1) == '\\' || *(sp+1) == '/' )	sp++;
	}

	*tp = 0;
}

void 
gpFilename_backslash( char *sfile, char *tfile )
{
	char	*sp,	*tp;

	for( sp = sfile, tp = tfile ; *sp != 0 ; sp++, tp++ ){
		if( *sp == '/' )
			*tp = '\\';
		else *tp = *sp;
	}

	for( sp = sfile, tp = tfile ; *sp != 0 ; sp++ ){
		if( *sp != '\\' || *(sp+1) != '\\' )
			*tp++ = *sp;
	}

	*tp = 0;
}




void
gp_filename_outFile( char *outFile, char *inFile, char *extension )
{
char	name[256],	file[256];

	if( outFile[0] == 0 ){
		strcpy( outFile, inFile );
		if( extension != NULL )
			gp_filename_force_extension( outFile, extension );
	}

	if( gpDir_exist( outFile ) == 1 ){
		gp_filename_extract_FullName( inFile, name, 256 );
		sprintf( file, "%s/%s", outFile, name );
		strcpy( outFile, file );
	}
	
	if( extension != NULL )
		gp_filename_force_extension( outFile, extension );
}


void
gpFilename_outFile( char *outName, char *inFile, char *extension, char *outFile )
{
	strcpy( outFile, outName );

	gp_filename_outFile( outFile, inFile, extension );
}


void
gp_filename_outDir( char *outDir, char *inFile )
{

	if( outDir[0] == 0 )
		gp_filename_extract_dir( inFile, outDir, 256 );

}


int gpFilename_path( char *fname, char *dir, char *path )
{
	if( fname[0] == '\\' || fname[0] == '/'  || fname[1] == ':' ){
		strcpy( path, fname );
		return( 1 );
	}

	sprintf( path, "%s\\%s", dir, fname );

	return( 1 );
}


int
gpFilename_is_number( char *file )
{
	char	fname[256];
	char	*p;
	int	i;

	gpFilename_name( file, fname );

	for( p = fname, i = 0 ; *p != 0  ; p++, i++ ){
		if( *p == '.' )	return( 1 );
		if( *p < '0'  || *p > '9' )
			return( -1 );
	}


	if( i == 0 )
		return( -1 );

	return( 1 );
}