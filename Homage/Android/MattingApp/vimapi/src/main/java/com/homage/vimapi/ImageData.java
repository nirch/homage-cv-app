package com.vim.vimapi;

import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;

import android.graphics.Bitmap;



public class ImageData {
	
	public int m_width;
	public int m_height;
	public int m_depth;

	public byte[] m_data;
	
	public int 	m_use;
	
	public ImageData()
	{
		m_width = 0;
		m_height = 0;
		m_depth = 0;
		m_data = null;
		m_use = 0;
	}
		
	public ImageData( int width, int height, byte[] data )
	{
		m_width = width;
		m_height = height;
		m_data = data;
		m_use = 0;
	}
	
	
	public ImageData( int width, int height )
	{
		m_width = width;
		m_height = height;
		m_depth = 4;
		
		m_data = new byte[4*width*height];
		
		m_use = 0;
	}
	
	public ImageData( int width, int height, int size )
	{
		m_width = width;
		m_height = height;
		
		m_data = new byte[size];
		
		m_use = 0;
	}
	
	public byte[] getData ()
	{
		return m_data;
	}

	public void SetColor( byte r, byte g, byte b )
	{
		int	i,	k;
		for( i = 0, k = 0 ; i < m_width * m_height ; i++ ){
			m_data[k++] = 0;
			m_data[k++] = r;
			m_data[k++] = g;
			m_data[k++] = b;
		}
	}
	
	
	public void SetColor( byte[] yuv )
	{
		int	i,	k;
		for( i = 0, k = 0 ; i < m_width * m_height ; i++ ){
			m_data[k++] = 0;
			m_data[k++] = yuv[i];
			m_data[k++] = yuv[i];
			m_data[k++] = yuv[i];
		}
	}
	
	
	public ImageData( Bitmap bmp )
	{
		m_width = bmp.getWidth();
		m_height = bmp.getHeight();
		
		
		
		
		int size     = bmp.getRowBytes() * bmp.getHeight();
		ByteBuffer b = ByteBuffer.allocate(size);
		
		bmp.copyPixelsToBuffer(b);
		
		
		
		m_data = new byte[size];

		
		try {
		     b.get( m_data, 0, m_data.length);
		} catch (BufferUnderflowException e) {
		     // always happens
		}
	
	
		m_use = 0;
	}
	
	
	public ImageData( Bitmap bmp, int depth )
	{
		m_width = bmp.getWidth();
		m_height = bmp.getHeight();
		m_depth = depth;
		
		
		int[] pixels = new int[bmp.getHeight()*bmp.getWidth()];
		pixels[0] = 0;

		bmp.getPixels(pixels, 0, bmp.getWidth(), 0, 0, bmp.getWidth(), bmp.getHeight());
		
	


		
		int size     = bmp.getRowBytes() * bmp.getHeight();
		ByteBuffer b = ByteBuffer.allocate(size);
		
		m_data = new byte[m_depth*m_width*m_height];
			
		int	i,	j,	x;
		for( i = 0, j = 0 ; i < pixels.length ; i++ )
		{
			x = pixels[i];
			
			if( m_depth == 4 )
				m_data[j++] = (byte) ((x) >> 24);
			
			 m_data[j++] = (byte) ((x) >> 16);
			 m_data[j++] = (byte) ((x) >> 8);
			 m_data[j++] = (byte) ((x) /*>> 0*/);
		}
		
			
		m_use = 0;
	}
	
	
	public Bitmap toBitmap ( Bitmap bmp )
	{
		if( bmp == null )
			bmp = Bitmap.createBitmap( m_width, m_height, Bitmap.Config.ARGB_8888 );
			
		bmp.copyPixelsFromBuffer(ByteBuffer.wrap(m_data));

		return bmp;

	}
	
	
	public int writeJpeg( int quality, String file) {
		
		Bitmap bm = vTool.CreateBitmapFromRGB( m_width, m_height, m_data );
		
		int ret = vTool.writeJpeg( bm, quality, file );
		
		return( ret );
		

	}
	
	
	public static ImageData read( String file ) {
		
		Bitmap bmp = vTool.readBitmap( file );
		
		if( bmp == null )
			return( null );
		
		ImageData im = new ImageData( bmp );
		
		
		return( im );
		

	}
	
}

