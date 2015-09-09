package com.homage.renderer;

/**
 * Created by Yoram on 19/07/2015.
 */
public class vVideoEncodderI {

    int open( int width, int height, int frameRate, string file )
    {
        return( 1 );
    }

    int write( int width, int hieght, byte[] data )
    {
        return( 1 );
    }

    int close()
    {
        return( 1 );
    }
}
