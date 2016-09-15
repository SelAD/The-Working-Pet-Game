/****************************************************************************
Copyright (c) 2015 Chukong Technologies Inc.
 
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
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.ResizeLayout;
import android.annotation.TargetApi;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Point;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.view.Display;
import android.view.ViewGroup;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.FrameLayout;
import android.widget.Toast;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.startapp.android.publish.StartAppAd;
import com.startapp.android.publish.StartAppSDK;

public class AppActivity extends Cocos2dxActivity {
    private static AppActivity _appActiviy;
    private AdView adView;
    private StartAppAd startAppAd = new StartAppAd(this);
    private static final String AD_UNIT_ID = "ca-app-pub-7570080929753196/2206180662";
    public static void showToast(final String message) {
       _appActiviy.runOnUiThread(new Runnable() {
           @Override
           public void run() {
               Toast.makeText(_appActiviy.getBaseContext(), message, Toast.LENGTH_SHORT).show();
           }
       });
    }
    // Helper get display screen to avoid deprecated function use
    private Point getDisplaySize(Display d)
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
        {
            return getDisplaySizeGE11(d);
        }
        return getDisplaySizeLT11(d);
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private Point getDisplaySizeGE11(Display d)
    {
        Point p = new Point(0, 0);
        d.getSize(p);
        return p;
    }
    private Point getDisplaySizeLT11(Display d)
    {
        try
        {
            Method getWidth = Display.class.getMethod("getWidth", new Class[] {});
            Method getHeight = Display.class.getMethod("getHeight", new Class[] {});
            return new Point(((Integer) getWidth.invoke(d, (Object[]) null)).intValue(), ((Integer) getHeight.invoke(d, (Object[]) null)).intValue());
        }
        catch (NoSuchMethodException e2) // None of these exceptions should ever occur.
        {
            return new Point(-1, -1);
        }
        catch (IllegalArgumentException e2)
        {
            return new Point(-2, -2);
        }
        catch (IllegalAccessException e2)
        {
            return new Point(-3, -3);
        }
        catch (InvocationTargetException e2)
        {
            return new Point(-4, -4);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        int width = getDisplaySize(getWindowManager().getDefaultDisplay()).x;

        /*Startapp-SDK*/
        StartAppSDK.init(this, "210096496", true);
        /*End Startapp*/
       /* LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
                width,
                LinearLayout.LayoutParams.WRAP_CONTENT);
        adParams.gravity = Gravity.BOTTOM;*/

        adView = new AdView(this);
        adView.setAdSize(AdSize.SMART_BANNER);
        adView.setAdUnitId(AD_UNIT_ID);


        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
                .addTestDevice("8B978E8A7A95EF98B9DA40EC0D1A4ACB")
                .build();
        RelativeLayout relativeLayout = new RelativeLayout(this);
        RelativeLayout.LayoutParams adParams = new RelativeLayout.LayoutParams(
                AdView.LayoutParams.WRAP_CONTENT,
                AdView.LayoutParams.WRAP_CONTENT);
        // align bottom
        mFrameLayout.addView(relativeLayout);
        adParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        relativeLayout.addView(adView, adParams);
        adView.loadAd(adRequest);
        adView.setBackgroundColor(Color.BLACK);
        adView.setBackgroundColor(0);
        //addContentView(adView,adParams);

        _appActiviy = this;

    }


    public static void hideAd()
    {
        _appActiviy.runOnUiThread(new Runnable()
        {

            @Override
            public void run()
            {
                if (_appActiviy.adView.isEnabled())
                    _appActiviy.adView.setEnabled(false);
                if (_appActiviy.adView.getVisibility() != 4 )
                    _appActiviy.adView.setVisibility(View.INVISIBLE);
            }
        });

    }


    public static void showAd()
    {
        _appActiviy.runOnUiThread(new Runnable()
        {

            @Override
            public void run()
            {
                if (!_appActiviy.adView.isEnabled())
                    _appActiviy.adView.setEnabled(true);
                if (_appActiviy.adView.getVisibility() == 4 )
                    _appActiviy.adView.setVisibility(View.VISIBLE);
            }
        });

    }
    public static void showStartappAd()
    {
        _appActiviy.runOnUiThread(new Runnable() {

            @Override
            public void run() {
                _appActiviy.startAppAd.showAd();
                _appActiviy.startAppAd.loadAd();
            }
        });

    }
    public static void openURL(final String url)
    {
        Intent intent = new Intent(Intent.ACTION_VIEW);
        intent.setData(Uri.parse(url));
        _appActiviy.startActivity(intent);
    }
    @Override
    protected void onResume() {
        super.onResume();
        if (adView != null) {
            adView.resume();
        }
        _appActiviy.startAppAd.onResume();
    }

    @Override
    protected void onPause() {
        if (adView != null) {
            adView.pause();
        }
        super.onPause();
        _appActiviy.startAppAd.onPause();
    }

    @Override
    protected void onDestroy() {
        adView.destroy();
        super.onDestroy();
    }

}
