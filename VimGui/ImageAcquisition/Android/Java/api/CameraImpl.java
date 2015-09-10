/**
 * 
 */
package com.xtr.api;

import java.io.IOException;

import android.annotation.TargetApi;
import android.app.Activity;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.os.Build;
import android.util.Log;
import android.view.TextureView;
import android.view.View;

/**
 * @author eyal
 * 
 *  
 *  The order of the calls to the camera should be: new Camera() ,
 *  mCamera.open(), mCamera.setPreviewTexture(ready-surface) , mCamera.startPreview()
 * 
 *  The order of the calls can be:			
 *  Init(), Start() SurfaceAvailable()  - typically on direct C++
 *  Init(), SurfaceAvailable(), Start() - typically on Unity3d   
 * 
 */
public class CameraImpl implements TextureView.SurfaceTextureListener {
	public static final int INVALID_CAMERA_ID = -1;
	public static final int DEFAULT_WIDTH = 640;
	public static final int DEFAULT_HEIGHT = 480;
	public static final int NV21_BITS_PER_PIXEL = 16;
	public static final int NUM_OF_CAMERA_BUFFERS = 1;

	public static final String TAG = "Xtr3d.java.CameraImpl";

	// Data Members:
	private Activity mActivity; // Hack to be able to init OpenGL texture.
	private long mContext; // Who to actually invoke during callbacks (C++
							// object, as long pointer)
	private Camera mCamera;
	private int mWidth;
	private int mHeight;

	private static final String m_tag = "CameraImpl";
	
//	Handler mHandler;
//	Thread t ;
	
	private TextureView mTextureView;
	private SurfaceTexture mSurface;

	public CameraImpl(long context) {
		mContext = context;
		mActivity = null;
		mCamera = null;

		mWidth = DEFAULT_WIDTH;
		mHeight = DEFAULT_HEIGHT;
	}

	//called from the Image-Aq. C++ code directly
	@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)  
	public void SetActivity(Activity activity) {
		mActivity = activity;

		mTextureView = new TextureView(mActivity);
		// TODO: verify the app's isHardwareAccelerated is TRUE!!!
		// The following code must be run in UI context, because of the need to
		// lock the canvas.
		/*
		 * runOnUiThread(new Runnable() {
		 * 
		 * @Override public void run() { Canvas c = m_textureView.lockCanvas();
		 * if (!c.isHardwareAccelerated()) { Log.w(TAG,
		 * "A TextureView or a subclass can only be " +
		 * "used with hardware acceleration enabled."); throw ??? } }
		 */

		mTextureView.setSurfaceTextureListener(this);
	}
	
	public void Start(final boolean shouldOutputBGR)
	{
//		Log.e(TAG, "start-main-thread");
		StartHere(shouldOutputBGR);

		
//      USING SEPERATE THREAD REDUCES PERFORMANCE!!!
//      see http://mindtherobot.com/blog/159/android-guts-intro-to-loopers-and-handlers/		
//		if (t==null)
//		{
//			 t = new Thread("looper-thread") {
//			 
//				@Override
//				public void run() {
//				  try {
//				    // preparing a looper on current thread     
//				    // the current thread is being detected implicitly
//				    Looper.prepare();
//				 
//				    // now, the handler will automatically bind to the
//				    // Looper that is attached to the current thread
//				    // You don't need to specify the Looper explicitly
//				    mHandler 	 = new Handler();
//				     
//				    // After the following line the thread will start
//				    // running the message loop and will not normally
//				    // exit the loop unless a problem happens or you
//				    // quit() the looper (see below)
//				    Looper.loop();
//				  } catch (Throwable t) {
//				    Log.e(TAG, "halted due to an error", t);
//				  } 
//				}
//				
//			};
//			t.setPriority(Thread.MAX_PRIORITY);
//			t.start();
//		}
//		FutureTask<Void> task = new FutureTask<Void>(new Runnable() {
//			
//			@Override
//			public void run() {
//				Log.e(TAG, "start-different-thread");
//				StartHere(shouldOutputBGR);
//				
//			}
//		}, null);
//		mHandler.post(task);
//		try{
//			task.get();
//		 } catch (Exception e)
//		 {
//			 Log.e(TAG,"future result exception",e);
//		 }
	}

	public void StartHere(boolean shouldOutputBGR) {
		//Log.i(TAG, "Start() called");
		if (mCamera != null) {
			Log.i(TAG,
					"Start(): mCamera is not null, has already been started, exiting");
			return;
		}
		boolean operationSuccessful = false;
		int numCameras = GetNumberOfCameras();
		int cameraItor;
		// First, find the first camera corresponding to front facing camera
		// (TODO: may need to use the config .xml in case there is an external
		// camera attached!)
		for (cameraItor = 0; cameraItor < numCameras; ++cameraItor) {
			if (CameraIsFrontFacing(cameraItor)) {
				break;
			}
		}
		if (cameraItor == numCameras) {
			Log.e(m_tag, "Couldn't find any front facing cameras!");
			return;
		}

		// Invoke Open:
		operationSuccessful = Open(cameraItor);
		if (!operationSuccessful) {
			Log.e(m_tag, "Open failed!");
			return;
		}

		// Set the preview callback:
		operationSuccessful = SetPreviewCallback();
		if (!operationSuccessful) {
			Log.e(m_tag, "SetPreviewCallback failed!");
			return;
		}

		
		//Log.v(TAG, "Start(): calling StartPreviewIfSurfaceAndCameraAreReady!");
		StartPreviewIfSurfaceAndCameraAreReady();
		//Log.v(TAG, "Start(): after calling StartPreviewIfSurfaceAndCameraAreReady!");
		
	}

	public boolean Stop() {
		if (mCamera == null) {
			Log.i(TAG,
					"Stop(): mCamera is null, Stop() has already been called");
			return true;
		}
		mCamera.stopPreview();
		mCamera.release();
		mCamera = null;
		return true;
	}

	public View GetView() {
		return mTextureView;
	}

	// Methods defined in interface TextureView.SurfaceTextureListener
	@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1)
	@Override
	public void onSurfaceTextureAvailable(SurfaceTexture surface, int width,
			int height) {
		//Log.i(TAG, "onSurfaceTextureAvailable() called");



		if (mWidth > width || mHeight > height) {
			Log.w(m_tag, "Surface dimensions (WxH):" + width + "x" + height
					+ "are smaller than the camera's: " + mWidth + "x"
					+ mHeight + ". Note that view might be trimmed");
		}

		surface.setDefaultBufferSize(mWidth, mHeight);

		try {
			// TODO: In case this isn't honeycomb, call an empty method!
			mSurface = surface;
			

			mTextureView.requestLayout();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//Log.v(TAG, "onSurfaceTextureAvailable(): calling StartPreviewIfSurfaceAndCameraAreReady!");
		StartPreviewIfSurfaceAndCameraAreReady();
		//Log.v(TAG, "onSurfaceTextureAvailable(): after calling StartPreviewIfSurfaceAndCameraAreReady!");
	}

	@Override
	public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
		Log.i(TAG, "onSurfaceTextureDestroyed() called");
		return Stop();
	}

	@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
	@Override
	public void onSurfaceTextureSizeChanged(SurfaceTexture arg0, int arg1,
			int arg2) {
		Log.i(TAG, "onSurfaceTextureSizeChanged() called");
		// Size ignored, as camera does all the work for us
		mTextureView.requestLayout();
	}

	@Override
	public void onSurfaceTextureUpdated(SurfaceTexture surface) {
		// Invoked every time there's a new Camera preview frame. Not used.
		//mTextureView.setVisibility(View.INVISIBLE);
	}

	// Private methods and members:
	private int GetNumberOfCameras() {
		return Camera.getNumberOfCameras();
	}

	private boolean CameraIsFrontFacing(int index) {
		boolean result = false;
		try {
			Camera.CameraInfo camInfo = new Camera.CameraInfo();
			Camera.getCameraInfo(index, camInfo);
			result = (camInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT);
		} catch (OutOfMemoryError eMem) {
			Log.e(m_tag, "Out of memory!");
		} catch (Exception e) {
			Log.e(m_tag, "Unknown error!");
		}

		return result;
	}

	private boolean Open(final int index) {
		boolean result = false;
		try {
			
		
			 
		    try {
		    	mCamera =android.hardware.Camera.open(index);
		    	
		    } catch (Exception e) {
		    	Log.e(m_tag, "Camera Open failed with error!",e);
		    }
			result = (mCamera != null);

			Camera.Parameters paramsToSet = mCamera.getParameters();

			paramsToSet.setPreviewSize(mWidth, mHeight);
			int format = paramsToSet.getPreviewFormat();
			if (format != ImageFormat.NV21) {
				Log.w(m_tag, "Default format is NOT NV21. Setting to NV21...");
			}
			paramsToSet.setPreviewFormat(ImageFormat.NV21); // Make sure the
															// android default
															// is set.

			mCamera.setParameters(paramsToSet);

			format = mCamera.getParameters().getPreviewFormat();
			if (format != ImageFormat.NV21) {
				Log.e(m_tag, "Setting to NV21 to failed!");
				result = false;
			}

		} catch (RuntimeException e) {
			Log.e(m_tag, "Runtime error!");
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		return result;
	}

	private boolean SetPreviewCallback() {
		boolean result = false;
		try {
			CameraPreviewCallback cb = new CameraPreviewCallback(mContext);
			int bufferSize = (DEFAULT_WIDTH * DEFAULT_HEIGHT
					* NV21_BITS_PER_PIXEL / 8);
			for (int i = 0; i < NUM_OF_CAMERA_BUFFERS; i++) {
				mCamera.addCallbackBuffer(new byte[bufferSize]);
			}

			mCamera.setPreviewCallbackWithBuffer(cb);
			result = true;
		} catch (OutOfMemoryError eMem) {
			Log.e(m_tag, "Out of memory!");
		} catch (Exception e) {
			Log.e(m_tag, "Unknown error!");
		}

		return result;
	}
	//note: this method can be called multiple times without any ill effects (even 3 times in a row)
	private void StartPreviewIfSurfaceAndCameraAreReady() {
		
		if (mCamera==null || mSurface==null)
			return;
		
		try {
			mCamera.setPreviewTexture(mSurface);
		} catch (IOException e)
		{
			Log.e(m_tag,e.getMessage());
		}
		
		


		mCamera.startPreview();

		//Log.i(TAG, "StartPreviewIfSurfaceAndCameraAreReady exit");

	}

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}

}
