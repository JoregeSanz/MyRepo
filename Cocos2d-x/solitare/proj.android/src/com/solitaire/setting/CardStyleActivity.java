/**
 * 
 */
package com.solitaire.setting;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ImageView;

import com.leejyhung.RealSolitaire.R;
import com.solitaire.global.GlobalContants;

/**
 * @author LightSky
 * @since  2015/02/21	
 */
public class CardStyleActivity extends Activity implements OnClickListener {
	
	private ImageView check_face_standard;
	private ImageView check_face_classic;
	
	private ImageView check_back_standard;
	private ImageView check_back_classic;
	
	private int m_nCardStyle;
	
	private final int nBitBackClassic = 3;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.cardstyle);
		
//		getWindow().setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
//		getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
		
		m_nCardStyle = getIntent().getIntExtra(GlobalContants.kCardStyleParam, 0);
		
		findViewById(R.id.btn_done).setOnClickListener(this);
		
		findViewById(R.id.row_face_standard).setOnClickListener(this);
		check_face_standard = (ImageView)findViewById(R.id.check_face_standard);
		
		findViewById(R.id.row_face_classic).setOnClickListener(this);
		check_face_classic = (ImageView)findViewById(R.id.check_face_classic);
		
		findViewById(R.id.row_back_standard).setOnClickListener(this);
		check_back_standard = (ImageView)findViewById(R.id.check_back_standard);
		
		findViewById(R.id.row_back_classic).setOnClickListener(this);
		check_back_classic = (ImageView)findViewById(R.id.check_back_classic);
		
		setSettings();
	}

	private void setSettings()
	{
		if((m_nCardStyle & (1<<GlobalContants.nBitFaceClassic)) == (1<<GlobalContants.nBitFaceClassic))
		{
			check_face_standard.setVisibility(View.INVISIBLE);
			check_face_classic.setVisibility(View.VISIBLE);
		}
		else
		{
			check_face_standard.setVisibility(View.VISIBLE);
			check_face_classic.setVisibility(View.INVISIBLE);
		}
		
		if((m_nCardStyle & (1<<nBitBackClassic)) == (1<<nBitBackClassic))
		{
			check_back_standard.setVisibility(View.INVISIBLE);
			check_back_classic.setVisibility(View.VISIBLE);
		}
		else
		{
			check_back_standard.setVisibility(View.VISIBLE);
			check_back_classic.setVisibility(View.INVISIBLE);
		}
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if( v.getId() == R.id.btn_done )
		{
			Intent intent = new Intent();
			intent.putExtra(GlobalContants.kCardStyleParam, m_nCardStyle);
			setResult(RESULT_OK, intent);
			
			finish();
		}
		else if( v.getId() == R.id.row_face_standard )
		{
			if( (m_nCardStyle & (1<<GlobalContants.nBitFaceClassic)) == (1<<GlobalContants.nBitFaceClassic))	//Now classic is selected
			{
				m_nCardStyle ^= (1<<GlobalContants.nBitFaceClassic);
				
				check_face_standard.setVisibility(View.VISIBLE);
				check_face_classic.setVisibility(View.INVISIBLE);
			}
		}
		else if( v.getId() == R.id.row_face_classic )
		{
			if( (m_nCardStyle & (1<<GlobalContants.nBitFaceClassic)) != (1<<GlobalContants.nBitFaceClassic))	//Now standard is selected
			{
				m_nCardStyle |= (1<<GlobalContants.nBitFaceClassic);
				
				check_face_standard.setVisibility(View.INVISIBLE);
				check_face_classic.setVisibility(View.VISIBLE);
			}
		}
		else if( v.getId() == R.id.row_back_standard )
		{
			if( (m_nCardStyle & (1<<nBitBackClassic)) == (1<<nBitBackClassic))	//Now classic blue is selected
			{
				m_nCardStyle ^= (1<<nBitBackClassic);
				
				check_back_standard.setVisibility(View.VISIBLE);
				check_back_classic.setVisibility(View.INVISIBLE);
			}
		}
		else if( v.getId() == R.id.row_back_classic )
		{
			if( (m_nCardStyle & (1<<nBitBackClassic)) != (1<<nBitBackClassic))	//Now standard blue is selected
			{
				m_nCardStyle |= (1<<nBitBackClassic);
				
				check_back_standard.setVisibility(View.INVISIBLE);
				check_back_classic.setVisibility(View.VISIBLE);
			}
		}
	}

}
