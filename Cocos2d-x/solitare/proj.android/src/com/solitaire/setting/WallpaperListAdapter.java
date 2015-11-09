package com.solitaire.setting;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.leejyhung.RealSolitaire.R;
import com.solitaire.global.GlobalContants;

public class WallpaperListAdapter extends ArrayAdapter<String> {
	private final Context context;
	private final String[] values;

	private int nSelectedIndex;
	private boolean bIsShow;
	
	public WallpaperListAdapter(Context context, String[] values) {
		super(context, R.layout.custom_list, values);
		this.context = context;
		this.values = values;
		bIsShow = false;
	}

	@SuppressLint("ViewHolder")
	@Override
	public View getView(final int position, View convertView,
			final ViewGroup parent) {
		LayoutInflater inflater = (LayoutInflater) context
				.getSystemService(Context.LAYOUT_INFLATER_SERVICE);

		View rowView = inflater.inflate(R.layout.custom_list, parent, false);
		//rowView.setOnLongClickListener(null);
		TextView textView = (TextView) rowView.findViewById(R.id.txt);
		textView.setText(GlobalContants.kWallpaperNames[position]);
		
		ImageView imageView = (ImageView) rowView.findViewById(R.id.logo);
		ImageView imgCheck = (ImageView)rowView.findViewById(R.id.img_sel);
				
		String filename = values[position];
		if( filename.equals("Select Photo...") )
		{
			if( bIsShow )
			{
				imageView.setVisibility(View.INVISIBLE);
				imgCheck.setVisibility(View.INVISIBLE);
				
				return rowView;
			}
			else
			{
				rowView = inflater.inflate(R.layout.empty_list, parent, false);
				return rowView;
			}
		}
		
		imageView.setVisibility(View.VISIBLE);
		int resID = this.context.getResources().getIdentifier(filename,
				"drawable", this.context.getPackageName());
		imageView.setImageResource(resID);
		
		if( position == nSelectedIndex )
			imgCheck.setVisibility(View.VISIBLE);
		else
			imgCheck.setVisibility(View.INVISIBLE);

		return rowView;
	}
	
	public void setSelectedIndex(int pos)
	{
		nSelectedIndex = pos;
	}
	
	public void showSelectPhotoItem(boolean isShow)
	{
		bIsShow = isShow;
	}
}