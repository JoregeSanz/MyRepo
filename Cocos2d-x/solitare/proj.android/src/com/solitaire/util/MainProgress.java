package com.solitaire.util;

import android.app.Dialog;
import android.content.Context;
import android.view.View;
import android.view.Window;
import android.view.animation.Animation;
import android.view.animation.RotateAnimation;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.leejyhung.RealSolitaire.R;

public class MainProgress extends Dialog implements View.OnClickListener{
	
	private TextView 	txt_msg;
	Animation 			animation;
	ImageView 			imageView;
	Context 			mContext;
	
	boolean   cancelable = false;
	public MainProgress(Context context) {
		super(context, R.style.custom_dialog_theme);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.setContentView(R.layout.progress);
		mContext = context;
		setCancelable(false);
		
		txt_msg = (TextView)findViewById(R.id.txt_msg);
		
		imageView = (ImageView) findViewById(R.id.imgRing);

		findViewById(R.id.rootView).setOnClickListener(this);
		findViewById(R.id.layout1).setOnClickListener(this);
		txt_msg.setOnClickListener(this);
	}

	public void setMessage(final String msg)
	{
		txt_msg.setText(msg);
	}

	@Override
	public void dismiss() {
		super.dismiss();
		
		if ( animation != null ) {
			animation.cancel();
		}
		
		if (imageView != null ) {
			imageView.clearAnimation();
		}
	}

	@Override
	public void onClick(View v) {
		switch(v.getId()) {
		case R.id.rootView:
			if ( cancelable )
				dismiss();
			break;
		}
	}
	
	

	@Override
	public void show() {
		super.show();
		
		imageView.measure(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
		
		int width = imageView.getMeasuredWidth();
		int height = imageView.getMeasuredHeight();
		
		
		/*int width = mContext.getResources().getDimensionPixelOffset(R.dimen.progress_size);
		int height = width;*/

		animation = new RotateAnimation(0f, 360f, width/2, height/2);
		animation.setDuration(2000);
		animation.setRepeatCount(100);
		imageView.clearAnimation();
		imageView.setAnimation(animation);
		animation.startNow();
	}

	@Override
	public void setCancelable(boolean flag) {
		super.setCancelable(flag);
		cancelable = flag;
	}
}
