package com.solitaire.global;

import java.io.FileInputStream;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

public class GlobalFunc {
    /**
     * @author lightsky
     * @since 2014/05/13
     * @description out of memory error when image loading
     */
    public static Bitmap decodeFile(FileInputStream f)
    {
    	Bitmap bitmap = null;
    	try
    	{
    		//decode image size
    		BitmapFactory.Options o = new BitmapFactory.Options();
    		o.inJustDecodeBounds = true;

    		bitmap = BitmapFactory.decodeStream(f,null,o);
    		final int REQUIRED_SIZE=70;
    		int width_tmp=o.outWidth, height_tmp=o.outHeight;
    		int scale=1;
    		while(true)
    		{
    			if(width_tmp/2<REQUIRED_SIZE || height_tmp/2<REQUIRED_SIZE)
    				break;
    			width_tmp/=2;
    			height_tmp/=2;
    			scale*=2;
    		}
    		//decode with inSampleSize
    		BitmapFactory.Options o2 = new BitmapFactory.Options();
    		o2.inSampleSize=scale;
    		try {
    			bitmap = BitmapFactory.decodeStream(f, null, o2);
    		}catch(Exception e)
    		{
    			System.out.println("decode file Exception : " + e.toString());
    		} finally {
    			f.close();
    		}
    	}catch(OutOfMemoryError ome) {
    		if ( bitmap != null ){
    			bitmap.recycle();
    			bitmap = null;
    		}
    	}catch (Exception e) {
    		System.out.println("decode file Exception : " + e.toString());
    		
    	} 
    	return bitmap;
    }
    
	public static Bitmap RotateBitmap(Bitmap source, float angle)
	{
	      Matrix matrix = new Matrix();
	      matrix.postRotate(angle);
	      return Bitmap.createBitmap(source, 0, 0, source.getWidth(), source.getHeight(), matrix, true);
	}
}
