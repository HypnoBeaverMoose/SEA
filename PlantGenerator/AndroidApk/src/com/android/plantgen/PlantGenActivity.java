/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.plantgen;

import android.content.res.AssetManager;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;
import java.io.File;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;


public class PlantGenActivity extends Activity 
{

    PlantGenView mView;
	private AssetManager mgr;
    @Override protected void onCreate(Bundle icicle) 
	{
		mgr = getResources().getAssets();
		PlantGenLib.SetAssetManager(mgr);
		mView = new PlantGenView(getApplication());
        
		super.onCreate(icicle);
		setContentView(mView);
    }

    @Override protected void onPause() 
	{
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() 
	{
        super.onResume();
        mView.onResume();
    }
}
