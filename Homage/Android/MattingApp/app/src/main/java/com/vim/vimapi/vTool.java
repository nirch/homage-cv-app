package com.vim.vimapi;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;


public class vTool {

	public static void log(String msg) {
		Log.d("mychic.JNILog", msg);
	}

	public static Bitmap CreateBitmapFromRGB(int width, int height,
			byte[] buffer) {
		Bitmap bm = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		bm.copyPixelsFromBuffer(ByteBuffer.wrap(buffer));

		return bm;
	}
	


	
	
	public static int writeJpeg(byte[] data, int width, int height, int quality, String file) {
	
		Bitmap bm = CreateBitmapFromRGB(width, height, data);
		
		int ret = writeJpeg( bm, quality, file );
		
		return( ret );

	}
	

	public static int writeJpeg(byte[] data, int width, int height, String file) {
		
		Bitmap bm = CreateBitmapFromRGB(width, height, data);
		
		int ret = writeJpeg( bm, 60, file );
		
		return( ret );
		

	}
	
	
	public static int writeJpeg( Bitmap bm, int quality, String file)
	{
	
		try {

			// Open output stream
			FileOutputStream fOut = new FileOutputStream( file, false);
			// write integers as separated ascii's
			bm.compress(CompressFormat.JPEG, quality, fOut);
			// fOut.write(data);
			// Close output stream
			fOut.flush();
			fOut.close();
		} catch (IOException e) {
			e.printStackTrace();
			return 0;
		}

		return (1);
	}

	
	public static int write(byte[] data, String file)
	{


		try {

			// Open output stream
			FileOutputStream fOut = new FileOutputStream(file, false);
			// write integers as separated ascii's
			fOut.write(data);
			// Close output stream
			fOut.flush();
			fOut.close();
		} catch (IOException e) {
			e.printStackTrace();
			return 0;
		}

		return (1);
	}
	
	
	public static byte[] Bitmap2Byte( Bitmap bmp )
	{
		
		  int size     = bmp.getRowBytes() * bmp.getHeight();
		  ByteBuffer b = ByteBuffer.allocate(size);

		  bmp.copyPixelsToBuffer(b);

		  byte[] data = new byte[size];

		  try {
		    b.get( data, 0, data.length);
		  } catch (BufferUnderflowException e) {
		     // always happens
		  }
	
		  return( data );
	}
	
	public static String getSaveFullPath( String file)
	{
		String path = getSavePath();
	

		boolean exists = (new File(path)).exists();
		if (!exists) {
			new File(path).mkdirs();
		}
		// Open output stream
		return (path + file );
			
	
	}
	
	public static String getExtrnalPath( String dir, String file )
	{
		String path = getSavePath( dir );
	
		boolean exists = (new File(path)).exists();
		if (!exists) {
			new File(path).mkdirs();
		}
		// Open output stream
		return (path + file );
			
	
	}
			

	public static String getSavePath() {
		return Environment.getExternalStorageDirectory().getAbsolutePath()
				+ "/Android/data/com.mychic.virtualmirror/files/";
	}
	
	public static String getSavePath( String dir) {
		return Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + dir + "/";
//				+ "/Android/data/com.mychic.virtualmirror/files/";
	}

	public static String getFileFromURL(String url) {
		return url.substring(url.lastIndexOf('/') + 1, url.length());
	}

	public static int calculateInSampleSize(BitmapFactory.Options options,
			int reqWidth, int reqHeight) {
		// Raw height and width of image
		final int height = options.outHeight;
		final int width = options.outWidth;
		int inSampleSize = 1;

		if (height > reqHeight || width > reqWidth) {

			// Calculate ratios of height and width to requested height and
			// width
			final int heightRatio = Math.round((float) height
					/ (float) reqHeight);
			final int widthRatio = Math.round((float) width / (float) reqWidth);

			// Choose the smallest ratio as inSampleSize value, this will
			// guarantee
			// a final image with both dimensions larger than or equal to the
			// requested height and width.
			inSampleSize = heightRatio < widthRatio ? heightRatio : widthRatio;
		}

		return inSampleSize;
	}

	public static Bitmap decodeSampledBitmapFromResource(Resources res,
			int resId, int reqWidth, int reqHeight) {

		// First decode with inJustDecodeBounds=true to check dimensions
		final BitmapFactory.Options options = new BitmapFactory.Options();
		options.inJustDecodeBounds = true;
		BitmapFactory.decodeResource(res, resId, options);

		// Calculate inSampleSize
		options.inSampleSize = calculateInSampleSize(options, reqWidth,
				reqHeight);

		// Decode bitmap with inSampleSize set
		options.inJustDecodeBounds = false;
		return BitmapFactory.decodeResource(res, resId, options);
	}

	
	public static Bitmap readBitmap( Resources res, int resId )
	{
		// First decode with inJustDecodeBounds=true to check dimensions
		final BitmapFactory.Options options = new BitmapFactory.Options();

		options.inScaled = false;
		options.inJustDecodeBounds = false;
		return BitmapFactory.decodeResource(res, resId, options);
	}
	
	
	
	public static Bitmap readBitmap( String file )
	{
		// First decode with inJustDecodeBounds=true to check dimensions
		final BitmapFactory.Options options = new BitmapFactory.Options();

		options.inScaled = false;
		options.inJustDecodeBounds = false;
		return BitmapFactory.decodeFile( file, options);
	}


	
	
	public static boolean isTablet(Context context) {
		return (context.getResources().getConfiguration().screenLayout & Configuration.SCREENLAYOUT_SIZE_MASK) >= Configuration.SCREENLAYOUT_SIZE_LARGE;
	}
	

}
