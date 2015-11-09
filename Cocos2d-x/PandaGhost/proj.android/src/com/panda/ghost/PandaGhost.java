/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

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
package com.panda.ghost;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import twitter4j.Twitter;
import twitter4j.auth.AccessToken;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.Signature;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.widget.Toast;

import com.facebook.android.DialogError;
import com.facebook.android.Facebook;
import com.facebook.android.Facebook.DialogListener;
import com.facebook.android.FacebookError;
import com.panda.facebook.FacebookManager;
import com.panda.twitter.Twitt_Sharing;

public class PandaGhost extends Cocos2dxActivity {
	static PandaGhost shared;

	private static final int FE_REQ_FACEBOOK = 2;
	private static final int FE_TASK_TW = 2;

	static Facebook facebook = null;
	private static SharedPreferences mPrefs;

	static String strPostFB = "This game is Panda Ghost's Revenge!";
	static String strPostTwitter = "This game is Panda Ghost's Revenge!";

	// private Chartboost cb;

	// final static String CB_ID = "52fa173ff8975c1cfb8e4a6b";
	// final static String CB_SIGNATURE =
	// "a92fd42f88f5b6cea9fc75bc882b79e20be550e9";

	public final static String consumer_key = "mx87OyWcrFD6Y1TtYFrg";
	public final static String secret_key = "U3mQHdTgs0QF9zJCbaiWXFat5bsHS4NNJGwCuoqm3M";

	private AccessToken accessToken;
	private static Twitter twitter;

	@SuppressWarnings("deprecation")
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		shared = this;

		facebook = new Facebook("480820128712184");
		mPrefs = getPreferences(MODE_PRIVATE);

		String access_token = mPrefs.getString("access_token", null);
		long expires = mPrefs.getLong("access_expires", 0);
		if (access_token != null) {
			facebook.setAccessToken(access_token);
		}
		if (expires != 0) {
			facebook.setAccessExpires(expires);
		}

		// this.cb = Chartboost.sharedChartboost();
		// this.cb.onCreate(this, CB_ID, CB_SIGNATURE, null);
		// shared.cb.showInterstitial();

		PackageInfo packageInfo;
		try {
			packageInfo = getPackageManager().getPackageInfo("com.jin.pacpanda",
					PackageManager.GET_SIGNATURES);
			for (Signature signature : packageInfo.signatures) {
				MessageDigest md = MessageDigest.getInstance("SHA");
				md.update(signature.toByteArray());
				String key = new String(Base64.encode(md.digest(), 0));
				// String key = new String(Base64.encodeBytes(md.digest()));
				Log.e("Hash key", key);
			}
		} catch (NameNotFoundException e1) {
			Log.e("Name not found", e1.toString());
		}

		catch (NoSuchAlgorithmException e) {
			Log.e("No such an algorithm", e.toString());
		} catch (Exception e) {
			Log.e("Exception", e.toString());
		}

	}

	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// pacpanda should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	static {
		System.loadLibrary("cocos2dcpp");
	}

	@Override
	protected void onStart() {
		super.onStart();
		// this.cb.onStart(this);
		// this.cb.startSession();
	}

	@Override
	protected void onStop() {
		super.onStop();
		// this.cb.onStop(this);
	}

	@Override
	public void onBackPressed() {
		// TODO Auto-generated method stub
		// if (this.cb.onBackPressed())
		// return;
		// else
		super.onBackPressed();
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		facebook.authorizeCallback(requestCode, resultCode, data);

		switch (requestCode) {
		case FE_REQ_FACEBOOK: {
			FacebookManager.sharedManager().OnActivityResult(requestCode,
					requestCode, data);
			break;
		}
		}

	}

	@SuppressWarnings("deprecation")
	static void onPostFBMessageWithPhoto(int nscore) {
		
		Log.d("onPostFBMessageWithPhoto","onPostFBMessageWithPhoto");

		Bundle parameters = new Bundle();

		strPostFB = strPostFB + "    Score:" + String.valueOf(nscore);
		parameters.putString("message", strPostFB);

		String response = "";
		try {
			response = facebook.request("me/feed", parameters, "POST");
		} catch (Exception e) {

		}

		if (response == null || response.equals("") || response.equals("false")) {
			Toast.makeText(shared, (String) "post failed", Toast.LENGTH_SHORT)
					.show();
		} else {
			Toast.makeText(shared, (String) "successfully posted",
					Toast.LENGTH_SHORT).show();
		}

	}

	static void onReportMessageToFB(final int nscore) {

		FacebookManager.sharedManager();
		
		shared.runOnUiThread(new Runnable() {
			@SuppressWarnings("deprecation")
			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (!facebook.isSessionValid()) {
					facebook.authorize(shared,
							new String[] { "publish_stream" },
							new DialogListener() {

								@Override
								public void onComplete(Bundle values) {
									// TODO Auto-generated method stub
									SharedPreferences.Editor editor = mPrefs
											.edit();
									editor.putString("access_token",
											facebook.getAccessToken());
									editor.putLong("access_expires",
											facebook.getAccessExpires());
									editor.commit();

									onPostFBMessageWithPhoto(nscore);
								}

								@Override
								public void onFacebookError(FacebookError e) {
									// TODO Auto-generated method stub
									Log.d("Post","onFaceBookError");
								}

								@Override
								public void onError(DialogError e) {
									// TODO Auto-generated method stub
									Log.d("Post","onError");

								}

								@Override
								public void onCancel() {
									// TODO Auto-generated method stub
									Log.d("Post","onCancel");

								}

							});
				} else {
					onPostFBMessageWithPhoto(nscore);
				}
			}
		});
	}

	public static boolean isNetworkAvailable() {

		ConnectivityManager connectivity = (ConnectivityManager) shared
				.getContext().getSystemService(
						shared.getContext().CONNECTIVITY_SERVICE);
		if (connectivity == null) {
			return false;
		} else {
			NetworkInfo[] info = connectivity.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getState() == NetworkInfo.State.CONNECTED) {
						return true;
					}
				}
			}
		}
		return true;
	}

	@SuppressWarnings("deprecation")
	static void onReportMessageToTwitter(final int nscore) {

		shared.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (isNetworkAvailable()) {

					Twitt_Sharing twitt = new Twitt_Sharing(shared,
							consumer_key, secret_key);
					strPostTwitter = strPostTwitter + "   Score:"
							+ String.valueOf(nscore);

					twitt.shareToTwitter(strPostTwitter, null);
					// }
					// Now share both message & image to sharing activity

				} else {
					Toast.makeText(shared,
							"No Network Connection Available !!!",
							Toast.LENGTH_LONG).show();
				}
			}
		});
	}
}
