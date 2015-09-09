/**
 *
 */
package com.vim.vimapi.Camera;

import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.util.Log;


public final class CameraPreviewCallback implements PreviewCallback
{


	@Override
	public void onPreviewFrame(byte[] frame, Camera camera)
	{
		//Log.v("CameraPreviewCallback", "onPreviewFrame invoked with frame addr: " + frame);
		//m_camImpl.onReceivedNewImage(frame);

		//call native method with frame + TODO: timestamp
		//callMe(frame, mContext);


		Camera.Parameters parameters = camera.getParameters();
		int width = parameters.getPreviewSize().width;
		int height = parameters.getPreviewSize().height;



		camera.addCallbackBuffer(frame);

	}

	//private native void callMe(byte[] frame, long context);

}
