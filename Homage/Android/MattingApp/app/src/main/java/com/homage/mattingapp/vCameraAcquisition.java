package  com.homage.mattingapp;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.widget.Button;
import android.widget.FrameLayout;

import com.homage.matting.Matting;
import com.vim.vimapi.ImageData;
import com.vim.vimapi.vTool;
import com.vim.vimapi.Camera.*;




public class vCameraAcquisition  implements PreviewCallback
{


	private vCamera m_cameraPreview;

	


//	private enumCameraResolution m_resolution = enumCameraResolution.MED;
	private int m_width = 640;
	private int m_height = 480;



	
	public vCameraAcquisition()
	{


		
		m_cameraPreview = null;

//		m_resolution = enumCameraResolution.MED;
	}

	
	
	public int open( int iCamera, int resolution )
	{
				
		if (m_cameraPreview == null) 
			m_cameraPreview = new vCamera( null, this );
		
				
		m_cameraPreview.setResolution( resolution );

		m_cameraPreview.open( iCamera );
		
		m_cameraPreview.start();


		
		m_width = m_cameraPreview.getWidth();
		m_height = m_cameraPreview.getHeight();
		

		return (1);
	}
	
	
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


        Matting.cameraCallBack( width, height, frame );

		camera.addCallbackBuffer(frame);

	}
	
	


	public int getWidth() {
		return (m_width);
	}

	public int getHeight() {
		return (m_height);
	}

	
	

	
	public static int getCameraOrientation(int cameraId) {
		android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
		android.hardware.Camera.getCameraInfo(cameraId, info);

		return info.orientation;
	}
	
	
	public void Pause() {
//		m_cameraPreview.onPause();
	}
	
	public void Resume() {
//		m_cameraPreview.onResume();
	}

	public void Close() {
		
		if (m_cameraPreview != null) {
//			m_cameraPreview.onStop();
		}

		m_cameraPreview = null;
	}
}
