package com.solitaire.setting;

import android.R.color;
import android.app.Activity;
import android.content.Intent;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.text.BoringLayout;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Toast;

import com.leejyhung.RealSolitaire.R;
import com.solitaire.global.GlobalContants;
import com.solitaire.global.UserEmailFetcher;

public class AboutActivity extends Activity implements OnClickListener {

	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.about);
		
//		getWindow().setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
//		getWindow().setBackgroundDrawable(new ColorDrawable(color.transparent));
		
		findViewById(R.id.btn_done).setOnClickListener(this);
		
		findViewById(R.id.setting_visitsite).setOnClickListener(this);
		findViewById(R.id.setting_feedback).setOnClickListener(this);
		
		findViewById(R.id.setting_tell).setOnClickListener(this);
		
		findViewById(R.id.setting_twitter).setOnClickListener(this);
		findViewById(R.id.setting_facebook).setOnClickListener(this);
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		
		if( v.getId() == R.id.btn_done )
		{
			finish();
		}
		else if( v.getId() == R.id.setting_visitsite )
		{
			Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(GlobalContants.websiteAddress));
			startActivity(browserIntent);
		}
		else if( v.getId() == R.id.setting_feedback )
		{
			final String[] TO = {GlobalContants.emailAddress};
			final String[] CC = {UserEmailFetcher.getEmail(this)};
			final String subject = getResources().getString(R.string.feedback_subject);
			final String text = "";
			
			sendEmail(TO, CC, subject, text);
		}
		else if( v.getId() == R.id.setting_tell )
		{
			final String[] TO = null;
			final String[] CC = {UserEmailFetcher.getEmail(this)};
			final String subject = getResources().getString(R.string.tell_subject);
			final String text = getResources().getString(R.string.tell_text);
			
			sendEmail(TO, CC, subject, text);
		}
		else if( v.getId() == R.id.setting_twitter )
		{
			Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(GlobalContants.twitterSolitaireAddress));
			startActivity(browserIntent);
		}
		else if( v.getId() == R.id.setting_facebook )
		{
			Intent browserIntent = new Intent(Intent.ACTION_VIEW, Uri.parse(GlobalContants.facebookSolitaireAddress));
			startActivity(browserIntent);
		}
	}
	
	private void sendEmail(final String[] TO, final String[] CC, final String subject, final String text)
	{
		Intent emailIntent = new Intent(Intent.ACTION_SEND);
	    emailIntent.setData(Uri.parse("mailto:"));
	    emailIntent.setType("text/plain");

	    emailIntent.putExtra(Intent.EXTRA_EMAIL, TO);
	    emailIntent.putExtra(Intent.EXTRA_CC, CC);
	    emailIntent.putExtra(Intent.EXTRA_SUBJECT, subject);
	    emailIntent.putExtra(Intent.EXTRA_TEXT, text);

	    try {
	    	startActivity(Intent.createChooser(emailIntent, "Send mail..."));
	    	finish();
	    	Log.i("Finished sending email...", "");
	    } catch (android.content.ActivityNotFoundException ex) {
	    	Toast.makeText(AboutActivity.this, 
	    	"There is no email client installed.", Toast.LENGTH_SHORT).show();
	    }
	}
}
