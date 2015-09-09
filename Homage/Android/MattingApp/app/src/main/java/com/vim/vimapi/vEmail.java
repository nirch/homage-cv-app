package com.vim.vimapi;

import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

public class vEmail {
	private Context context;
	
	public vEmail(Context c) {
		context = c;
	}
	
	public boolean send(String data, String to) {
		Intent i = new Intent(Intent.ACTION_SEND);
		i.setType("message/rfc822");
		i.putExtra(Intent.EXTRA_EMAIL  , new String[]{to});
		i.putExtra(Intent.EXTRA_SUBJECT, "MyChic - VirtualMirror");
		i.putExtra(Intent.EXTRA_TEXT   , data);
		try {
		    context.startActivity(Intent.createChooser(i, "Send mail..."));
		} catch (android.content.ActivityNotFoundException ex) {
		    Toast.makeText(context, "There are no email clients installed.", Toast.LENGTH_SHORT).show();
		}
		
		return true;
	}
}
