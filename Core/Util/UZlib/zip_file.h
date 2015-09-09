#ifndef _zip_file_H
#define _zip_file_H

#ifdef __cplusplus
extern "C" {
#endif


// ***************  zip_file.c ***************  //

/*
ziping file filename to zipfilename, filename not deleted 

char *filename    - name of file to copress
char *zipfilename - name of zip file
	DEFAULT: zipfilename==NULL  -  zipfilename = filename.zip 
int level contain the level of compression 
	DEFAULT: level==-1  -  level = Z_DEFAULT_COMPRESSION)
int size_buffer - buffer of memory for reading filename
	size_buffer > 0 && size_buffer <= 1048576
	DEFAULT: size_buffer==-1  - size_buffer = WRITEBUFFERSIZE = 16384
int opt_overwrite - overwrite zip file
	opt_overwrite==1 - overwrite
	opt_overwrite==2 - additing
	DEFAULT: opt_overwrite==-1 - overwrite
return 0 if OK, else no 0
*/
int zip_file( char *filename, char *zipfilename,
			  int level, int size_buffer, int opt_overwrite );



// ***************  zip_files.c ***************  //

/*
ziping all files in folder pathname ( not in subdir )
file name.ext ziping to name.zip and deleted ( not *.zip filse )

pathname   - pathname to folder
deltaClock - only files modified befor deltaClock clock
	deltaClock == -1  -  deltaClock=DELTA_CLOCK_DEFAULT
	DELTA_CLOCK_DEFAULT = 24
return 0 if OK, else no 0
*/
int zip_files_date( char *pathname, int deltaClock );


#ifdef __cplusplus
}
#endif

#endif /* _zip_file_H */
