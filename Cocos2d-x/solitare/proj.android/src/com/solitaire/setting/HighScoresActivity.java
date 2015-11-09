package com.solitaire.setting;

import java.util.Arrays;
import java.util.Comparator;

import android.app.Activity;
import android.app.Dialog;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.leejyhung.RealSolitaire.R;
import com.solitaire.global.GlobalContants;

public class HighScoresActivity extends Activity implements OnClickListener {

	private ListView standardListView;
	private ListView vegasListView;
	
	private ScoreListAdapter standardAdapter;
	private ScoreListAdapter vegasAdapter;
	
	private RelativeLayout rowNoScores;
	private RelativeLayout rowNoSession;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.high_scores);
		
//		getWindow().setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
//		getWindow().setBackgroundDrawable(new ColorDrawable(color.transparent));
		
		standardAdapter = null;
		vegasAdapter = null;
		
		findViewById(R.id.btn_done).setOnClickListener(this);
		findViewById(R.id.tab_standard).setOnClickListener(this);
		findViewById(R.id.tab_vegas).setOnClickListener(this);
		findViewById(R.id.row_reset).setOnClickListener(this);
		
		standardListView = (ListView) findViewById(R.id.list_scores);
		vegasListView = (ListView) findViewById(R.id.list_sessions);
		
		rowNoScores = (RelativeLayout)findViewById(R.id.row_noscores);
		rowNoSession = (RelativeLayout)findViewById(R.id.row_nosession);
		
		setValues();
	}

	private void setValues()
	{
		final SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
		
		//////////////	  standard		//////////////
		//played
		int nPlayed = preferences.getInt(GlobalContants.kStandardPlayed, 0);
		((TextView)findViewById(R.id.txt_standard_gamesplayed)).setText(String.valueOf(nPlayed));
		
		String standardResults = preferences.getString(GlobalContants.kStandardResults, "NORESULT");
		int cnt = 0;
		
		if( !standardResults.equals("NORESULT") )
		{
			rowNoScores.setVisibility(View.GONE);
			
			String results[] = standardResults.split("#");
			cnt = results.length;
			
			GameResult standardScoreResults[] = new GameResult[cnt];
			for( int i = 0; i < cnt; i ++ )
				standardScoreResults[i] = new GameResult(results[i]);
			
			Arrays.sort( standardScoreResults, new Comparator<GameResult>(){
		        public int compare( GameResult a, GameResult b ){
		            return b.m_nScore - a.m_nScore;
		        }
		    });
			
			standardListView.setVisibility(View.VISIBLE);
			
			standardAdapter = new ScoreListAdapter(this, standardScoreResults);
			standardAdapter.setVegasMode(false);
			standardListView.setAdapter(standardAdapter);
		}
		
		//games won
		int percent = 0;
		if( nPlayed > 0 )
			percent = Math.round(cnt * 100.0f / nPlayed);
		((TextView)findViewById(R.id.txt_standard_gameswon)).setText(String.valueOf(cnt) + "(" + String.valueOf(percent) + "%)");
		
		//winning streak
		final int nWinningStreak = preferences.getInt(GlobalContants.kWinningStreak, 0);
		((TextView)findViewById(R.id.txt_winningstreak)).setText(String.valueOf(nWinningStreak));
		
		//losing streak
		final int nLosingStreak = preferences.getInt(GlobalContants.kLosinStreak, 0);
		((TextView)findViewById(R.id.txt_losingstreak)).setText(String.valueOf(nLosingStreak));
		
		//current streak
		final String sCurStreak = preferences.getString(GlobalContants.kCurrentStreak, "");
		((TextView)findViewById(R.id.txt_currentstreak)).setText(sCurStreak);
		
		//////////////		vegas		///////////////
		
		//played 
		nPlayed = preferences.getInt(GlobalContants.kVegasPlayed, 0);
		((TextView)findViewById(R.id.txt_gamesplayed)).setText(String.valueOf(nPlayed));
		
		//win
		int nWon = preferences.getInt(GlobalContants.kVegasWon, 0);
		percent = 0;
		
		int nMostWon = 0;
		int nMostLost = 0;
		int nCurWinnings = 0;
		
		if( nPlayed > 0 )
		{
			percent = Math.round(nWon * 100.0f / nPlayed);
			
			String vegasResults = preferences.getString(GlobalContants.kVegasResults, "NORESULT");
			
			if( !vegasResults.equals("NORESULT") )
			{
				rowNoSession.setVisibility(View.GONE);
				
				String results[] = vegasResults.split("#");
				cnt = results.length;
				
				GameResult vegasScoreResults[] = new GameResult[cnt];
				for( int i = 0; i < cnt; i ++ )
					vegasScoreResults[i] = new GameResult(results[i]);
				
				Arrays.sort( vegasScoreResults, new Comparator<GameResult>(){
			        public int compare( GameResult a, GameResult b ){
			            return b.m_nScore - a.m_nScore;
			        }
			    });
				
				for( int i = 0;i < cnt; i ++ )
				{
					if( i == 0 && vegasScoreResults[i].m_nScore > 0 )
						nMostWon = vegasScoreResults[i].m_nScore;
					
					if( i == cnt - 1 && vegasScoreResults[i].m_nScore < 0 ) 
						nMostLost = Math.abs(vegasScoreResults[i].m_nScore);
					
					nCurWinnings += vegasScoreResults[i].m_nScore;
				}
				
				vegasListView.setVisibility(View.VISIBLE);
				
				vegasAdapter = new ScoreListAdapter(this, vegasScoreResults);
				vegasAdapter.setVegasMode(true);
				
				vegasListView.setAdapter(vegasAdapter);
			}
		}
		
		((TextView)findViewById(R.id.txt_gameswon)).setText(String.valueOf(nWon) + "(" + String.valueOf(percent) + "%)");
		
		//most money won
		((TextView)findViewById(R.id.txt_moneywon)).setText("$" + String.valueOf(nMostWon));
		//most money lost
		((TextView)findViewById(R.id.txt_moneylost)).setText("$" + String.valueOf(nMostLost));
		//current winnings
		String sCurWinnings = "$" + String.valueOf(nCurWinnings);
		if( nCurWinnings < 0 ) 
			sCurWinnings = "-$" + String.valueOf(Math.abs(nCurWinnings));
		
		((TextView)findViewById(R.id.txt_winnings)).setText(sCurWinnings);
	}
	
	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		if( v.getId() == R.id.btn_done )
		{
			finish();
		}
		else if( v.getId() == R.id.tab_standard )
		{
			findViewById(R.id.view_standard).setVisibility(View.VISIBLE);
			findViewById(R.id.view_vegas).setVisibility(View.GONE);
		}
		else if( v.getId() == R.id.tab_vegas )
		{
			findViewById(R.id.view_standard).setVisibility(View.GONE);
			findViewById(R.id.view_vegas).setVisibility(View.VISIBLE);
		}
		else if( v.getId() == R.id.row_reset )
		{

			final Dialog dialog = new Dialog(this);
			dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
			dialog.setContentView(R.layout.reset_scores);

			Button btnReset = (Button) dialog
					.findViewById(R.id.btn_reset);
			btnReset.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					resetScores();
					
					dialog.dismiss();
				}

			});

			Button btnCancel = (Button) dialog
					.findViewById(R.id.btn_cancel);
			btnCancel.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					dialog.dismiss();
				}

			});

			dialog.show();
		}
	}
	
	private void resetScores()
	{
		SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
		preferences.edit().remove(GlobalContants.kStandardPlayed).commit();
		preferences.edit().remove(GlobalContants.kWinningStreak).commit();
		preferences.edit().remove(GlobalContants.kLosinStreak).commit();
		preferences.edit().remove(GlobalContants.kCurrentStreak).commit();
		preferences.edit().remove(GlobalContants.kStandardResults).commit();
		
		preferences.edit().remove(GlobalContants.kVegasPlayed).commit();
		preferences.edit().remove(GlobalContants.kVegasWon).commit();
		preferences.edit().remove(GlobalContants.kVegasResults).commit();
		
		setValues();
		
		standardListView.setVisibility(View.GONE);
		rowNoScores.setVisibility(View.VISIBLE);
		
		vegasListView.setVisibility(View.GONE);
		rowNoSession.setVisibility(View.VISIBLE);
		
	}
}
