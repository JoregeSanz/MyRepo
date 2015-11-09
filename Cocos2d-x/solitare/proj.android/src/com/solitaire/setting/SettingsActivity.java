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
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.leejyhung.RealSolitaire.R;
import com.solitaire.global.GlobalContants;

/**
 * @author LightSky
 * @since 2015/02/19
 */

public class SettingsActivity extends Activity implements OnClickListener {

	private final int REQUEST_WALLPAPER = 101;
	private final int REQUEST_CARDSTYLE = 102;
	private final int REQUEST_ADVANCED = 103;

	private String m_sFilepath;
	private int m_nGeneral;
	private int m_nWallpaper;
	private boolean m_bIsCostomed;
	private int m_nCardStyle;
	private int m_nAdvanced;

	// draw
	private ImageView imgDrawOne;
	private ImageView imgDrawThree;

	// scoring
	private ImageView imgNone;
	private ImageView imgStandard;
	private ImageView imgVegas;
	private ToggleButton toggleCumulative;

	private ToggleButton toggleTimedGame;
	private ToggleButton toggleShowMoves;
	private ToggleButton toggleGameSounds;

	private TextView txtWallpaper;
	private TextView txtCards;

	private final int nBitDrawOne = 0;
	private final int nBitDrawThree = 1;
	private final int nBitNone = 2;
	private final int nBitStandard = 3;
	private final int nBitVegas = 4;
	private final int nBitCumulative = 5;
	private final int nBitTimedGame = 6;
	private final int nBitShowMoves = 7;
	private final int nBitGameSounds = 8;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.settings);

		/*	dialog style
		getWindow().setLayout(ViewGroup.LayoutParams.MATCH_PARENT,
				ViewGroup.LayoutParams.MATCH_PARENT);
		getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
		*/

		Button btnDone = (Button) findViewById(R.id.btn_done);
		btnDone.setOnClickListener(this);

		imgDrawOne = (ImageView) findViewById(R.id.img_draw_one);
		findViewById(R.id.setting_draw_one).setOnClickListener(this);

		imgDrawThree = (ImageView) findViewById(R.id.img_draw_three);
		findViewById(R.id.setting_draw_three).setOnClickListener(this);

		imgNone = (ImageView) findViewById(R.id.img_none);
		findViewById(R.id.setting_none).setOnClickListener(this);

		imgStandard = (ImageView) findViewById(R.id.img_standard);
		findViewById(R.id.setting_standard).setOnClickListener(this);

		imgVegas = (ImageView) findViewById(R.id.img_vegas);
		findViewById(R.id.setting_vegas).setOnClickListener(this);

		toggleCumulative = (ToggleButton) findViewById(R.id.toggle_cumulative);
		toggleCumulative.setOnClickListener(this);

		toggleTimedGame = (ToggleButton) findViewById(R.id.toggle_timed_game);
		toggleTimedGame.setOnClickListener(this);

		toggleShowMoves = (ToggleButton) findViewById(R.id.toggle_show_moves);
		toggleShowMoves.setOnClickListener(this);

		toggleGameSounds = (ToggleButton) findViewById(R.id.toggle_game_sounds);
		toggleGameSounds.setOnClickListener(this);

		txtWallpaper = (TextView) findViewById(R.id.txt_wallpaper);
		findViewById(R.id.setting_wallpaper).setOnClickListener(this);

		txtCards = (TextView) findViewById(R.id.txt_cardStyle);
		findViewById(R.id.setting_cards).setOnClickListener(this);

		findViewById(R.id.setting_howtoplay).setOnClickListener(this);

		findViewById(R.id.setting_advanced).setOnClickListener(this);

		findViewById(R.id.setting_about).setOnClickListener(this);

		readSettingInfo();
	}

	private void readSettingInfo() {
		Intent intent = getIntent();
		m_sFilepath = intent.getStringExtra(GlobalContants.kCustomFilePath);
		m_nGeneral = intent.getIntExtra("nGeneral", 0);
		m_nWallpaper = intent.getIntExtra("nWallpaper", 3);
		m_nCardStyle = intent.getIntExtra("nCardStyle", 0);
		m_nAdvanced = intent.getIntExtra(GlobalContants.kAdvanced, 0);

		// general
		if ((m_nGeneral & (1 << nBitDrawOne)) == (1 << nBitDrawOne)) {
			imgDrawOne.setVisibility(View.VISIBLE);
			imgDrawThree.setVisibility(View.INVISIBLE);
		} else {
			imgDrawOne.setVisibility(View.INVISIBLE);
			imgDrawThree.setVisibility(View.VISIBLE);
		}

		// scroring
		if ((m_nGeneral & (1 << nBitNone)) == (1 << nBitNone)) {
			imgNone.setVisibility(View.VISIBLE);
			imgStandard.setVisibility(View.INVISIBLE);
			imgVegas.setVisibility(View.INVISIBLE);

			findViewById(R.id.setting_cumulative).setVisibility(View.GONE);
			findViewById(R.id.sepline_cumulative).setVisibility(View.GONE);
		} else if ((m_nGeneral & (1 << nBitStandard)) == (1 << nBitStandard)) {
			imgNone.setVisibility(View.INVISIBLE);
			imgStandard.setVisibility(View.VISIBLE);
			imgVegas.setVisibility(View.INVISIBLE);

			findViewById(R.id.setting_cumulative).setVisibility(View.GONE);
			findViewById(R.id.sepline_cumulative).setVisibility(View.GONE);
		} else if ((m_nGeneral & (1 << nBitVegas)) == (1 << nBitVegas)) {
			imgNone.setVisibility(View.INVISIBLE);
			imgStandard.setVisibility(View.INVISIBLE);
			imgVegas.setVisibility(View.VISIBLE);

			findViewById(R.id.setting_cumulative).setVisibility(View.VISIBLE);
			findViewById(R.id.sepline_cumulative).setVisibility(View.VISIBLE);
		}

		if ((m_nGeneral & (1 << nBitCumulative)) == (1 << nBitCumulative))
			toggleCumulative.setChecked(true);

		if ((m_nGeneral & (1 << nBitTimedGame)) == (1 << nBitTimedGame))
			toggleTimedGame.setChecked(true);

		if ((m_nGeneral & (1 << nBitShowMoves)) == (1 << nBitShowMoves))
			toggleShowMoves.setChecked(true);

		if ((m_nGeneral & (1 << nBitGameSounds)) == (1 << nBitGameSounds))
			toggleGameSounds.setChecked(true);

		// wallpaper
		txtWallpaper.setText(GlobalContants.kWallpaperNames[m_nWallpaper]);

		if ((m_nCardStyle & (1 << GlobalContants.nBitFaceClassic)) == (1 << GlobalContants.nBitFaceClassic))
			txtCards.setText("Classic");
		else
			txtCards.setText("Standard");
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub

		if (v.getId() == R.id.btn_done) {
			Intent intent = new Intent();
			intent.putExtra("nGeneral", m_nGeneral);
			intent.putExtra("nWallpaper", m_nWallpaper);
			intent.putExtra(GlobalContants.kIsCustomed, m_bIsCostomed);
			intent.putExtra("nCardStyle", m_nCardStyle);
			intent.putExtra(GlobalContants.kAdvanced, m_nAdvanced);
			setResult(RESULT_OK, intent);

			finish();
		} else if (v.getId() == R.id.setting_draw_one) {
			if ((m_nGeneral & (1 << nBitDrawOne)) != (1 << nBitDrawOne)) {
				imgDrawOne.setVisibility(View.VISIBLE);
				m_nGeneral |= (1 << nBitDrawOne);

				imgDrawThree.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitDrawThree)) == (1 << nBitDrawThree))
					m_nGeneral ^= (1 << nBitDrawThree);
			}
		} else if (v.getId() == R.id.setting_draw_three) {
			if ((m_nGeneral & (1 << nBitDrawThree)) != (1 << nBitDrawThree)) {
				imgDrawThree.setVisibility(View.VISIBLE);
				m_nGeneral |= (1 << nBitDrawThree);

				imgDrawOne.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitDrawOne)) == (1 << nBitDrawOne))
					m_nGeneral ^= (1 << nBitDrawOne);
			}
		} else if (v.getId() == R.id.setting_none) {
			if ((m_nGeneral & (1 << nBitNone)) != (1 << nBitNone)) {
				imgNone.setVisibility(View.VISIBLE);
				m_nGeneral |= (1 << nBitNone);

				imgStandard.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitStandard)) == (1 << nBitStandard))
					m_nGeneral ^= (1 << nBitStandard);

				imgVegas.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitVegas)) == (1 << nBitVegas))
					m_nGeneral ^= (1 << nBitVegas);

				findViewById(R.id.setting_cumulative).setVisibility(View.GONE);
				findViewById(R.id.sepline_cumulative).setVisibility(View.GONE);
			}
		} else if (v.getId() == R.id.setting_standard) {
			if ((m_nGeneral & (1 << nBitStandard)) != (1 << nBitStandard)) {
				imgStandard.setVisibility(View.VISIBLE);
				m_nGeneral |= (1 << nBitStandard);

				imgNone.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitNone)) == (1 << nBitNone))
					m_nGeneral ^= (1 << nBitNone);

				imgVegas.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitVegas)) == (1 << nBitVegas))
					m_nGeneral ^= (1 << nBitVegas);

				findViewById(R.id.setting_cumulative).setVisibility(View.GONE);
				findViewById(R.id.sepline_cumulative).setVisibility(View.GONE);
			}
		} else if (v.getId() == R.id.setting_vegas) {
			if ((m_nGeneral & (1 << nBitVegas)) != (1 << nBitVegas)) {
				imgVegas.setVisibility(View.VISIBLE);
				m_nGeneral |= (1 << nBitVegas);

				findViewById(R.id.setting_cumulative).setVisibility(
						View.VISIBLE);
				findViewById(R.id.sepline_cumulative).setVisibility(
						View.VISIBLE);

				imgNone.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitNone)) == (1 << nBitNone))
					m_nGeneral ^= (1 << nBitNone);

				imgStandard.setVisibility(View.INVISIBLE);
				if ((m_nGeneral & (1 << nBitStandard)) == (1 << nBitStandard))
					m_nGeneral ^= (1 << nBitStandard);
			}
		} else if (v.getId() == R.id.toggle_cumulative) {
			if (toggleCumulative.isChecked()) {
				m_nGeneral |= (1 << nBitCumulative);
				System.out.println(" ## cumulative is checked ##\n");
			} else {
				if ((m_nGeneral & (1 << nBitCumulative)) == (1 << nBitCumulative)) {
					m_nGeneral ^= (1 << nBitCumulative);
					System.out.println(" ## cumulative is unchecked ##\n");
				}
			}
		} else if (v.getId() == R.id.toggle_timed_game) {
			if (toggleTimedGame.isChecked())
				m_nGeneral |= (1 << nBitTimedGame);
			else {
				if ((m_nGeneral & (1 << nBitTimedGame)) == (1 << nBitTimedGame))
					m_nGeneral ^= (1 << nBitTimedGame);
			}
		} else if (v.getId() == R.id.toggle_show_moves) {
			if (toggleShowMoves.isChecked())
				m_nGeneral |= (1 << nBitShowMoves);
			else {
				if ((m_nGeneral & (1 << nBitShowMoves)) == (1 << nBitShowMoves))
					m_nGeneral ^= (1 << nBitShowMoves);
			}
		} else if (v.getId() == R.id.toggle_game_sounds) {
			if (toggleGameSounds.isChecked())
				m_nGeneral |= (1 << nBitGameSounds);
			else {
				if ((m_nGeneral & (1 << nBitGameSounds)) == (1 << nBitGameSounds))
					m_nGeneral ^= (1 << nBitGameSounds);
			}
		} else if (v.getId() == R.id.setting_wallpaper) {
			
			Intent intent = new Intent(SettingsActivity.this, WallpaperActivity.class);
			intent.putExtra(GlobalContants.kCustomFilePath, m_sFilepath);
			intent.putExtra(GlobalContants.kWallpaperNumber, m_nWallpaper);
			startActivityForResult(intent, REQUEST_WALLPAPER);
		} else if (v.getId() == R.id.setting_cards) {
			
			Intent intent = new Intent(SettingsActivity.this, CardStyleActivity.class);
			intent.putExtra(GlobalContants.kCardStyleParam, m_nCardStyle);
			startActivityForResult(intent, REQUEST_CARDSTYLE);
		} else if (v.getId() == R.id.setting_howtoplay) {
			
			Intent intent = new Intent(SettingsActivity.this, Howtoplay.class);
			startActivity(intent);
		} else if (v.getId() == R.id.setting_advanced) {
			
			Intent intent = new Intent(SettingsActivity.this, AdvancedActivity.class);
			intent.putExtra(GlobalContants.kAdvanced, m_nAdvanced);
			startActivityForResult(intent, REQUEST_ADVANCED);
		} else if (v.getId() == R.id.setting_about) {
			
			Intent intent = new Intent(SettingsActivity.this, AboutActivity.class);
			startActivity(intent);
			// Toast.makeText(SettingsActivity.this, "Coming soon",
			// Toast.LENGTH_LONG).show();
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);

		if (requestCode == REQUEST_WALLPAPER && resultCode == RESULT_OK) {
			int nWallpaper = data.getIntExtra(GlobalContants.kWallpaperNumber,
					3);
			m_nWallpaper = nWallpaper;
			m_bIsCostomed = data.getBooleanExtra(GlobalContants.kIsCustomed,
					false);

			// wallpaper
			txtWallpaper.setText(GlobalContants.kWallpaperNames[m_nWallpaper]);
		} else if (requestCode == REQUEST_CARDSTYLE && resultCode == RESULT_OK) {
			int nCardStyle = data
					.getIntExtra(GlobalContants.kCardStyleParam, 0);
			m_nCardStyle = nCardStyle;

			if ((m_nCardStyle & (1 << GlobalContants.nBitFaceClassic)) == (1 << GlobalContants.nBitFaceClassic))
				txtCards.setText("Classic");
			else
				txtCards.setText("Standard");
		}
		else if( requestCode == REQUEST_ADVANCED && resultCode == RESULT_OK) {
			m_nAdvanced = data.getIntExtra(GlobalContants.kAdvanced, 0);
		}
	}
}