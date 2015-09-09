#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <io.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>


#ifndef WIN32
# include <unistd.h>
# include <utime.h>
# include <sys/types.h>
# include <sys/stat.h>
#else
# include <direct.h>
# include <io.h>
#endif

#include "zip.h"
#include "Uigp/igp.h"


#ifdef WIN32
#define USEWIN32IOAPI
#include "zip_iowin32.h"
#endif



#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

#ifdef WIN32
uLong filetime(f, tmzip, dt)
    char *f;                /* name of file to get info on */
    tm_zip *tmzip;             /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
  int ret = 0;
  {
	  
      FILETIME ftLocal;
      HANDLE hFind;
      WIN32_FIND_DATA  ff32;

      hFind = FindFirstFile(f,&ff32);
      if (hFind != INVALID_HANDLE_VALUE)
      {
        FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
        FileTimeToDosDateTime(&ftLocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
        FindClose(hFind);
        ret = 1;
      }
  }
  return ret;
}
#else
#ifdef unix
uLong filetime(f, tmzip, dt)
    char *f;                /* name of file to get info on */
    tm_zip *tmzip;             /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
  int ret=0;
  struct stat s;        /* results of stat() */
  struct tm* filedate;
  time_t tm_t=0;

  if (strcmp(f,"-")!=0)
  {
    char name[MAXFILENAME+1];
    int len = strlen(f);

    strncpy(name, f,MAXFILENAME-1);
    /* strncpy doesnt append the trailing NULL, of the string is too long. */
    name[ MAXFILENAME ] = '\0';

    if (name[len - 1] == '/')
      name[len - 1] = '\0';
    /* not all systems allow stat'ing a file with / appended */
    if (stat(name,&s)==0)
    {
      tm_t = s.st_mtime;
      ret = 1;
    }
  }
  filedate = localtime(&tm_t);

  tmzip->tm_sec  = filedate->tm_sec;
  tmzip->tm_min  = filedate->tm_min;
  tmzip->tm_hour = filedate->tm_hour;
  tmzip->tm_mday = filedate->tm_mday;
  tmzip->tm_mon  = filedate->tm_mon ;
  tmzip->tm_year = filedate->tm_year;

  return ret;
}
#else
uLong filetime(f, tmzip, dt)
    char *f;                /* name of file to get info on */
    tm_zip *tmzip;             /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
    return 0;
}
#endif
#endif




int check_exist_file(filename)
    const char* filename;
{
    FILE* ftestexist;
    int ret = 1;
    ftestexist = fopen(filename,"rb");
    if (ftestexist==NULL)
        ret = 0;
    else
        fclose(ftestexist);
    return ret;
}

/* calculate the CRC32 of a file,
   because to encrypt a file, we need known the CRC32 of the file before */
int getFileCrc(const char* filenameinzip,void*buf,unsigned long size_buf,unsigned long* result_crc)
{
   unsigned long calculate_crc=0;
   int err=ZIP_OK;
   FILE * fin = fopen(filenameinzip,"rb");
   unsigned long size_read = 0;
   unsigned long total_read = 0;
   if (fin==NULL)
   {
       err = ZIP_ERRNO;
   }

    if (err == ZIP_OK)
        do
        {
            err = ZIP_OK;
            size_read = (int)fread(buf,1,size_buf,fin);
            if (size_read < size_buf)
                if (feof(fin)==0)
            {
                printf("error in reading %s\n",filenameinzip);
                err = ZIP_ERRNO;
            }

            if (size_read>0)
                calculate_crc = crc32(calculate_crc,buf,size_read);
            total_read += size_read;

        } while ((err == ZIP_OK) && (size_read>0));

    if (fin)
        fclose(fin);

    *result_crc=calculate_crc;
    printf("file %s crc %x\n",filenameinzip,calculate_crc);
    return err;
}


/*
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
			  int level, int size_buffer, int opt_overwrite )
{
char zipfilename_try[MAXFILENAME+16];
char filenameinzip[MAXFILENAME];
char dir_filename[MAXFILENAME], dir_zipfilename[MAXFILENAME];
int size_buf=0;
void* buf=NULL;
const char* password=NULL;
int opt_compress_level=Z_DEFAULT_COMPRESSION;
int err=0;

int i, len;
int dot_found=0;

zlib_filefunc_def ffunc;
zipFile zf;

FILE * fin;
int size_read;
zip_fileinfo zi;
unsigned long crcFile=0;


	// if the file don't exist, we not work
	if ( filename == NULL || check_exist_file(filename)==0 )
    {
        printf("file don't exist\n");
        return ZIP_PARAMERROR;
    }


    //seting zipfilename_try
	if (zipfilename != 0 )
	    strncpy(zipfilename_try, zipfilename, MAXFILENAME-1);
	else
		strncpy(zipfilename_try, filename, MAXFILENAME-1);
    // strncpy doesnt append the trailing NULL, of the string is too long. 
    zipfilename_try[ MAXFILENAME-1 ] = '\0';

    len=(int)strlen(zipfilename_try);
	for ( i = len-1, dot_found= 0; i >= 0; i-- )
	{
		if ( zipfilename_try[i]=='/' || zipfilename_try[i]=='\\') 	
			break;
		if (zipfilename_try[i]=='.') 	{
			dot_found=i;
			break;
		}
	}

    if (dot_found==0)
		strcat(zipfilename_try,".zip");
	else if ( zipfilename == 0 ) {
		//zipfilename_try from filename
		zipfilename_try[dot_found] = 0;
		strcat(zipfilename_try,".zip");	
	}

	if ( gp_strnicmp(zipfilename_try, filename, MAXFILENAME ) == 0 )
	{
        printf("file name %s eqwil zip name %s\n", filename, zipfilename_try);
        return ZIP_PARAMERROR;
	}


	//setting filenameinzip - name file in zip file
	strncpy( filenameinzip, filename, MAXFILENAME );

	gp_filename_extract_dir(filename, dir_filename, MAXFILENAME);
	gp_filename_extract_dir(zipfilename_try, dir_zipfilename, MAXFILENAME);

	if ( gp_strnicmp(dir_filename, dir_zipfilename, MAXFILENAME ) == 0 ) 
		gp_filename_extract_FullName( filename, filenameinzip, MAXFILENAME ); 
	else if ( gp_strnicmp(dir_filename, dir_zipfilename, strlen(dir_zipfilename) ) == 0 ) {
		char name[MAXFILENAME];
		gp_filename_extract_FullName( filename, name, MAXFILENAME ); 
		strncpy( filenameinzip, dir_filename+strlen(dir_zipfilename), MAXFILENAME );
		strcat( filenameinzip, "/" );
		strcat( filenameinzip, name );
	} 
	else {
		char *pos;
		if ( ( pos = strchr( filename, ':' ) ) != NULL )
			strncpy( filenameinzip, pos+1, MAXFILENAME );
	}

	filenameinzip[ MAXFILENAME-1 ] = 0;


	opt_compress_level = Z_DEFAULT_COMPRESSION;
	if ( level >= 0 && level <= 9 )
		opt_compress_level = level;


    size_buf = WRITEBUFFERSIZE; //16384
	if ( size_buffer > 0 && size_buffer <= 1048576)
		size_buf = size_buffer;

    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
        printf("Error allocating memory\n");
        return ZIP_INTERNALERROR;
    }


	if ( opt_overwrite <= 0 )  
		opt_overwrite = 1;

    if (opt_overwrite==2)
    {
		// if the file don't exist, we not append file 
        if (check_exist_file(zipfilename_try)==0)
			opt_overwrite=1;
    }


#ifdef USEWIN32IOAPI
	fill_win32_filefunc(&ffunc);
    zf = zipOpen2(zipfilename_try,(opt_overwrite==2) ? 2 : 0,NULL,&ffunc);
#else
    zf = zipOpen(zipfilename_try,(opt_overwrite==2) ? 2 : 0);
#endif
    if (zf == NULL) {
		printf("error opening %s\n",zipfilename_try);
	    free(buf);
		return ZIP_ERRNO;
    }

    zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
    zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;
	filetime(filename,&zi.tmz_date,&zi.dosDate);

	//err = zipOpenNewFileInZip(zf,filenameinzip,&zi,
	//                          NULL,0,NULL,0,NULL, //comment
	//                          (opt_compress_level != 0) ? Z_DEFLATED : 0,
	//                          opt_compress_level);

	//if ((password != NULL) && (err==ZIP_OK))
	//	err = getFileCrc(filename,buf,size_buf,&crcFile);

	err = zipOpenNewFileInZip3(zf,filenameinzip,&zi,
					 NULL,0,NULL,0,NULL ,// comment
					 (opt_compress_level != 0) ? Z_DEFLATED : 0,
					 opt_compress_level,0,
					 // -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, 
					 -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
					 password,crcFile);
	if (err != ZIP_OK) {
		printf("error in opening %s in zipfile\n",filenameinzip);
	    free(buf);
		if ( zipClose(zf,NULL) != ZIP_OK)
            printf("error in closing %s\n",zipfilename_try);
		return ZIP_ERRNO;
	}

	fin = fopen(filename,"rb");
    if (fin == NULL)
    {
        printf("error in opening %s for reading\n",filename);
	    free(buf);
		if ( zipCloseFileInZip(zf) != ZIP_OK)
			printf("error in closing %s in the zipfile\n",
								filenameinzip);
		if ( zipClose(zf,NULL) != ZIP_OK)
            printf("error in closing %s\n",zipfilename_try);
		return ZIP_ERRNO;
	}


    err= ZIP_OK;

	do
	{
		size_read = (int)fread(buf,1,size_buf,fin);
		if (size_read < size_buf && feof(fin)==0)
		{
			printf("error in reading %s\n",filename);
			err = ZIP_ERRNO;
		}

		if (size_read>0)
		{
			err = zipWriteInFileInZip (zf,buf,size_read);
			if (err<0)
				printf("error in writing %s in the zipfile\n",
								 filenameinzip);
		}
	} while ((err == ZIP_OK) && (size_read>0));

	fclose(fin);

	err = zipCloseFileInZip(zf);
	if (err!=ZIP_OK)
			printf("error in closing %s in the zipfile\n",
								filenameinzip);
	err = zipClose(zf,NULL);
    if (err != ZIP_OK)
            printf("error in closing %s\n",zipfilename_try);

    free(buf);

    return err;
}
