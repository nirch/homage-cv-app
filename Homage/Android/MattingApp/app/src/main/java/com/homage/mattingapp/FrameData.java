package com.homage.mattingapp;


import com.vim.vimapi.ImageData;

public class FrameData
{
	int	m_iFrame;
	
	int m_width;
	int m_height;
	
	ImageData m_im;
    ImageData m_aim;

	int[] m_frameLabelState;
	
	int m_state;
	
	protected FrameData( int width, int height )
	{
		super();
		
		m_width = width;
		m_height = height;
		
		m_im = new ImageData( width, height );
        m_aim = new ImageData( width/2, height/2 );

        m_frameLabelState = new int[16];
		m_state = 0;
	}
		
		
	protected FrameData( ImageData im, int[] frameLabelState, int state )
	{
		super();
		m_im = im;
		m_frameLabelState = frameLabelState;
		m_state = state;
	}
	

	public int getFrame() {
		return m_iFrame;
	}
	
	public int getState() {
		return m_state;
	}
	
	
	public ImageData getImage()
	{
		return( m_im );
	}
	
	
	public int[] getFrameLabelState()
	{
		return( m_frameLabelState );
	}
	

	

}