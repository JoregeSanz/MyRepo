package com.solitaire.setting;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.leejyhung.RealSolitaire.R;

public class ScoreListAdapter extends ArrayAdapter<GameResult> {

	private boolean m_isVegas;
	
	public ScoreListAdapter(Context context, GameResult[] values) {
		super(context, 0, values);
	}

	@SuppressLint("ViewHolder")
	@Override
	public View getView(final int position, View convertView,
			final ViewGroup parent) {

		GameResult gameResult = getItem(position);

		LayoutInflater inflater = (LayoutInflater) getContext()
				.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

		View rowView = inflater.inflate(R.layout.score_list, null, true);
		TextView textView = (TextView) rowView.findViewById(R.id.txt_name);

		String username = gameResult.m_sUsername + " " + gameResult.m_sDrawmode;
		if (gameResult.m_bTimed)
			username = username + "✪";
		textView.setText(username);
		
		TextView txtScore = (TextView)rowView.findViewById(R.id.txt_score);
		int nScore = gameResult.m_nScore;
		String score = String.valueOf(nScore);
		if( m_isVegas )
		{
			if( nScore >= 0 ) 
				score = "$" + score;
			else
				score = "-$" + String.valueOf(Math.abs(nScore)); 
			
			txtScore.setTextColor(getContext().getResources().getColor(R.color.minus_points_color));
		}

		txtScore.setText(score);
		
		return rowView;
	}
	
	public void setVegasMode(boolean isVegas)
	{
		m_isVegas = isVegas;
	}
}