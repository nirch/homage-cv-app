package com.vim.vimapi;


import java.io.IOException;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.mime.HttpMultipartMode;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.ByteArrayBody;
import org.apache.http.impl.client.DefaultHttpClient;

public class vHttp {

	public static void  post( byte[] data, String url )
	{
		final byte[] image = data;
		final String path = url;
		final int aa = 1;

		new Thread(new Runnable() {
			public void run() {
			    HttpClient httpclient = new DefaultHttpClient();
			    HttpPost httppost = new HttpPost(path);
			    int aa = 2;

			    try {
			    	MultipartEntity entity = new MultipartEntity(HttpMultipartMode.BROWSER_COMPATIBLE);
			    	entity.addPart("image", new ByteArrayBody(image,"tempfile"));
			        httppost.setEntity(entity);

			        // Execute HTTP Post Request
			        HttpResponse response = httpclient.execute(httppost);

			        aa = 6*7;


			    } catch (ClientProtocolException e) {
			    	e.printStackTrace();
			    } catch (IOException e) {
			    	e.printStackTrace();
			    }
			}
		}).start();
	}
}
