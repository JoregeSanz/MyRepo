package com.panda.facebook;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import com.facebook.android.*;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * User: YRH
 * Date: 6/14/13
 * Time: 12:54 PM
 */
public class FacebookManager {
    public static abstract class FBDelegate {
        public void OnLoginSuccess(FacebookManager fbMgr) {
        }

        public void OnLoginFail(FacebookManager fbMgr, String err) {
        }

        public void OnLogoutSuccess(FacebookManager fbMgr) {
        }

        public void OnUploadSuccess(FacebookManager fbMgr) {
        }

        public void OnUploadFail(FacebookManager fbMgr) {
        }
    }

    private static class EventListener extends BaseRequestListener implements Facebook.DialogListener {
        private Activity mA;
        private FBDelegate mDel;

        public EventListener(Activity a, FBDelegate del) {
            mA = a;
            mDel = del;
        }

        @Override
        public void onComplete(Bundle values) {
            SessionStore.save(Utility.mFacebook, mA);
            mDel.OnLoginSuccess(shared);
        }

        @Override
        public void onFacebookError(FacebookError error) {
            mDel.OnLoginFail(shared, error.getLocalizedMessage());
        }

        @Override
        public void onError(DialogError error) {
            mDel.OnLoginFail(shared, error.getLocalizedMessage());
        }

        @Override
        public void onCancel() {
            mDel.OnLoginFail(shared, "Operation canceled");
        }

        // for BaseRequestListener
        @Override
        public void onComplete(String response, final Object state) {
            Integer id = (Integer) state;
            switch (id) {
                case ID_LOGOUT:
                    SessionStore.clear(mA);
                    mDel.OnLogoutSuccess(shared);
                    break;
                case ID_PHOTO:
                    mDel.OnUploadSuccess(shared);
                    break;
                default:
                    break;
            }
        }

        @Override
        public void onFacebookError(FacebookError e, final Object state) {
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    private static FacebookManager shared = null;

    public static FacebookManager sharedManager() {
        if (shared == null)
            shared = new FacebookManager();
        return shared;
    }

    /////////////////////////////////////////////////////////////////////////////////
    @SuppressWarnings("deprecation")
	private FacebookManager() {
        Utility.mFacebook = new Facebook(APP_ID);
        Utility.mAsyncRunner = new AsyncFacebookRunner(Utility.mFacebook);
    }

    @SuppressWarnings("deprecation")
	public boolean IsLogined() {
        return Utility.mFacebook.isSessionValid();
    }

    @SuppressWarnings("deprecation")
	public void SessionOpenOrClose(Activity a, FBDelegate del, int requestCode) {
        final Activity activity = a;
        SessionStore.restore(Utility.mFacebook, activity);

        EventListener listener = new EventListener(a, del);
        if (Utility.mFacebook.isSessionValid()) {
            AsyncFacebookRunner asyncRunner = new AsyncFacebookRunner(Utility.mFacebook);
            asyncRunner.logout(activity, listener, ID_LOGOUT);
        } else {
//            Utility.mFacebook.login(activity, PERMITION, requestCode, listener);
            Utility.mFacebook.authorize(activity, PERMITION, Facebook.FORCE_DIALOG_AUTH, new EventListener(activity, del));
        }
    }

    public boolean UploadPhoto(Activity a, FBDelegate del, Bitmap img, String strComment) {
        if (!IsLogined()) return false;
        try {
//            Bitmap bmp = BitmapFactory.decodeFile(strImg);
        	Bitmap bmp = img;
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            bmp.compress(Bitmap.CompressFormat.JPEG, 100, baos);
            byte[] imgs = baos.toByteArray();
            baos.close();
            return UploadPhoto(a, del, imgs, strComment);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return false;
    }

	public boolean UploadPhoto(Activity a, FBDelegate del, byte[] img, String strComment) {
        if (!IsLogined()) return false;
        Bundle params = new Bundle();
        params.putByteArray("photo", img);
        params.putString("caption", strComment);

        EventListener listener = new EventListener(a, del);
        Utility.mAsyncRunner.request("me/photos", params, "POST", listener, ID_PHOTO);
        return true;
    }

    @SuppressWarnings("deprecation")
	public void OnActivityResult(int requestCode, int resultCode, Intent data) {
        Utility.mFacebook.authorizeCallback(requestCode, resultCode, data);
    }

    /////////////////////////////////////////////////////////////////////////////////
    private static final String APP_ID = "480820128712184";
    private static final String[] PERMITION = {
            "offline_access", "publish_stream", "user_photos",
            "publish_checkins", "photo_upload"};

    private static final int ID_LOGOUT = 0;
    private static final int ID_PHOTO = 1;
}