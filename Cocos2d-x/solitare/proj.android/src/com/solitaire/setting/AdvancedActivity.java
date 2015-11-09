package com.solitaire.setting;

import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ToggleButton;

import com.leejyhung.RealSolitaire.R;
import com.solitaire.global.GlobalContants;

public class AdvancedActivity extends Activity implements OnClickListener {

	private ToggleButton toggleRightHandMode;
	private ToggleButton toggleAutoComplete;
	private ToggleButton toggleGameSolver;
	private ToggleButton toggleSummary;
	
	private int m_nAdvanced;
	private boolean bSummary;
	
	private final int kBitRightHandMode = 0;
	private final int kBitAutoComplete = 1;
	private final int kBitGameSolver = 2;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.advanced);
		
//		getWindow().setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
//		getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
		
		findViewById(R.id.btn_done).setOnClickListener(this);
		
		toggleRightHandMode = (ToggleButton)findViewById(R.id.toggle_righthand);
		toggleRightHandMode.setOnClickListener(this);
		
		toggleAutoComplete = (ToggleButton)findViewById(R.id.toggle_autocomplete);
		toggleAutoComplete.setOnClickListener(this);
		
		toggleGameSolver = (ToggleButton)findViewById(R.id.toggle_gamesolver);
		toggleGameSolver.setOnClickListener(this);
		
		toggleSummary = (ToggleButton)findViewById(R.id.toggle_summary);
		toggleSummary.setOnClickListener(this);
		
		initSettings();
	}

	private void initSettings()
	{
		m_nAdvanced = getIntent().getIntExtra(GlobalContants.kAdvanced, 0);
		if( (m_nAdvanced & (1<<kBitRightHandMode)) == (1<<kBitRightHandMode) )
			toggleRightHandMode.setChecked(true);
		else
			toggleRightHandMode.setChecked(false);
		
		if( (m_nAdvanced & (1<<kBitAutoComplete)) == (1<<kBitAutoComplete) )
			toggleAutoComplete.setChecked(true);
		else
			toggleAutoComplete.setChecked(false);
		
		if( (m_nAdvanced & (1<<kBitGameSolver)) == (1<<kBitGameSolver) )
			toggleGameSolver.setChecked(true);
		else
			toggleGameSolver.setChecked(false);
		
		//end of game summary
		SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
		final boolean bShowSummary = preferences.getBoolean(GlobalContants.kSettingSummary, false);
		bSummary = bShowSummary;
		if( bShowSummary )
			toggleSummary.setChecked(true);
		else
			toggleSummary.setChecked(false);
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		if( v.getId() == R.id.btn_done )
		{
			//save summary setting
			SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
			SharedPreferences.Editor editor = preferences.edit();
			editor.putBoolean(GlobalContants.kSettingSummary, bSummary);
			editor.apply();
			
			Intent intent = new Intent();
			intent.putExtra(GlobalContants.kAdvanced , m_nAdvanced);
			setResult(RESULT_OK, intent);
			
			System.out.println(" ----------   " + m_nAdvanced + "   ---------\n");
			
			finish();
		}
		else if( v.getId() == R.id.toggle_summary )
		{
			bSummary = toggleSummary.isChecked();	
		}
		else if( v.getId() == R.id.toggle_righthand )
		{
			if( toggleRightHandMode.isChecked() )
				m_nAdvanced |= (1<<kBitRightHandMode);
			else
				m_nAdvanced ^= (1<<kBitRightHandMode);
			
		}
		else if( v.getId() == R.id.toggle_autocomplete )
		{
			if( toggleAutoComplete.isChecked() )
				m_nAdvanced |= (1<<kBitAutoComplete);
			else
				m_nAdvanced ^= (1<<kBitAutoComplete);
		}
		else if( v.getId() == R.id.toggle_gamesolver) 
		{
			if( toggleGameSolver.isChecked() )
				m_nAdvanced |= (1<<kBitGameSolver);
			else
				m_nAdvanced ^= (1<<kBitGameSolver);
		}
	}	
}
