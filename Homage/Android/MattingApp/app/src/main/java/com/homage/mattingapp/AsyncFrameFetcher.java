package com.homage.mattingapp;

import android.os.AsyncTask;

import com.homage.matting.Matting;


public class AsyncFrameFetcher extends AsyncTask <Integer, FrameData, String>
{
	int m_width = 480;
	int m_height = 480;
	int m_taskId = 1;
	
	MainActivity m_activity = null;
	
	
	
	public AsyncFrameFetcher( MainActivity activity, int width, int height )
	{
		m_activity = activity;
		m_width = width;
		m_height = height;
	}
	
	
	@Override
	protected String doInBackground( Integer... loopParams )
	{
		boolean running = true;
		
		int iFrame = 0;
		FrameData fd = new FrameData( m_width, m_height );
		
		while(running){

			fd.m_state = Matting.getUpdate( fd.m_im.m_data, fd.m_aim.m_data );
			fd.m_iFrame = iFrame++;
			
			publishProgress( fd );
			
			//SystemClock.sleep(loopParams[0]);
			
			if(isCancelled())
				running = false;
		}
		
		return("We are done!");
    	}
    	
    	
    	@Override
        protected void onProgressUpdate(FrameData... fd ) {
    		m_activity.updateData( fd[0] );
        }



    	
    	@Override
    	protected void onPostExecute(String result)
    	{
 //   		Toast.makeText(AsyncTaskActivity.this, result, Toast.LENGTH_SHORT).show();
	}
	
	
	@Override
	protected void onCancelled() {
//    		Toast.makeText(AsyncTaskActivity.this, "cancelled", Toast.LENGTH_SHORT).show();
	}
}

