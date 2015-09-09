package com.vim.vimapi.Camera;

import java.io.File;
import java.util.List;


import android.app.Activity;
import android.view.SurfaceHolder.Callback;


import android.hardware.Camera;
import android.hardware.Camera.Face;
import android.hardware.Camera.FaceDetectionListener;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;

import android.hardware.Camera.Face;
import android.hardware.Camera.FaceDetectionListener;

import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;






public class CameraPreview implements SurfaceHolder.Callback {



	public static int		RESOLUTION_LOW = 1;
	public static int		RESOLUTION_MED = 2;
	public static int		RESOLUTION_HIGH = 3;
	
	
	public static int		BACK_CAMERA = 0;
	public static int		FRONT_CAMERA = 1;


	


	public static final int NUM_OF_CAMERA_BUFFERS = 1;
	
	private Camera mCam;
    private SurfaceView mCamSV;
    private PreviewCallback mPreviewCallback;


    public int m_width = 640;
    public int m_height = 480;

    private int m_faceDetect = 0;

	// locks
	private Object m_imageLock = new Object();


	public CameraPreview(SurfaceView SV , PreviewCallback previewCallback )
	{
    	mCamSV = SV;
    	mPreviewCallback = previewCallback;//new CameraPreviewCallback();//previewCallback;

	}


//	public void setResolution( enumCameraResolution resolution ){
//		switch (resolution) {
//		case RESOLUTION_LOW:
//			m_width = 320;
//			m_height = 240;
//			break;
//		case RESOLUTION_MED:
//			m_width = 640;
//			m_height = 480;
//			break;
//		case RESOLUTION_HIGH:
//			m_width = 720;
//			m_height = 1280;
//			break;
//		}
//	}

	

	public void setResolution( int resolution ){
		
		if( resolution == RESOLUTION_LOW ) {
			m_width = 320;
			m_height = 240;
			return;
		}
		
		
		if( resolution == RESOLUTION_MED ){
			m_width = 640;
			m_height = 480;
			return;
		}
		
		
		if( resolution ==  RESOLUTION_HIGH ){
			m_width = 720;
			m_height = 1280;
			
			return;
		}
	}

    public void open( int cameraId )
    {


		if( mCamSV != null ){
			SurfaceHolder holder = mCamSV.getHolder();
			holder.addCallback(this);
		}



		mCam = Camera.open( cameraId );

		setDesiredCameraParameters();

		Camera.Parameters parameters = mCam.getParameters();

		m_width = parameters.getPreviewSize().width;
		m_height = parameters.getPreviewSize().height;


		int bufferSize = (int)(m_width*m_height*1.5);
		for (int i = 0; i < NUM_OF_CAMERA_BUFFERS; i++) 
			mCam.addCallbackBuffer( new byte[bufferSize] );
		
		//mCam.setPreviewCallback( mPreview );
		mCam.setPreviewCallback( mPreviewCallback );//new CameraPreviewCallback() );
//		mCam.startPreview();
    }

	public void close() {
		if (mCam != null) {
//			mCam.onStop();
			mCam = null;
		}
	}

	public void pause() {
//		mCam.onPause();
	}

	public void resume() {
//		mCam.onResume();
	}


	void setDesiredCameraParameters() {
		Log.d("mychic.camera","6");
		Camera.Parameters parameters = mCam.getParameters();

		if (parameters == null) {
			return;
		}

		/*
		String focusMode = findSettableValue(
				parameters.getSupportedFocusModes(),
				Camera.Parameters.FOCUS_MODE_AUTO,
				Camera.Parameters.FOCUS_MODE_MACRO);

		List<Camera.Size> sizes = parameters.getSupportedPictureSizes();
		Size picturSize = findProperPicureSize(sizes);

		if (picturSize != null) {
			parameters.setPictureSize(picturSize.width, picturSize.height);
			Log.i("", "SIZE  :  WIDTH :" + picturSize.width + " HEIGHT :" + picturSize.height);
		}

		if (focusMode != null) {
			parameters.setFocusMode(focusMode);
		}
		*/
		parameters.setPreviewSize( m_width, m_height );


		parameters.setPreviewFpsRange(10000, 15000);

		mCam.setParameters(parameters);
	}


	public int getWidth() {
		return (m_width);
	}

	public int getHeight() {
		return (m_height);
	}


	public void surfaceCreated(SurfaceHolder holder)
    {
        // No action required



	      try {
	         mCam.setPreviewDisplay(holder);


	           mCam.startPreview();

	          if( m_faceDetect == 1 )
	        	  startFaceDetection(); // start face detection feature

	      } catch (Exception e) {
//	          Log.d(TAG, "Error setting camera preview: " + e.getMessage());
	      }


    }

/*
	// Called when surface dimensions etc change
    public void surfaceChanged(SurfaceHolder sh,
                               int format,
                               int width,
                               int height)
    {
        // Start camera preview
    	Log.d("mychic.camera","surefaceChanged");
    	startCamera(sh, width, height);


    	if( m_faceDetect == 1 )
    		startFaceDetection(); // start face detection feature
    }
  */

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {

        if (holder.getSurface() == null){
            // preview surface does not exist
//            Log.d(TAG, "mHolder.getSurface() == null");
            return;
        }

        try {
            mCam.stopPreview();

        } catch (Exception e){
            // ignore: tried to stop a non-existent preview
//            Log.d(TAG, "Error stopping camera preview: " + e.getMessage());
        }


        try {
        	mCam.setPreviewDisplay(holder);
        	mCam.startPreview();

        	if( m_faceDetect == 1 )
        		startFaceDetection(); // re-start face detection feature

        } catch (Exception e){
            // ignore: tried to stop a non-existent preview
//            Log.d(TAG, "Error starting camera preview: " + e.getMessage());
        }
    }

    // Called when the surface is closed/destroyed
    public void surfaceDestroyed(SurfaceHolder sh)
    {
        // No action required
    }



    // Setup camera based on surface parameters
    public void startCamera(SurfaceHolder sh, int width, int height)
    {
    	Log.d("mychic.camera","startCamera");
    	Camera.Parameters p = mCam.getParameters();
		for(Camera.Size s : p.getSupportedPreviewSizes())
		{
		    // In this instance, simply use the first available
		    // preview size; could be refined to find the closest
		    // values to the surface size
		    p.setPreviewSize(s.width, s.height);
		    break;
		}

		mCam.setParameters(p);

		try
		{
		    mCam.setPreviewDisplay(sh);
		}
		catch(Exception e)
		{
		    // Log surface setting exceptions
		}


		start();
    }


    // Stop camera when application ends
    public void start()
    {
	      mCam.startPreview();
    }


    // Stop camera when application ends
    public void stop()
    {
    	Log.d("mychic.camera","stopCamera");
    	SurfaceHolder holder = mCamSV.getHolder();
    	holder.removeCallback(this);
       // mCamSH.removeCallback(this);

    	mCam.stopPreview();
    	mCam.release();
    }





     public void startFaceDetection(){
    	    // Try starting Face Detection
    	    Camera.Parameters params = mCam.getParameters();

    	    // start face detection only *after* preview has started
    	    if (params.getMaxNumDetectedFaces() > 0){
    	        // camera supports face detection, so can start it:
    	    	mCam.startFaceDetection();
    	    }
    	}

     FaceDetectionListener faceDetectionListener = new FaceDetectionListener(){
 		@Override
 		public void onFaceDetection(Face[] faces, Camera camera) {
 			if (faces.length > 0 && faces[0].rect != null) {
 //				Face face = faces[0];
 //				faceRect = faces[0].rect;
 //				Log.d("mychic.camera", "Face detection : FOUND face = " + String.valueOf(faces.length));

 	            Log.d("FaceDetection", "face detected: "+ faces.length +
 	                    " Face 1 Location X: " + faces[0].rect.centerX() +
 	                    "Y: " + faces[0].rect.centerY() );

 			}
 		}
 	};



	public static int getCameraOrientation(int cameraId) {
		android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
		android.hardware.Camera.getCameraInfo(cameraId, info);

		return info.orientation;
	}

}
