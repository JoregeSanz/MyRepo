package com.solitaire.setting;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.StatusLine;
import org.apache.http.client.methods.HttpGet;

import android.app.Activity;
import android.app.Dialog;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.RelativeLayout;

import com.leejyhung.RealSolitaire.R;
import com.loopj.android.http.AsyncHttpClient;
import com.solitaire.global.GlobalContants;
import com.solitaire.global.GlobalFunc;
import com.solitaire.util.MainProgress;

public class WallpaperActivity extends Activity implements OnItemClickListener,
		OnClickListener {

	private ListView listView;
	
	private String m_sFilepath;
	private int m_nWallpaper;
	private boolean bIsCustomed = false;
	
	private WallpaperListAdapter adapter;
	
	// wallpaper names
	private final String sWallpaperFileNames[] = { "green", "blue", "red",
			"wood", "peacock", "red_sea_bannerfish", "rainforest",
			"spring_flowers", "tropical", "penguins",
			"borealis", "lava", "mirrorlake", "newyorkcity",
    		"raindrops", "santamonica", "skydive",
			"custom", "Select Photo..." };

	private final int GET_CAMERA = 1000;
	private final int GET_GALLERY = 1001;
	
	private Context context = this;
	AsyncHttpClient client = null;
	
	public WallpaperActivity() {
		// TODO Auto-generated constructor stub
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);

		setContentView(R.layout.wallpapers);

//		getWindow().setLayout(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
//		getWindow().setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
		
		m_sFilepath = getIntent().getStringExtra(GlobalContants.kCustomFilePath);
		m_nWallpaper = getIntent().getIntExtra(GlobalContants.kWallpaperNumber,
				3);

		findViewById(R.id.btn_done).setOnClickListener(this);

		listView = (ListView) findViewById(R.id.list_wallpapers);
		adapter = new WallpaperListAdapter(this, sWallpaperFileNames);
		adapter.setSelectedIndex(m_nWallpaper);
		listView.setAdapter(adapter);

		listView.setOnItemClickListener(this);
	}

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position,
			long id) {
		// TODO Auto-generated method stub
		m_nWallpaper = position;
		
		if( sWallpaperFileNames[position].equals("custom") )
		{
			adapter.showSelectPhotoItem(true);
		}
		else if( sWallpaperFileNames[position].equals("Select Photo...") )
		{
			m_nWallpaper = position-1;
			getCustomWallpaper();
			return;
		}
		else
		{
			adapter.showSelectPhotoItem(false);
			
			//for test
//			AppActivity sharedApp = AppActivity.getInstance(); 
//			sharedApp.setWallpaper(m_nWallpaper);
		}
		
		adapter.setSelectedIndex(m_nWallpaper);
		adapter.notifyDataSetChanged();
	}

	@Override
	public void onClick(View v) {
		// TODO Auto-generated method stub
		if (v.getId() == R.id.btn_done) {

			Intent intent = new Intent();
			intent.putExtra(GlobalContants.kWallpaperNumber, m_nWallpaper);
			intent.putExtra(GlobalContants.kIsCustomed, bIsCustomed);
			setResult(RESULT_OK, intent);
			
			finish();
		}
	}
	
	static String picturePath;
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		
		if( requestCode == GET_CAMERA )
		{
			if( resultCode == RESULT_OK ) 
			{
				File f = new File(Environment.getExternalStorageDirectory().toString());
				
				if ( f != null)
				{
		            for (File temp : f.listFiles()) {
		                if (temp != null && temp.getName() != null && temp.getName().equals("solitaire_temp")) {
		                    f = temp;
		                    break;
		                }
		            }
		            
		            Bitmap bitmap = null;
		            
		            try {
		            	BitmapFactory.Options bitmapOptions = new BitmapFactory.Options();

		            	//out of memory error
		            	bitmapOptions.inJustDecodeBounds = false;
		            	bitmapOptions.inPreferredConfig = Config.RGB_565;
		            	bitmapOptions.inDither = true;

		            	bitmap = BitmapFactory.decodeFile(f.getAbsolutePath(), bitmapOptions); 
		            	
		            	savePhoto(bitmap);
		            }
		            catch(OutOfMemoryError ome) {
		            	if ( bitmap != null ) {
		            		bitmap.recycle();
		            		bitmap = null;
		            	}
		            }
		            catch(Exception e) 
		            {

		            	if ( bitmap != null ) {
		            		bitmap.recycle();
		            		bitmap = null;
		            	}
	
		            	try {
		            		FileInputStream stream = new FileInputStream(f);
		            		bitmap = GlobalFunc.decodeFile(stream);
		            		stream.close();
	
		            		savePhoto(bitmap);
		            		
		            		if ( bitmap != null ) {
		            			bitmap.recycle();
		            			bitmap = null;
		            		}
	
		            	} catch (Exception e1) {
		            		e1.printStackTrace();
		            	}
	
		            	if ( bitmap != null ) {
		            		bitmap.recycle();
		            		bitmap = null;
		            	}
					}
				}
	  		} 
  		} 
		else if ( requestCode == GET_GALLERY ) 
		{
			if( resultCode == RESULT_OK ) 
			{
				Uri selectedImage = data.getData();
				
				if ( selectedImage == null ) 
					return;
				
				String[] filePath = { MediaStore.Images.Media.DATA };
				
	            Cursor c = getContentResolver().query(selectedImage,filePath, null, null, null);
	            
	            if ( c == null)
	            	return;
	            
	            if ( c.moveToFirst() ) {
	            	try{
	            		int columnIndex = c.getColumnIndex(filePath[0]);
	            		picturePath = c.getString(columnIndex);
	            	}catch(Exception e){}
	            }
	            
	            c.close();
	            
	            if( picturePath == null && selectedImage.getHost().startsWith("com.") ) {	//google.android.gallery3d.provider
	            	picturePath = data.getData().toString();
				}
	            
	            new AsyncTask<String, Void, Bitmap>() {

	            	private MainProgress dialog;
	            	private Bitmap myThumbnail = null;
	            	@Override
	            	protected void onPreExecute() {
	            		dialog = new MainProgress(WallpaperActivity.this);
	            		dialog.setMessage(getString(R.string.loading_image));
	            		dialog.setCancelable(false);
	            		dialog.show();
	            	}
	            	@Override
	            	protected void onPostExecute(Bitmap result) {
	            		super.onPreExecute();

	            		if (dialog != null && dialog.isShowing()) {
	            			dialog.dismiss();
	            		}
	            		if (myThumbnail != null) {
	            			savePhoto(myThumbnail);
	            			/*myThumbnail.recycle();
	            			myThumbnail = null;*/
	            		}
	            	}

	            	@Override
	            	protected Bitmap doInBackground(String... params) {
	            		
	            		if( params[0].startsWith("content://com.") ) { //google.android.gallery3d.provider
	        				ContentResolver res = context.getContentResolver();
	        				Uri uri = Uri.parse(params[0]);
	        				try{
	        					InputStream stream = res.openInputStream(uri);
	        					myThumbnail = BitmapFactory.decodeStream(stream);
	        					stream.close();
	        				}
	        				catch(Exception e) {
	        					return null;
	        				}
	        				return myThumbnail;
	        				
	        			} else if (params[0].startsWith("http")) {
	        				
	        				if ( client == null )
	        					client = new AsyncHttpClient();
	        				
	            			HttpGet getRequest = new HttpGet(params[0]);

	            			try {
	            				HttpResponse response = client.getHttpClient().execute(getRequest);
	            				
	            				if ( response == null )
	            					return null;
	            				
	            				StatusLine line = response.getStatusLine();
	            				
	            				int statusCode = 0;
	            				
	            				if ( line != null )
	            					statusCode = line.getStatusCode();
	            				
	            				if (statusCode != HttpStatus.SC_OK) { 
	            					Log.w("ImageDownloader", "Error " + statusCode + " while retrieving bitmap from " + params); 
	            					return null;
	            				}

	            				HttpEntity entity = null;
	            				
	            				entity = response.getEntity();
	            				
	            				if (entity != null) {
	            					InputStream inputStream = null;
	            					try {
	            						inputStream = entity.getContent(); 
	            						myThumbnail = BitmapFactory.decodeStream(inputStream);
	            					} finally {
	            						if (inputStream != null) {
	            							inputStream.close();  
	            						}
	            						entity.consumeContent();
	            					}
	            				}
	            			} catch (Exception e) {
	            				getRequest.abort();
	            				Log.w("ImageDownloader", "Error while retrieving bitmap from " + params[0]);
	            			} finally {
	            				/*if (client != null) {
	            					client.close();
	            				}*/
	            			}
	            			
	            			return myThumbnail;
	            		} else {
	            			
	            			try{
		            			//because out of memory error
		            			 BitmapFactory.Options options = new BitmapFactory.Options();;
		            			 options.inJustDecodeBounds = false;
		            			 options.inPreferredConfig = Config.RGB_565;
		            			 options.inDither = true;
		            			 myThumbnail = BitmapFactory.decodeFile(params[0], options);
	            			}
	            			catch(OutOfMemoryError ome){
	            				if ( myThumbnail != null ) {
	            					myThumbnail.recycle();
	            					myThumbnail = null;
	            				}
	            				
	            				FileInputStream fis = null;
	            				try{
	            					fis = new FileInputStream(params[0]);
	            				}catch(Exception e) {
	            					e.printStackTrace();
	            				}
	            				
	            				myThumbnail = GlobalFunc.decodeFile(fis);
	            				
	            				try{
		            				if ( fis != null )
		            					fis.close();
	            				}catch(Exception e){
	            					e.printStackTrace();
	            				}
	            			}catch(Exception e){
		            			
	            			}
	            			
	            			return myThumbnail;
	            		}
	            	}
	            }.execute(picturePath);
			}
		}
	}

	public void getCustomWallpaper()
	{
		final Dialog dlg = new Dialog(this);
    	dlg.requestWindowFeature(Window.FEATURE_NO_TITLE);
    	dlg.setContentView(R.layout.getphoto);
//    	mApp.setTypefacesAllView(dlg.findViewById(R.id.rootView));
    	dlg.getWindow().setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));

    	RelativeLayout lyt_album = (RelativeLayout)dlg.findViewById(R.id.lyt_album);
    	RelativeLayout lyt_camera = (RelativeLayout)dlg.findViewById(R.id.lyt_camera);
    	//RelativeLayout lyt_cancel = (RelativeLayout)dlg.findViewById(R.id.lyt_cancel);
    	
    	lyt_album.setOnClickListener(new OnClickListener(){

    		@Override
    		public void onClick(View v) {
    			dlg.dismiss();
    			
    			Intent intent = new   Intent(Intent.ACTION_PICK,android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                startActivityForResult(intent, GET_GALLERY);
    		}
    	});
    	
    	lyt_camera.setOnClickListener(new OnClickListener(){

    		@Override
    		public void onClick(View v) {
    			dlg.dismiss();
    			
    			Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
	   	         File f = new File(android.os.Environment.getExternalStorageDirectory(), "solitaire_temp");
	   	         intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(f));
	   	         startActivityForResult(intent, GET_CAMERA);
    		}
    	});
    	
    	dlg.setCanceledOnTouchOutside(true);
    	dlg.show();
	}
	
	private void savePhoto(Bitmap bmp)
	{
		System.out.println("\n####  **  custom wallpaper save **  ####\n");
		
		if( bmp == null )
			return;
		
		int width = bmp.getWidth();
		int height = bmp.getHeight();
		
		//rotate from landscape to portrait
		//if( width > height )	
		//	bmp = GlobalFunc.RotateBitmap(bmp, 90);
		
		//rotate from portrait to landscape
		if( width < height )	
			bmp = GlobalFunc.RotateBitmap(bmp, 90);
				
		//scale to screen size
		DisplayMetrics dm = new DisplayMetrics();
	    getWindowManager().getDefaultDisplay().getMetrics(dm);
	    int displayWidth = dm.widthPixels;
	    int displayHeight = dm.heightPixels;
	    
		bmp = Bitmap.createScaledBitmap(bmp, displayWidth, displayHeight, false);
		/////////////////
		
		final String filepath = m_sFilepath + "custom.png";
		
		File file = new File(filepath);
		file.delete();
		if(file.exists()){
		    try {
				file.getCanonicalFile().delete();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		    
		    if(file.exists()){
		         getApplicationContext().deleteFile(file.getName());
		    }
		}
		
		FileOutputStream out = null;
		try {
		    out = new FileOutputStream(filepath);
		    bmp.compress(Bitmap.CompressFormat.PNG, 100, out); // bmp is your Bitmap instance
		    // PNG is a lossless format, the compression factor (100) is ignored
		} catch (Exception e) {
		    e.printStackTrace();
		} finally {
		    try {
		        if (out != null) {
		            out.close();
		        }
		    } catch (IOException e) {
		        e.printStackTrace();
		    }
		}
		
		bIsCustomed = true;
	}
}
