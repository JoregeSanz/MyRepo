/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.view.Display;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.leejyhung.RealSolitaire.R;
import com.solitaire.global.GlobalContants;
import com.solitaire.setting.GameResult;
import com.solitaire.setting.HighScoresActivity;
import com.solitaire.setting.SettingsActivity;

@SuppressLint("NewApi") 
public class AppActivity extends Cocos2dxActivity {

	private static AppActivity self;

	// native method
	public native void onQuitAndStartGame();

	public native void onRestartGame();

	public native void onSettings(int nGeneral, int nWallpaper, int nCardStyle, int nAdvanced, int nNewGameStart);

	public native void onChangedOrientation(boolean isPortrait, float w, float h);
	
	public native void saveState();

	// setting information
	private String m_sFilepath;
	private int m_nGeneral;
	private int m_nWallpaper;
	private int m_nCardStyle;
	private int m_nAdvanced;

	private final int GET_SETTINGS = 100;

	//google admob
	private AdView adView;
	private static final String AD_UNIT_ID = "ca-app-pub-0000000000000000/0000000000";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);

		//google admob
		
		//getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		Display display = getWindowManager().getDefaultDisplay();
		int width = display.getWidth(); 


		LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
		width,
		LinearLayout.LayoutParams.WRAP_CONTENT);


		adView = new AdView(this);
		adView.setAdSize(AdSize.BANNER);
		adView.setAdUnitId(AD_UNIT_ID);


		AdRequest adRequest = new AdRequest.Builder()
		.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
		.addTestDevice("0BF875477082BD8F9B8A0AF8B272011D")
		.build();

		adView.loadAd(adRequest);
        adView.setBackgroundColor(Color.BLACK);
        adView.setBackgroundColor(0);
//        addContentView(adView,adParams);
        
        RelativeLayout relativeLayout=new RelativeLayout(this);
        RelativeLayout.LayoutParams layoutParams1=new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
        RelativeLayout.LayoutParams layoutParams=new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, RelativeLayout.TRUE);
        layoutParams.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);
        //layoutParams.bottomMargin = 150; 	//portrait
        layoutParams.bottomMargin = 80; 	//landscape
        
        relativeLayout.addView(adView, layoutParams);
        this.addContentView(relativeLayout, layoutParams1);

        self = this;
//	    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD) {
//	    	self.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
//	    }
        
        checkTablet();
	}

	private void checkTablet()
	{
		boolean isTablet = self.isTablet();
		if( isTablet == false ) 
		{
			new AlertDialog.Builder(self)
			.setTitle("Alert")
			.setMessage(
					"This game is for tablet. Please try again in tablet.")
			.setPositiveButton(android.R.string.ok,
					new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog,
								int which) {
							// continue
							self.finish();
						}
					})
			.setIcon(android.R.drawable.ic_dialog_alert)
			.show();
		}
	}
	
	@Override
	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		//glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		// glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);

		return glSurfaceView;
	}
	
	public boolean isTablet() {
	    return (self.getResources().getConfiguration().screenLayout
	            & Configuration.SCREENLAYOUT_SIZE_MASK)
	            >= Configuration.SCREENLAYOUT_SIZE_LARGE;
	}

	/**
	 * unused
	 * @return
	 */
	public static AppActivity getInstance()
	{
		return self;
	}
	
	/**
	 * unused
	 * @param nWallpaper
	 */
	public void setWallpaper(int nWallpaper)
	{	
		self.m_nWallpaper = nWallpaper;
		self.onSettings(self.m_nGeneral, self.m_nWallpaper, self.m_nCardStyle, self.m_nAdvanced, 0);
	}
	
	public static void showNewGameDialog(final String info, final String result) {
		
		boolean isSummary = true;
		if( info.equals("") )
			isSummary = false;
		
		SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(self);
		boolean bSummarySetting = preferences.getBoolean(GlobalContants.kSettingSummary, false);
		if( bSummarySetting == false )
			isSummary = false;
		
		final boolean bSummary = isSummary;

		self.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				final Dialog dialog = new Dialog(self);
				dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
				dialog.setContentView(R.layout.restartgame);

				Button btnNewGame = (Button) dialog
						.findViewById(R.id.btn_newgame);
				btnNewGame.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						if( bSummary )
							self.showGameOverSummaryDialog(info, true);
						else
							self.onQuitAndStartGame();
						
						self.setGameResult(result);
						
						dialog.dismiss();
					}

				});

				Button btnRestart = (Button) dialog
						.findViewById(R.id.btn_restart);
				btnRestart.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						if( bSummary )
							self.showGameOverSummaryDialog(info, false);
						else
							self.onRestartGame();
						
						self.setGameResult(result);
						
						dialog.dismiss();
					}

				});

				Button btnKeep = (Button) dialog.findViewById(R.id.btn_keep);
				btnKeep.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						dialog.dismiss();
					}

				});

				dialog.show();
			}

		});
	}

	private void setGameResult(final String result)
	{
		//format : scoresetting_drawmode_timed_won_score
		String params[] = result.split("_");
		final String sScoreSetting = params[0];
		final String sDrawMode = params[1];
		final String sTimed = params[2];
		final String sWon = params[3];
		final String sScore = params[4];
		
		final SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(self);
		SharedPreferences.Editor editor = preferences.edit();
		
		int nScoreSetting = Integer.parseInt(sScoreSetting);
		if( nScoreSetting == 0 ) //none
			return;
		else if( nScoreSetting == 1 )	//standard
		{
			String sCurStreak;
			
			int nGamePlayed = preferences.getInt(GlobalContants.kStandardPlayed, 0);
			nGamePlayed ++;
			
			int nLosingStreak = preferences.getInt(GlobalContants.kLosinStreak, 0);
			nLosingStreak ++;
			
			if( sWon.equals("1") )	//When won, save to won list
			{
				GameResult gameResult = new GameResult(preferences.getString(GlobalContants.kName, "User"), sDrawMode, sTimed, sScore);
				
				//format : result#result
				String results = preferences.getString(GlobalContants.kStandardResults, "NORESULT");
				if( results.equals("NORESULT") )
				{
					results = gameResult.getResultString();
				}
				else
					results = results + "#" + gameResult.getResultString();
				
				editor.putString(GlobalContants.kStandardResults, results);
				
				sCurStreak = "1 won";
			}
			else
			{
				sCurStreak = String.valueOf(nLosingStreak) + " losses";
			}
			
			//save result
			editor.putInt(GlobalContants.kStandardPlayed, nGamePlayed);
			editor.putInt(GlobalContants.kLosinStreak, nLosingStreak);
			editor.putString(GlobalContants.kCurrentStreak, sCurStreak);
		}
		else	//vegas
		{
			int nVegasPlayed = preferences.getInt(GlobalContants.kVegasPlayed, 0);
			nVegasPlayed ++;
			
			if( sWon.equals("1") )
			{
				int nVegasWon = preferences.getInt(GlobalContants.kVegasWon, 0);
				nVegasWon ++;
				
				editor.putInt(GlobalContants.kVegasWon, nVegasWon);
			}
			
			editor.putInt(GlobalContants.kVegasPlayed, nVegasPlayed);
			
			GameResult gameResult = new GameResult(preferences.getString(GlobalContants.kName, "User"), sDrawMode, sTimed, sScore);
			
			//format : result#result
			String results = preferences.getString(GlobalContants.kVegasResults, "NORESULT");
				
			if( results.equals("NORESULT") )
			{
				results = gameResult.getResultString();
			}
			else
				results = results + "#" + gameResult.getResultString();
						
			editor.putString(GlobalContants.kVegasResults, results);
		}
		
		editor.apply();
	}
	
	private void showGameOverSummaryDialog(final String info, final boolean isNewGame) {
		
		String params[] = info.split("_");
		final String sMode = params[0];
		final String sScore = params[1];
		final String sMoves = params[2];
		final String sTime = params[3];

		self.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub

				final Dialog dialog = new Dialog(self);
				dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
				dialog.setContentView(R.layout.gameover_summary);
		
				final SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(self);
				  String name = preferences.getString(GlobalContants.kName,"");
				  if(name.equals(""))
				  {
				    name = name+"your name";  /* Edit the value here*/
				  }
				
				final EditText txtName = (EditText)dialog.findViewById(R.id.edit_name);
				txtName.setText(name);
				
				TextView txtMode = (TextView)dialog.findViewById(R.id.txt_mode);
				txtMode.setText(sMode);
				
				String score = sScore;
				if( sMode.startsWith("Vegas") )
				{
					int nScore = Integer.parseInt(score);
					if( nScore >= 0 )
						score = "$" + score;
					else
						score = "-$" + String.valueOf(Math.abs(nScore));
				}
				
				TextView txtScore = (TextView)dialog.findViewById(R.id.txt_score);
				txtScore.setText(score);
				
				TextView txtMoves = (TextView)dialog.findViewById(R.id.txt_moves);
				txtMoves.setText(sMoves);
				
				TextView txtTime = (TextView)dialog.findViewById(R.id.txt_time);
				if( sTime.equals("-1") )
				{
					LinearLayout lineTime = (LinearLayout)dialog.findViewById(R.id.line_time);
					RelativeLayout rowTime = (RelativeLayout)dialog.findViewById(R.id.row_time);
					
					lineTime.setVisibility(View.GONE);
					rowTime.setVisibility(View.GONE);
				}
				else
					txtTime.setText(self.getFormattedTime(sTime));
				
				Button btnDone = (Button) dialog
						.findViewById(R.id.btn_done);
				btnDone.setOnClickListener(new OnClickListener() {

					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						
						SharedPreferences.Editor editor = preferences.edit();
						editor.putString(GlobalContants.kName, txtName.getText().toString());
						editor.apply();

						
						if( isNewGame )
							self.onQuitAndStartGame();
						else
							self.onRestartGame();
						
						dialog.dismiss();
					}

				});

				dialog.show();
			}

		});
	}
	
	public static void showGameWonDialog(final String info, final String result) {
		
		self.setGameResult(result);
		
		boolean isSummary = true;
		if( info.equals("") )
			isSummary = false;
		
		SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(self);
		boolean bSummarySetting = preferences.getBoolean(GlobalContants.kSettingSummary, false);
		if( bSummarySetting == false )
			isSummary = false;
		
		final boolean bSummary = isSummary;
		if( bSummary )
		{
			String params[] = info.split("_");
			final String sMode = params[0];
			final String sScore = params[1];
			final String sMoves = params[2];
			final String sTime = params[3];

			self.runOnUiThread(new Runnable() {

				@Override
				public void run() {
					// TODO Auto-generated method stub

					final Dialog dialog = new Dialog(self);
					dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
					dialog.setContentView(R.layout.gameover_summary);
			
					TextView txtTitle = (TextView) dialog.findViewById(R.id.txt_title);
					txtTitle.setText(R.string.you_won);
					
					ImageView imgBanner = (ImageView)dialog.findViewById(R.id.img_banner);
					imgBanner.setImageResource(R.drawable.vegas_won_header);
					
					final SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(self);
					  String name = preferences.getString(GlobalContants.kName,"");
					  if(name.equals(""))
					  {
					    name = name+"your name";  /* Edit the value here*/
					  }
					
					final EditText txtName = (EditText)dialog.findViewById(R.id.edit_name);
					txtName.setText(name);
					
					TextView txtMode = (TextView)dialog.findViewById(R.id.txt_mode);
					txtMode.setText(sMode);
					
					String score = sScore;
					if( sMode.startsWith("Vegas") )
					{
						int nScore = Integer.parseInt(score);
						if( nScore >= 0 )
							score = "$" + score;
						else
							score = "-$" + String.valueOf(Math.abs(nScore));
					}
					
					TextView txtScore = (TextView)dialog.findViewById(R.id.txt_score);
					txtScore.setText(score);
					
					TextView txtMoves = (TextView)dialog.findViewById(R.id.txt_moves);
					txtMoves.setText(sMoves);
					
					TextView txtTime = (TextView)dialog.findViewById(R.id.txt_time);
					if( sTime.equals("-1") )
					{
						LinearLayout lineTime = (LinearLayout)dialog.findViewById(R.id.line_time);
						RelativeLayout rowTime = (RelativeLayout)dialog.findViewById(R.id.row_time);
						
						lineTime.setVisibility(View.GONE);
						rowTime.setVisibility(View.GONE);
					}
					else
						txtTime.setText(self.getFormattedTime(sTime));
					
					Button btnDone = (Button) dialog
							.findViewById(R.id.btn_done);
					btnDone.setOnClickListener(new OnClickListener() {

						@Override
						public void onClick(View v) {
							// TODO Auto-generated method stub
							SharedPreferences.Editor editor = preferences.edit();
							editor.putString(GlobalContants.kName, txtName.getText().toString());
							editor.apply();
							
							self.onQuitAndStartGame();
							dialog.dismiss();
						}

					});

					dialog.show();
				}
			});
		}
	}
	
	private String getFormattedTime(final String sTime)
	{
		String time = sTime;
		int nTime = Integer.parseInt(sTime);
		if( nTime >= 60 )
		{
			int nMin = nTime / 60;
			int nSec = nTime % 60;
			
			time = sTime + "(" + String.valueOf(nMin) + "m " + String.valueOf(nSec) + "s)";
		}
		
		return time;
	}
	
	public static void showNoWinnableMessage()
	{
		self.runOnUiThread(new Runnable()
	     {
		     @Override
		     public void run() {
				new AlertDialog.Builder(self)
				.setTitle("Game Solver")
				.setMessage(
						"This game is no longer winnable. Please try again.")
				.setPositiveButton(android.R.string.yes,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {
								// continue
								
							}
						})
				.setIcon(android.R.drawable.ic_dialog_alert)
				.show();
		     }
	     });
	}
	
	public static void showSettingActivity(String filepath, int nGeneral, int nWallpaper,
			int nCardStyle, int nAdvanced) {
		System.out.println(" ## showSettingActivity ##\n " + filepath + " : " + nGeneral + ", "
				+ nWallpaper + ", " + nCardStyle + ", " + nAdvanced);

		self.m_sFilepath = filepath;
		self.m_nGeneral = nGeneral;
		self.m_nWallpaper = nWallpaper;
		self.m_nCardStyle = nCardStyle;
		self.m_nAdvanced = nAdvanced;

		Intent intent = new Intent(self, SettingsActivity.class);
		intent.putExtra(GlobalContants.kCustomFilePath, filepath);
		intent.putExtra("nGeneral", nGeneral);
		intent.putExtra("nWallpaper", nWallpaper);
		intent.putExtra("nCardStyle", nCardStyle);
		intent.putExtra(GlobalContants.kAdvanced, nAdvanced);

		self.startActivityForResult(intent, self.GET_SETTINGS);
	}

	public static void showHighScoresView()
	{
		Intent intent = new Intent(self, HighScoresActivity.class);
		self.startActivity(intent);
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);

		if (requestCode == self.GET_SETTINGS && resultCode == RESULT_OK) {
			final int nGeneral = data.getIntExtra("nGeneral", 0);
			final int nWallpaper = data.getIntExtra("nWallpaper", 3);
			final boolean bIsCustomed = data.getBooleanExtra(GlobalContants.kIsCustomed, false);
			final int nCardStyle = data.getIntExtra("nCardStyle", 0);
			final int nAdvanced = data.getIntExtra(GlobalContants.kAdvanced, 0);

			if (self.m_nGeneral == nGeneral && self.m_nWallpaper == nWallpaper
					&& self.m_nCardStyle == nCardStyle && bIsCustomed == false && m_nAdvanced == nAdvanced )
				return;

			int nLim = 7;
			int nNewGameStart = 0;
			for (int i = 0; i < nLim; i++) {
				if (((self.m_nGeneral & (1 << i)) != (nGeneral & (1 << i)))) // is
																				// changed?
				{
					nNewGameStart = 1;
					break;
				}
			}

			if (nNewGameStart == 1) {
				new AlertDialog.Builder(self)
						.setTitle("Game Settings Changed")
						.setMessage(
								"The settings won't apply to the game in progress. Would you like to start a new game?")
						.setPositiveButton(android.R.string.yes,
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int which) {
										// continue with delete
										self.onSettings(nGeneral, nWallpaper,
												nCardStyle, nAdvanced, 1);
									}
								})
						.setNegativeButton(android.R.string.no,
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int which) {
										// do nothing
									}
								}).setIcon(android.R.drawable.ic_dialog_alert)
						.show();
			} else
				self.onSettings(nGeneral, nWallpaper, nCardStyle, nAdvanced, 0);
		}
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);

		Display display = getWindowManager().getDefaultDisplay();
		float w = display.getWidth();
		float h = display.getHeight();
		
		System.out.println(" ####### " + w + ",  " + h + "  ########");
		
		// Checks the orientation of the screen
		if (newConfig.orientation == Configuration.ORIENTATION_LANDSCAPE) {
			System.out.println(" ----  landscape ----");
			Toast.makeText(this, "landscape", Toast.LENGTH_SHORT).show();
			self.onChangedOrientation(false, w, h);
		} else if (newConfig.orientation == Configuration.ORIENTATION_PORTRAIT) {
			System.out.println(" ----  portrait ----");
			Toast.makeText(this, "portrait", Toast.LENGTH_SHORT).show();
			self.onChangedOrientation(true, w, h);
		}
	}
	
	/**
	 *  google admob
	 */
	public static void hideAd()
    {
     self.runOnUiThread(new Runnable()
     {
	     @Override
	     public void run()
	     {
			if (self.adView.isEnabled())
				self.adView.setEnabled(false);
			if (self.adView.getVisibility() != 4 )
				self.adView.setVisibility(View.INVISIBLE);
			}
	     });
    }

    public static void showAd()
    {
    	 self.runOnUiThread(new Runnable()
	     {
		     @Override
		     public void run()
		     {  
				if (!self.adView.isEnabled())
					self.adView.setEnabled(true);
				if (self.adView.getVisibility() == 4 )
					self.adView.setVisibility(View.VISIBLE); 
			 }
	     });

    }

	/* (non-Javadoc)
	 * @see org.cocos2dx.lib.Cocos2dxActivity#onPause()
	 */
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		
//		self.saveState();
	}

	/* (non-Javadoc)
	 * @see org.cocos2dx.lib.Cocos2dxActivity#onDestroy()
	 */
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		
//		self.saveState();
	}
}
