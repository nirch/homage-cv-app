package com.homage.mattingapp;

import android.content.Context;

/**
 * Created by Yoram on 19/07/2015.
 */
public class vVideoEncodder {
    int a;
    public int open( int width, int height, int frameRate, String file )
    {
        return( 1 );
    }

    public int write( int width, int height, byte[] data )
    {
        return( 1 );
    }

    public int close()
    {
        return( 1 );
    }

    public void test()
    {
        a = 6;
    }
}
