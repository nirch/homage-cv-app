package com.homage.mattingapp;

import android.graphics.Bitmap;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import com.vim.vimapi.Camera.vCamera;
import com.vim.vimapi.vTool;

import com.homage.matting.Matting;
import com.homage.renderer.Renderer;



public class MainActivity extends ActionBarActivity implements View.OnClickListener  {

    private vCameraAcquisition m_camera;
    private AsyncFrameFetcher  m_asyncFrameFetcher;
    private ImageView m_mainView;



    private Bitmap m_bmp;

    static Button recordButton;

    vVideoEncodder videoEncoder;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        m_mainView = (ImageView) findViewById(R.id.imageViewCamera);


        recordButton = (Button)findViewById(R.id.recordButton);
        recordButton.setOnClickListener(this);

    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


    public void onStart()
    {

        init();

        super.onStart();
    }

    @Override
    public void onClick(View view) {

        switch (view.getId()){
            case R.id.recordButton:
                Matting.setOperation(2);
                break;
        }

    }


    public void updateData(FrameData fd){
        m_bmp = fd.m_im.toBitmap( m_bmp );
        m_bmp.setHasAlpha(true);


        //  m_mainView.setBackgroundColor(getResources().getColor(android.R.color.black));
        m_mainView.setImageBitmap(m_bmp);

    }


    public void init()
    {
        /// 	HiPark  hipark = new HiPark();
        Matting.openLog( vTool.getExtrnalPath("Notes", "Matting.log") );


        rendererTest();

        m_camera = new vCameraAcquisition();//this, preview);


        int cameraOrientation = m_camera.getCameraOrientation( vCamera.FRONT_CAMERA );
        m_camera.open(  vCamera.FRONT_CAMERA, vCamera.RESOLUTION_MED );

        Matting.initCamera( cameraOrientation );


        String prmFile = vTool.getExtrnalPath( "emu", "emu.xml" );
        String sideFile = vTool.getExtrnalPath( "emu", "emu.ctr" );
        int ret = Matting.init( prmFile, sideFile, 480, 480 );

        // Parking.setDumpDir( vTool.getSavePath( "Notes") );


        m_asyncFrameFetcher = new AsyncFrameFetcher( this,  m_camera.getHeight(), m_camera.getWidth() );
        m_asyncFrameFetcher.execute( 10 );

        Matting.start();


 //       Matting.setOperation(2);
    }


    public void rendererTest() {

        Renderer rd = new Renderer();


        int ret = rd.create();

        int bg_source_id = Renderer.addSource(1, vTool.getExtrnalPath("emu", "1/bg.gif") );

        videoEncoder = new vVideoEncodder();
        videoEncoder.close();

       // Object bb = aa;

        ret = rd.addJavaOutput( videoEncoder, "bb.txt", 240, 240, 10 );

        Renderer.process();
    }
}
