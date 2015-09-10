/**
 * 
 */
package com.xtr.api;

import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;

/**
 * @author eyal
 *
 */
public final class CameraPreviewCallback implements PreviewCallback 
{
	public CameraPreviewCallback(long contextAsLong)
	{
		//Log.v("CameraPreviewCallback", "context addr: " + contextAsLong);
		
		mContext = contextAsLong;
	}
	
	public CameraPreviewCallback(CameraImpl camImpl)
	{	
		m_camImpl = camImpl;
	}	
	
	/* (non-Javadoc)
	 * @see android.hardware.Camera.PreviewCallback#onPreviewFrame(byte[], android.hardware.Camera)
	 */
	@Override
	public void onPreviewFrame(byte[] frame, Camera camera) 
	{
		//Log.v("CameraPreviewCallback", "onPreviewFrame invoked with frame addr: " + frame);		
		//m_camImpl.onReceivedNewImage(frame);

		//call native method with frame + TODO: timestamp
		callMe(frame, mContext);
	
		camera.addCallbackBuffer(frame);
				
	}

	private native void callMe(byte[] frame, long context);  

	
	private long mContext;	
	
	private CameraImpl m_camImpl;
	
}
