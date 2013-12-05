/*
    Copyright (c) 2012-2013, BogDan Vatra <bogdan@kde.org>
    Contact: http://www.qt-project.org/legal

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

package org.qtproject.qt5.android.bindings;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.lang.reflect.Method;
import java.lang.String;
import java.util.ArrayList;
import java.util.Arrays;

import org.kde.necessitas.ministro.IMinistro;
import org.kde.necessitas.ministro.IMinistroCallback;

import com.acs.smartcard.Reader;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.content.res.Resources.Theme;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.AttributeSet;
import android.util.Log;
import android.view.ContextMenu;
import android.view.ContextMenu.ContextMenuInfo;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager.LayoutParams;
import android.view.accessibility.AccessibilityEvent;
import dalvik.system.DexClassLoader;

//@ANDROID-11
import android.app.Fragment;
import android.view.ActionMode;
import android.view.ActionMode.Callback;
//@ANDROID-11





import com.acs.smartcard.Features;
import com.acs.smartcard.PinModify;
import com.acs.smartcard.PinProperties;
import com.acs.smartcard.PinVerify;
import com.acs.smartcard.ReadKeyOption;
import com.acs.smartcard.Reader;
import com.acs.smartcard.Reader.OnStateChangeListener;
import com.acs.smartcard.TlvProperties;


public class QtActivity extends Activity
{
    private final static int MINISTRO_INSTALL_REQUEST_CODE = 0xf3ee; // request code used to know when Ministro installation is finished
    private static final int MINISTRO_API_LEVEL = 3; // Ministro api level (check IMinistro.aidl file)
    private static final int NECESSITAS_API_LEVEL = 2; // Necessitas api level used by platform plugin
    private static final int QT_VERSION = 0x050100; // This app requires at least Qt version 5.1.0

    private static final String ERROR_CODE_KEY = "error.code";
    private static final String ERROR_MESSAGE_KEY = "error.message";
    private static final String DEX_PATH_KEY = "dex.path";
    private static final String LIB_PATH_KEY = "lib.path";
    private static final String LOADER_CLASS_NAME_KEY = "loader.class.name";
    private static final String NATIVE_LIBRARIES_KEY = "native.libraries";
    private static final String ENVIRONMENT_VARIABLES_KEY = "environment.variables";
    private static final String APPLICATION_PARAMETERS_KEY = "application.parameters";
    private static final String BUNDLED_LIBRARIES_KEY = "bundled.libraries";
    private static final String BUNDLED_IN_LIB_RESOURCE_ID_KEY = "android.app.bundled_in_lib_resource_id";
    private static final String BUNDLED_IN_ASSETS_RESOURCE_ID_KEY = "android.app.bundled_in_assets_resource_id";
    private static final String MAIN_LIBRARY_KEY = "main.library";
    private static final String STATIC_INIT_CLASSES_KEY = "static.init.classes";
    private static final String NECESSITAS_API_LEVEL_KEY = "necessitas.api.level";

    /// Ministro server parameter keys
    private static final String REQUIRED_MODULES_KEY = "required.modules";
    private static final String APPLICATION_TITLE_KEY = "application.title";
    private static final String MINIMUM_MINISTRO_API_KEY = "minimum.ministro.api";
    private static final String MINIMUM_QT_VERSION_KEY = "minimum.qt.version";
    private static final String SOURCES_KEY = "sources";               // needs MINISTRO_API_LEVEL >=3 !!!
                                                                       // Use this key to specify any 3rd party sources urls
                                                                       // Ministro will download these repositories into their
                                                                       // own folders, check http://community.kde.org/Necessitas/Ministro
                                                                       // for more details.

    private static final String REPOSITORY_KEY = "repository";         // use this key to overwrite the default ministro repsitory

    private static final String APPLICATION_PARAMETERS = null; // use this variable to pass any parameters to your application,
                                                               // the parameters must not contain any white spaces
                                                               // and must be separated with "\t"
                                                               // e.g "-param1\t-param2=value2\t-param3\tvalue3"

    private static final String ENVIRONMENT_VARIABLES = "QT_USE_ANDROID_NATIVE_STYLE=0\t";
                                                               // use this variable to add any environment variables to your application.
                                                               // the env vars must be separated with "\t"
                                                               // e.g. "ENV_VAR1=1\tENV_VAR2=2\t"
                                                               // Currently the following vars are used by the android plugin:
                                                               // * QT_USE_ANDROID_NATIVE_STYLE - 1 to use the android widget style if available,
                                                               //   note that the android style plugin in Qt 5.1 is not fully functional.

    private static final int INCOMPATIBLE_MINISTRO_VERSION = 1; // Incompatible Ministro version. Ministro needs to be upgraded.
    private static final int BUFFER_SIZE = 1024;

    private ActivityInfo m_activityInfo = null; // activity info object, used to access the libs and the strings
    private DexClassLoader m_classLoader = null; // loader object
    private String[] m_sources = {"https://download.qt-project.org/ministro/android/qt5/latest"}; // Make sure you are using ONLY secure locations
    private String m_repository = "default"; // Overwrites the default Ministro repository
                                                        // Possible values:
                                                        // * default - Ministro default repository set with "Ministro configuration tool".
                                                        // By default the stable version is used. Only this or stable repositories should
                                                        // be used in production.
                                                        // * stable - stable repository, only this and default repositories should be used
                                                        // in production.
                                                        // * testing - testing repository, DO NOT use this repository in production,
                                                        // this repository is used to push a new release, and should be used to test your application.
                                                        // * unstable - unstable repository, DO NOT use this repository in production,
                                                        // this repository is used to push Qt snapshots.
    private String[] m_qtLibs = null; // required qt libs

    private AssetManager m_mgr;
    public static native void SetAssetManager(Object mgr);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////BEGIN NFC DOCKING STATION CODE //////////////////////////    
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    private static final String CMD_GET_TAG_ID     = "FFCA000004";
    private static final String CMD_LOAD_AUTH_KEY  = "FF82000006FFFFFFFFFFFF";		//universal key 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
    private static final String CMD_AUTH_BLOCK_04  = "FF860000050100046100";
    private static final String CMD_READ_BLOCK_04  = "FFB0000410"; 
    private static final String CMD_CLEAR_BLOCK_04 = "FFD600041000000306D10102550042FE0000000000"; 
    
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";
    private static final String[] stateStrings = { "Unknown", "Absent",
        "Present", "Swallowed", "Powered", "Negotiable", "Specific" };
    private static final byte[] transferTagID = { (byte)0x6D, (byte)0xF8, (byte)0xF9, (byte)0xC8 };
    
    //private static final String UNIVERSAL_KEY = "FFFFFFFFFFFF";
    
    private UsbManager mManager;
    private Reader mReader;
    private PendingIntent mPermissionIntent;
    
    // NFC docking station receiver class
    private final BroadcastReceiver mReceiver = new BroadcastReceiver()
    {
        public void onReceive(Context context, Intent intent)
        {
            String action = intent.getAction();
            
            if (ACTION_USB_PERMISSION.equals(action))
            {
                synchronized (this)
                {
                    UsbDevice device = (UsbDevice) intent
                            .getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (intent.getBooleanExtra(
                            UsbManager.EXTRA_PERMISSION_GRANTED, false))
                    {

                        if (device != null)
                        {
                            // Open reader
                        	Log.d("Qt", "Opening reader: " + device.getDeviceName()
                                    + "...");
                            new OpenTask().execute(device);
                        }
                    } else
                    {
                    	Log.d("Qt", "Permission denied for device "
                                + device.getDeviceName());
                    }
                }
            } else if (UsbManager.ACTION_USB_DEVICE_DETACHED.equals(action))
            {
                synchronized (this)
                {                	
                	Log.d("Qt", "USB device detached!");
                	
                    UsbDevice device = (UsbDevice) intent
                            .getParcelableExtra(UsbManager.EXTRA_DEVICE);

                    if (device != null && device.equals(mReader.getDevice()))
                    {
                        // Close reader
                    	Log.d("Qt", "Closing reader...");
                        new CloseTask().execute();
                    }
                }
            }
        }
    };
    
    private class OpenTask extends AsyncTask<UsbDevice, Void, Exception>
    {
        @Override
        protected Exception doInBackground(UsbDevice... params)
        {
            Exception result = null;

            try
            {
                mReader.open(params[0]);
            } catch (Exception e)
            {
                result = e;
            }

            return result;
        }

        @Override
        protected void onPostExecute(Exception result)
        {
            if (result != null)
            {
                Log.d("Qt", "Open Task: " + result.toString());
            } else
            {
            	Log.d("Qt", "Reader name: " + mReader.getReaderName());

                int numSlots = mReader.getNumSlots();
                Log.d("Qt", "Number of slots: " + numSlots);

                NFCStation.setLabelText("Reader name: " + mReader.getReaderName());
            }
        }
    }

    private class CloseTask extends AsyncTask<Void, Void, Void>
    {
        @Override
        protected Void doInBackground(Void... params)
        {
            mReader.close();
            return null;
        }
    }
    
    
    private class PowerParams {

        public int slotNum;
        public int action;
    }

    private class PowerResult {

        public byte[] atr;
        public Exception e;
    }

    private class PowerTask extends AsyncTask<PowerParams, Void, PowerResult>
    {
        @Override
        protected PowerResult doInBackground(PowerParams... params)
        {
            PowerResult result = new PowerResult();

            try
            {
                result.atr = mReader.power(params[0].slotNum, params[0].action);
            } catch (Exception e)
            {
                result.e = e;
            }

            return result;
        }

        @Override
        protected void onPostExecute(PowerResult result)
        {
            if (result.e != null)
            {
            	Log.d("Qt", "Power Task: " + result.e.toString());
            } else
            {
            	// Set Parameters
                SetProtocolParams params = new SetProtocolParams();
                params.slotNum = 0;
                params.preferredProtocols = Reader.PROTOCOL_T0 | Reader.PROTOCOL_T1;

                // Set protocol
                Log.d("Qt", "Slot " + 0 + ": Setting protocol to T=0/T=1...");
                new SetProtocolTask().execute(params);
            }
        }
    }
    
    private class SetProtocolParams {

        public int slotNum;
        public int preferredProtocols;
    }

    private class SetProtocolResult
    {
        public int activeProtocol;
        public Exception e;
    }

    private class SetProtocolTask extends
            AsyncTask<SetProtocolParams, Void, SetProtocolResult> {

        @Override
        protected SetProtocolResult doInBackground(SetProtocolParams... params) {

            SetProtocolResult result = new SetProtocolResult();

            try
            {
                result.activeProtocol = mReader.setProtocol(params[0].slotNum,
                        params[0].preferredProtocols);

            } catch (Exception e)
            {
                result.e = e;
            }

            return result;
        }

        @Override
        protected void onPostExecute(SetProtocolResult result) {

            if (result.e != null)
            {
            	Log.d("Qt", "Set Protocol Task: " + result.e.toString());
            } else
            {
                String activeProtocolString = "Active Protocol: ";

                switch (result.activeProtocol) {

                case Reader.PROTOCOL_T0:
                    activeProtocolString += "T=0";
                    break;

                case Reader.PROTOCOL_T1:
                    activeProtocolString += "T=1";
                    break;

                default:
                    activeProtocolString += "Unknown";
                    break;
                }

                // Show active protocol
                Log.d("Qt", activeProtocolString);
                
                sendCommand(CMD_GET_TAG_ID);
            }
        }
    }
    
    private class TransmitParams {

        public int slotNum;
        public int controlCode;
        public String commandString;
    }

    private class TransmitProgress
    {
        public int controlCode;
        public byte[] command;
        public int commandLength;
        public byte[] response;
        public int responseLength;
        public Exception e;
    }

    private class TransmitTask extends
            AsyncTask<TransmitParams, TransmitProgress, Void>
    {
        @Override
        protected Void doInBackground(TransmitParams... params)
        {
            TransmitProgress progress = new TransmitProgress();

            byte[] command;
            byte[] response = new byte[300];
            int responseLength;
            int foundIndex;
            int startIndex = 0;

            do {

                // Find carriage return
                foundIndex = params[0].commandString.indexOf('\n', startIndex);
                if (foundIndex >= 0) {
                    command = toByteArray(params[0].commandString.substring(
                            startIndex, foundIndex));
                } else {
                    command = toByteArray(params[0].commandString
                            .substring(startIndex));
                }

                // Set next start index
                startIndex = foundIndex + 1;

                progress.controlCode = params[0].controlCode;
                try {

                    if (params[0].controlCode < 0) {

                        // Transmit APDU
                        responseLength = mReader.transmit(params[0].slotNum,
                                command, command.length, response,
                                response.length);

                    } else {

                        // Transmit control command
                        responseLength = mReader.control(params[0].slotNum,
                                params[0].controlCode, command, command.length,
                                response, response.length);
                    }

                    progress.command = command;
                    progress.commandLength = command.length;
                    progress.response = response;
                    progress.responseLength = responseLength;
                    progress.e = null;

                } catch (Exception e) {

                    progress.command = null;
                    progress.commandLength = 0;
                    progress.response = null;
                    progress.responseLength = 0;
                    progress.e = e;
                }

                publishProgress(progress);

            } while (foundIndex >= 0);

            return null;
        }

        @Override
        protected void onProgressUpdate(TransmitProgress... progress)
        {
            if (progress[0].e != null)
            {
            	Log.d("Qt", "TransmitTask: " + progress[0].e.toString());
            } else
            {
                String cmd = toHexString( progress[0].command );
                if ( CMD_GET_TAG_ID.equals(cmd) )
                {
                	Log.d("Qt", "Get Tag ID command response received: ");
                	logBuffer(progress[0].response, progress[0].responseLength);
                	
                	int rLen = progress[0].responseLength;
                	if ( progress[0].response[rLen - 2] == (byte)0x90
                			&& progress[0].response[rLen - 1] == (byte)0x00 )		// 0x90 0x00 means command succeeded
                	{
                		// check for transfer tag ID
                		for ( int i = 0; i < rLen - 2; ++i )
                		{
                			if ( i >= transferTagID.length )
                				break;
                			
                			Log.d("Qt", progress[0].response[i] + "==" + transferTagID[i]);
                			
                			if ( progress[0].response[i] != transferTagID[i] )
                			{
                				Log.d("Qt", "Wrong Tag ID found...");
                				return;												// tag is not the transferTag
                			}
                		}
                		
                		// send upload authentication key command
                		sendCommand(CMD_LOAD_AUTH_KEY);
                	}
                } else if ( CMD_LOAD_AUTH_KEY.equals(cmd) )
                {
                	Log.d("Qt", "Load authentication key command response received: ");
                	logBuffer(progress[0].response, progress[0].responseLength);
                	
                	int rLen = progress[0].responseLength;
                	if ( progress[0].response[rLen - 2] == (byte)0x90
                			&& progress[0].response[rLen - 1] == (byte)0x00 )		// 0x90 0x00 means command succeeded
                	{
                		// send authenticate block 4 command
                		sendCommand(CMD_AUTH_BLOCK_04);
                	}
                } else if ( CMD_AUTH_BLOCK_04.equals(cmd) )
                {
                	Log.d("Qt", "Authenticate block 04 command response received: ");
                	logBuffer(progress[0].response, progress[0].responseLength);
                	
                	int rLen = progress[0].responseLength;
                	if ( progress[0].response[rLen - 2] == (byte)0x90
                			&& progress[0].response[rLen - 1] == (byte)0x00 )		// 0x90 0x00 means command succeeded
                	{
                		// send read block 04 command
                		sendCommand(CMD_READ_BLOCK_04);
                	}
                } else if ( CMD_READ_BLOCK_04.equals(cmd) )
                {
                	Log.d("Qt", "Read block 04 command response received: ");
                	logBuffer(progress[0].response, progress[0].responseLength);
                	
                	int rLen = progress[0].responseLength;
                	if ( progress[0].response[rLen - 2] == (byte)0x90
                			&& progress[0].response[rLen - 1] == (byte)0x00 )		// 0x90 0x00 means command succeeded
                	{
                		int[] plants = { -1, -1, -1 };
                		int pCounter = 0;
                		
                		// parse block 04 data
                		if ( progress[0].response[7] == 85 )						//85 == 0x55 == 'U', which identifies the Well Known Type (WKT) for an URI record
                		{ 										
                			for ( int i = 10; i < 16; ++i )							// skip WKT identifier, URI identifier code and plant block start identifier of the first block of the section
                			{
                				if ( progress[0].response[i] == 254
                						|| pCounter >= 3 )							// 254 == 0xFE == TLV block terminator
                					break;
                				
                				plants[pCounter] = progress[0].response[i];
                				pCounter++;
                			}
                			
                			NFCStation.processPlants( plants[0], plants[1], plants[2] );
                			
                			sendCommand(CMD_CLEAR_BLOCK_04);
                		} else
                		{
                			Log.d("Qt", "Error parsing block 04 data");
                		}
                	}
                } else if ( CMD_CLEAR_BLOCK_04.equals(cmd) )
                {
                	Log.d("Qt", "Transfertag cleared");
                	Log.d("Qt", "Read block 04 command response received: ");
                	logBuffer(progress[0].response, progress[0].responseLength);
                } else
                {
                	Log.d("Qt", "Response received from unknown command " + cmd);
                }
            }
        }
    }
    
    
    private void sendCommand( String command )
    {
        TransmitParams params = new TransmitParams();
        params.slotNum        = 0;
        params.controlCode    = -1;
        params.commandString  = command;

        // Transmit APDU
        Log.d("Qt", "Slot " + 0 + ": Transmitting APDU: " + command);
        new TransmitTask().execute(params);
    }
    
    
    /**
     * Converts the HEX string to byte array.
     * 
     * @param hexString
     *            the HEX string.
     * @return the byte array.
     */
    private byte[] toByteArray(String hexString) {

        int hexStringLength = hexString.length();
        byte[] byteArray = null;
        int count = 0;
        char c;
        int i;

        // Count number of hex characters
        for (i = 0; i < hexStringLength; i++) {

            c = hexString.charAt(i);
            if (c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a'
                    && c <= 'f') {
                count++;
            }
        }

        byteArray = new byte[(count + 1) / 2];
        boolean first = true;
        int len = 0;
        int value;
        for (i = 0; i < hexStringLength; i++) {

            c = hexString.charAt(i);
            if (c >= '0' && c <= '9') {
                value = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                value = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                value = c - 'a' + 10;
            } else {
                value = -1;
            }

            if (value >= 0) {

                if (first) {

                    byteArray[len] = (byte) (value << 4);

                } else {

                    byteArray[len] |= value;
                    len++;
                }

                first = !first;
            }
        }

        return byteArray;
    }
    
    
    /**
     * Converts the byte array to HEX string.
     * 
     * @param buffer
     *            the buffer.
     * @return the HEX string.
     */
    private String toHexString(byte[] buffer) {

        String bufferString = "";

        for (int i = 0; i < buffer.length; i++) {

            String hexChar = Integer.toHexString(buffer[i] & 0xFF);
            if (hexChar.length() == 1) {
                hexChar = "0" + hexChar;
            }

            bufferString += hexChar.toUpperCase();
        }

        return bufferString;
    }
    
    
    /**
     * Logs the contents of buffer.
     * 
     * @param buffer
     *            the buffer.
     * @param bufferLength
     *            the buffer length.
     */
    private void logBuffer(byte[] buffer, int bufferLength) {

        String bufferString = "";

        for (int i = 0; i < bufferLength; i++)
        {
            String hexChar = Integer.toHexString(buffer[i] & 0xFF);
            if (hexChar.length() == 1) {
                hexChar = "0" + hexChar;
            }

            if (i % 16 == 0)
            {
                if (bufferString != "")
                {
                	Log.d("Qt", bufferString);
                    bufferString = "";
                }
            }

            bufferString += hexChar.toUpperCase() + " ";
        }

        if (bufferString != "")
        	Log.d("Qt", bufferString);
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////// END NFC DOCKING STATION CODE //////////////////////////    
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    
 
    // this function is used to load and start the loader
    private void loadApplication(Bundle loaderParams)
    {
        try {
            final int errorCode = loaderParams.getInt(ERROR_CODE_KEY);
            if (errorCode != 0) {
                if (errorCode == INCOMPATIBLE_MINISTRO_VERSION) {
                    downloadUpgradeMinistro(loaderParams.getString(ERROR_MESSAGE_KEY));
                    return;
                }

                // fatal error, show the error and quit
                AlertDialog errorDialog = new AlertDialog.Builder(QtActivity.this).create();
                errorDialog.setMessage(loaderParams.getString(ERROR_MESSAGE_KEY));
                errorDialog.setButton(getResources().getString(android.R.string.ok), new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        finish();
                    }
                });
                errorDialog.show();
                return;
            }

            // add all bundled Qt libs to loader params
            ArrayList<String> libs = new ArrayList<String>();
            if ( m_activityInfo.metaData.containsKey("android.app.bundled_libs_resource_id") )
                libs.addAll(Arrays.asList(getResources().getStringArray(m_activityInfo.metaData.getInt("android.app.bundled_libs_resource_id"))));

            String libName = null;
            if ( m_activityInfo.metaData.containsKey("android.app.lib_name") ) {
                libName = m_activityInfo.metaData.getString("android.app.lib_name");
                loaderParams.putString(MAIN_LIBRARY_KEY, libName); //main library contains main() function
            }

            loaderParams.putStringArrayList(BUNDLED_LIBRARIES_KEY, libs);
            loaderParams.putInt(NECESSITAS_API_LEVEL_KEY, NECESSITAS_API_LEVEL);

            // load and start QtLoader class
            m_classLoader = new DexClassLoader(loaderParams.getString(DEX_PATH_KEY), // .jar/.apk files
                                               getDir("outdex", Context.MODE_PRIVATE).getAbsolutePath(), // directory where optimized DEX files should be written.
                                               loaderParams.containsKey(LIB_PATH_KEY) ? loaderParams.getString(LIB_PATH_KEY) : null, // libs folder (if exists)
                                               getClassLoader()); // parent loader

            @SuppressWarnings("rawtypes")
            Class loaderClass = m_classLoader.loadClass(loaderParams.getString(LOADER_CLASS_NAME_KEY)); // load QtLoader class
            Object qtLoader = loaderClass.newInstance(); // create an instance
            Method perpareAppMethod = qtLoader.getClass().getMethod("loadApplication",
                                                                    Activity.class,
                                                                    ClassLoader.class,
                                                                    Bundle.class);
            if (!(Boolean)perpareAppMethod.invoke(qtLoader, this, m_classLoader, loaderParams))
                throw new Exception("");

            QtApplication.setQtActivityDelegate(qtLoader);

            // now load the application library so it's accessible from this class loader
            if (libName != null)
            {
                System.loadLibrary(libName);
                Log.d("Qt", "System.LoadLibrary(libName) called");
            }

            m_mgr = getResources().getAssets();
            SetAssetManager(m_mgr);

            Method startAppMethod=qtLoader.getClass().getMethod("startApplication");
            if (!(Boolean)startAppMethod.invoke(qtLoader))
                throw new Exception("");

        } catch (Exception e) {
            e.printStackTrace();
            AlertDialog errorDialog = new AlertDialog.Builder(QtActivity.this).create();
            if (m_activityInfo.metaData.containsKey("android.app.fatal_error_msg"))
                errorDialog.setMessage(m_activityInfo.metaData.getString("android.app.fatal_error_msg"));
            else
                errorDialog.setMessage("Fatal error, your application can't be started.");

            errorDialog.setButton(getResources().getString(android.R.string.ok), new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    finish();
                }
            });
            errorDialog.show();
        }
    }

    private ServiceConnection m_ministroConnection=new ServiceConnection() {
        private IMinistro m_service = null;
    @Override
        public void onServiceConnected(ComponentName name, IBinder service)
        {
            m_service = IMinistro.Stub.asInterface(service);
            try {
                if (m_service!=null) {
                    Bundle parameters= new Bundle();
                    parameters.putStringArray(REQUIRED_MODULES_KEY, m_qtLibs);
                    parameters.putString(APPLICATION_TITLE_KEY, (String)QtActivity.this.getTitle());
                    parameters.putInt(MINIMUM_MINISTRO_API_KEY, MINISTRO_API_LEVEL);
                    parameters.putInt(MINIMUM_QT_VERSION_KEY, QT_VERSION);
                    parameters.putString(ENVIRONMENT_VARIABLES_KEY, ENVIRONMENT_VARIABLES);
                    if (null!=APPLICATION_PARAMETERS)
                        parameters.putString(APPLICATION_PARAMETERS_KEY, APPLICATION_PARAMETERS);
                    parameters.putStringArray(SOURCES_KEY, m_sources);
                    parameters.putString(REPOSITORY_KEY, m_repository);
                    m_service.requestLoader(m_ministroCallback, parameters);
                }
            } catch (RemoteException e) {
                    e.printStackTrace();
            }
        }

    private IMinistroCallback m_ministroCallback = new IMinistroCallback.Stub() {
        // this function is called back by Ministro.
        @Override
        public void loaderReady(final Bundle loaderParams) throws RemoteException {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    unbindService(m_ministroConnection);
                    loadApplication(loaderParams);
                }
            });
        }
    };

        @Override
        public void onServiceDisconnected(ComponentName name) {
            m_service = null;
        }
    };

    private void downloadUpgradeMinistro(String msg)
    {
        AlertDialog.Builder downloadDialog = new AlertDialog.Builder(this);
        downloadDialog.setMessage(msg);
        downloadDialog.setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                try {
                    Uri uri = Uri.parse("market://search?q=pname:org.kde.necessitas.ministro");
                    Intent intent = new Intent(Intent.ACTION_VIEW, uri);
                    startActivityForResult(intent, MINISTRO_INSTALL_REQUEST_CODE);
                } catch (Exception e) {
                    e.printStackTrace();
                    ministroNotFound();
                }
            }
        });

        downloadDialog.setNegativeButton(android.R.string.no, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                QtActivity.this.finish();
            }
        });
        downloadDialog.show();
    }

    private void ministroNotFound()
    {
        AlertDialog errorDialog = new AlertDialog.Builder(QtActivity.this).create();

        if (m_activityInfo.metaData.containsKey("android.app.ministro_not_found_msg"))
            errorDialog.setMessage(m_activityInfo.metaData.getString("android.app.ministro_not_found_msg"));
        else
            errorDialog.setMessage("Can't find Ministro service.\nThe application can't start.");

        errorDialog.setButton(getResources().getString(android.R.string.ok), new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                finish();
            }
        });
        errorDialog.show();
    }

    static private void copyFile(InputStream inputStream, OutputStream outputStream)
        throws IOException
    {
        byte[] buffer = new byte[BUFFER_SIZE];

        int count;
        while ((count = inputStream.read(buffer)) > 0)
            outputStream.write(buffer, 0, count);
    }


    private void copyAsset(String source, String destination)
        throws IOException
    {
        // Already exists, we don't have to do anything
        File destinationFile = new File(destination);
        if (destinationFile.exists())
            return;

        File parentDirectory = destinationFile.getParentFile();
        if (!parentDirectory.exists())
            parentDirectory.mkdirs();

        destinationFile.createNewFile();

        AssetManager assetsManager = getAssets();
        InputStream inputStream = assetsManager.open(source);
        OutputStream outputStream = new FileOutputStream(destinationFile);
        copyFile(inputStream, outputStream);
    }

    private static void createBundledBinary(String source, String destination)
        throws IOException
    {
        // Already exists, we don't have to do anything
        File destinationFile = new File(destination);
        if (destinationFile.exists())
            return;

        File parentDirectory = destinationFile.getParentFile();
        if (!parentDirectory.exists())
            parentDirectory.mkdirs();

        destinationFile.createNewFile();

        InputStream inputStream = new FileInputStream(source);
        OutputStream outputStream = new FileOutputStream(destinationFile);
        copyFile(inputStream, outputStream);
    }

    private void extractBundledPluginsAndImports(String localPrefix)
        throws IOException
    {
        ArrayList<String> libs = new ArrayList<String>();

        {
            String key = BUNDLED_IN_LIB_RESOURCE_ID_KEY;
            java.util.Set<String> keys = m_activityInfo.metaData.keySet();
            if (m_activityInfo.metaData.containsKey(key)) {
                String[] list = getResources().getStringArray(m_activityInfo.metaData.getInt(key));

                for (String bundledImportBinary : list) {
                    String[] split = bundledImportBinary.split(":");
                    String sourceFileName = localPrefix + "lib/" + split[0];
                    String destinationFileName = localPrefix + split[1];
                    createBundledBinary(sourceFileName, destinationFileName);
                }
            }
        }

        {
            String key = BUNDLED_IN_ASSETS_RESOURCE_ID_KEY;
            if (m_activityInfo.metaData.containsKey(key)) {
                String[] list = getResources().getStringArray(m_activityInfo.metaData.getInt(key));

                for (String fileName : list) {
                    String[] split = fileName.split(":");
                    String sourceFileName = split[0];
                    String destinationFileName = localPrefix + split[1];
                    copyAsset(sourceFileName, destinationFileName);
                }
            }

        }
    }

    private void startApp(final boolean firstStart)
    {
        try {
            if (m_activityInfo.metaData.containsKey("android.app.qt_sources_resource_id")) {
                int resourceId = m_activityInfo.metaData.getInt("android.app.qt_sources_resource_id");
                m_sources = getResources().getStringArray(resourceId);
            }

            if (m_activityInfo.metaData.containsKey("android.app.repository"))
                m_repository = m_activityInfo.metaData.getString("android.app.repository");

            if (m_activityInfo.metaData.containsKey("android.app.qt_libs_resource_id")) {
                int resourceId = m_activityInfo.metaData.getInt("android.app.qt_libs_resource_id");
                m_qtLibs = getResources().getStringArray(resourceId);
            }

            if (m_activityInfo.metaData.containsKey("android.app.use_local_qt_libs")
                    && m_activityInfo.metaData.getInt("android.app.use_local_qt_libs") == 1) {
                ArrayList<String> libraryList = new ArrayList<String>();


                String localPrefix = "/data/local/tmp/qt/";
                if (m_activityInfo.metaData.containsKey("android.app.libs_prefix"))
                    localPrefix = m_activityInfo.metaData.getString("android.app.libs_prefix");

                boolean bundlingQtLibs = false;
                if (m_activityInfo.metaData.containsKey("android.app.bundle_local_qt_libs")
                    && m_activityInfo.metaData.getInt("android.app.bundle_local_qt_libs") == 1) {
                    localPrefix = getApplicationInfo().dataDir + "/";
                    extractBundledPluginsAndImports(localPrefix);
                    bundlingQtLibs = true;
                }

                if (m_qtLibs != null) {
                    for (int i=0;i<m_qtLibs.length;i++) {
                        libraryList.add(localPrefix
                                        + "lib/lib"
                                        + m_qtLibs[i]
                                        + ".so");
                    }
                }

                if (m_activityInfo.metaData.containsKey("android.app.load_local_libs")) {
                    String[] extraLibs = m_activityInfo.metaData.getString("android.app.load_local_libs").split(":");
                    for (String lib : extraLibs) {
                        if (lib.length() > 0)
                            libraryList.add(localPrefix + lib);
                    }
                }


                String dexPaths = new String();
                String pathSeparator = System.getProperty("path.separator", ":");
                if (!bundlingQtLibs && m_activityInfo.metaData.containsKey("android.app.load_local_jars")) {
                    String[] jarFiles = m_activityInfo.metaData.getString("android.app.load_local_jars").split(":");
                    for (String jar:jarFiles) {
                        if (jar.length() > 0) {
                            if (dexPaths.length() > 0)
                                dexPaths += pathSeparator;
                            dexPaths += localPrefix + jar;
                        }
                    }
                }

                Bundle loaderParams = new Bundle();
                loaderParams.putInt(ERROR_CODE_KEY, 0);
                loaderParams.putString(DEX_PATH_KEY, dexPaths);
                loaderParams.putString(LOADER_CLASS_NAME_KEY, "org.qtproject.qt5.android.QtActivityDelegate");
                if (m_activityInfo.metaData.containsKey("android.app.static_init_classes")) {
                    loaderParams.putStringArray(STATIC_INIT_CLASSES_KEY,
                                                m_activityInfo.metaData.getString("android.app.static_init_classes").split(":"));
                }
                loaderParams.putStringArrayList(NATIVE_LIBRARIES_KEY, libraryList);
                loaderParams.putString(ENVIRONMENT_VARIABLES_KEY, ENVIRONMENT_VARIABLES
                                                                  + "\tQML2_IMPORT_PATH=" + localPrefix + "/qml"
                                                                  + "\tQML_IMPORT_PATH=" + localPrefix + "/imports"
                                                                  + "\tQT_PLUGIN_PATH=" + localPrefix + "/plugins");
                loadApplication(loaderParams);
                return;
            }

            try {
                if (!bindService(new Intent(org.kde.necessitas.ministro.IMinistro.class.getCanonicalName()),
                                 m_ministroConnection,
                                 Context.BIND_AUTO_CREATE)) {
                    throw new SecurityException("");
                }
            } catch (Exception e) {
                if (firstStart) {
                    String msg = "This application requires Ministro service. Would you like to install it?";
                    if (m_activityInfo.metaData.containsKey("android.app.ministro_needed_msg"))
                        msg = m_activityInfo.metaData.getString("android.app.ministro_needed_msg");
                    downloadUpgradeMinistro(msg);
                } else {
                    ministroNotFound();
                }
            }
        } catch (Exception e) {
            Log.e(QtApplication.QtTAG, "Can't create main activity", e);
        }
    }



    /////////////////////////// forward all notifications ////////////////////////////
    /////////////////////////// Super class calls ////////////////////////////////////
    /////////////// PLEASE DO NOT CHANGE THE FOLLOWING CODE //////////////////////////
    //////////////////////////////////////////////////////////////////////////////////

    @Override
    public boolean dispatchKeyEvent(KeyEvent event)
    {
        if (QtApplication.m_delegateObject != null && QtApplication.dispatchKeyEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.dispatchKeyEvent, event);
        else
            return super.dispatchKeyEvent(event);
    }
    public boolean super_dispatchKeyEvent(KeyEvent event)
    {
        return super.dispatchKeyEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean dispatchPopulateAccessibilityEvent(AccessibilityEvent event)
    {
        if (QtApplication.m_delegateObject != null && QtApplication.dispatchPopulateAccessibilityEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.dispatchPopulateAccessibilityEvent, event);
        else
            return super.dispatchPopulateAccessibilityEvent(event);
    }
    public boolean super_dispatchPopulateAccessibilityEvent(AccessibilityEvent event)
    {
        return super_dispatchPopulateAccessibilityEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev)
    {
        if (QtApplication.m_delegateObject != null && QtApplication.dispatchTouchEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.dispatchTouchEvent, ev);
        else
            return super.dispatchTouchEvent(ev);
    }
    public boolean super_dispatchTouchEvent(MotionEvent event)
    {
        return super.dispatchTouchEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean dispatchTrackballEvent(MotionEvent ev)
    {
        if (QtApplication.m_delegateObject != null && QtApplication.dispatchTrackballEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.dispatchTrackballEvent, ev);
        else
            return super.dispatchTrackballEvent(ev);
    }
    public boolean super_dispatchTrackballEvent(MotionEvent event)
    {
        return super.dispatchTrackballEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {

        if (QtApplication.m_delegateObject != null && QtApplication.onActivityResult != null) {
            QtApplication.invokeDelegateMethod(QtApplication.onActivityResult, requestCode, resultCode, data);
            return;
        }
        if (requestCode == MINISTRO_INSTALL_REQUEST_CODE)
            startApp(false);
        super.onActivityResult(requestCode, resultCode, data);
    }
    public void super_onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onApplyThemeResource(Theme theme, int resid, boolean first)
    {
        if (!QtApplication.invokeDelegate(theme, resid, first).invoked)
            super.onApplyThemeResource(theme, resid, first);
    }
    public void super_onApplyThemeResource(Theme theme, int resid, boolean first)
    {
        super.onApplyThemeResource(theme, resid, first);
    }
    //---------------------------------------------------------------------------


    @Override
    protected void onChildTitleChanged(Activity childActivity, CharSequence title)
    {
        if (!QtApplication.invokeDelegate(childActivity, title).invoked)
            super.onChildTitleChanged(childActivity, title);
    }
    public void super_onChildTitleChanged(Activity childActivity, CharSequence title)
    {
        super.onChildTitleChanged(childActivity, title);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onConfigurationChanged(Configuration newConfig)
    {
        if (!QtApplication.invokeDelegate(newConfig).invoked)
            super.onConfigurationChanged(newConfig);
    }
    public void super_onConfigurationChanged(Configuration newConfig)
    {
        super.onConfigurationChanged(newConfig);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onContentChanged()
    {
        if (!QtApplication.invokeDelegate().invoked)
            super.onContentChanged();
    }
    public void super_onContentChanged()
    {
        super.onContentChanged();
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onContextItemSelected(MenuItem item)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(item);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onContextItemSelected(item);
    }
    public boolean super_onContextItemSelected(MenuItem item)
    {
        return super.onContextItemSelected(item);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onContextMenuClosed(Menu menu)
    {
        if (!QtApplication.invokeDelegate(menu).invoked)
            super.onContextMenuClosed(menu);
    }
    public void super_onContextMenuClosed(Menu menu)
    {
        super.onContextMenuClosed(menu);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        if (QtApplication.m_delegateObject != null && QtApplication.onCreate != null) {
            QtApplication.invokeDelegateMethod(QtApplication.onCreate, savedInstanceState);
            return;
        }

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        try {
            m_activityInfo = getPackageManager().getActivityInfo(getComponentName(), PackageManager.GET_META_DATA);
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            finish();
            return;
        }

        if (null == getLastNonConfigurationInstance()) {
            // if splash screen is defined, then show it
            if (m_activityInfo.metaData.containsKey("android.app.splash_screen") )
                setContentView(m_activityInfo.metaData.getInt("android.app.splash_screen"));
            startApp(true);
        }
        
        
        // NFC docking station code
    	// Get USB manager
        mManager = (UsbManager) getSystemService(Context.USB_SERVICE);

        // Initialize reader
        mReader = new Reader(mManager);
        mReader.setOnStateChangeListener(new OnStateChangeListener()
        {
            @Override
            public void onStateChange(int slotNum, int prevState, int currState)
            {

                if (prevState < Reader.CARD_UNKNOWN
                        || prevState > Reader.CARD_SPECIFIC)
                {
                    prevState = Reader.CARD_UNKNOWN;
                }

                if (currState < Reader.CARD_UNKNOWN
                        || currState > Reader.CARD_SPECIFIC)
                {
                    currState = Reader.CARD_UNKNOWN;
                }
                
                /*
                     private static final String[] stateStrings = { "Unknown", "Absent",
            					"Present", "Swallowed", "Powered", "Negotiable", "Specific" };
                */
                
                if ( currState == 2 )			//card present
                {
                	Log.d("Qt", "Powering card...");
                	
                	// Set parameters
                    PowerParams params = new PowerParams();
                    params.slotNum = 0;
                    params.action  = Reader.CARD_WARM_RESET;

                    // Perform power action
                    new PowerTask().execute(params);
                }

                // Create output string
                final String outputString = "Slot " + slotNum + ": "
                        + stateStrings[prevState] + " -> "
                        + stateStrings[currState];
                
                Log.d("Qt", outputString);
            }
        });
        
        if ( mReceiver != null )
        {
        	Log.d("Qt", "mReceiver initialized");
        } else
        {
        	Log.d("Qt", "mReceiver not initialized");
        }
        
        // Register receiver for USB permission
        mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        IntentFilter filter = new IntentFilter();
        filter.addAction(ACTION_USB_PERMISSION);
        filter.addAction(UsbManager.ACTION_USB_DEVICE_DETACHED);
        registerReceiver(mReceiver, filter);
        
        
        // For each device
        for (UsbDevice device : mManager.getDeviceList().values())
        {
        	if (mReader.isSupported(device)) {
        		mManager.requestPermission(device,
                        mPermissionIntent);
                break;
            }
        }
        
        
        Log.d("Qt", "OnCreate executed");
    }
    //---------------------------------------------------------------------------

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo)
    {
        if (!QtApplication.invokeDelegate(menu, v, menuInfo).invoked)
            super.onCreateContextMenu(menu, v, menuInfo);
    }
    public void super_onCreateContextMenu(ContextMenu menu, View v, ContextMenuInfo menuInfo)
    {
        super.onCreateContextMenu(menu, v, menuInfo);
    }
    //---------------------------------------------------------------------------

    @Override
    public CharSequence onCreateDescription()
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate();
        if (res.invoked)
            return (CharSequence)res.methodReturns;
        else
            return super.onCreateDescription();
    }
    public CharSequence super_onCreateDescription()
    {
        return super.onCreateDescription();
    }
    //---------------------------------------------------------------------------

    @Override
    protected Dialog onCreateDialog(int id)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(id);
        if (res.invoked)
            return (Dialog)res.methodReturns;
        else
            return super.onCreateDialog(id);
    }
    public Dialog super_onCreateDialog(int id)
    {
        return super.onCreateDialog(id);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(menu);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onCreateOptionsMenu(menu);
    }
    public boolean super_onCreateOptionsMenu(Menu menu)
    {
        return super.onCreateOptionsMenu(menu);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onCreatePanelMenu(int featureId, Menu menu)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(featureId, menu);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onCreatePanelMenu(featureId, menu);
    }
    public boolean super_onCreatePanelMenu(int featureId, Menu menu)
    {
        return super.onCreatePanelMenu(featureId, menu);
    }
    //---------------------------------------------------------------------------


    @Override
    public View onCreatePanelView(int featureId)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(featureId);
        if (res.invoked)
            return (View)res.methodReturns;
        else
            return super.onCreatePanelView(featureId);
    }
    public View super_onCreatePanelView(int featureId)
    {
        return super.onCreatePanelView(featureId);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onCreateThumbnail(Bitmap outBitmap, Canvas canvas)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(outBitmap, canvas);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onCreateThumbnail(outBitmap, canvas);
    }
    public boolean super_onCreateThumbnail(Bitmap outBitmap, Canvas canvas)
    {
        return super.onCreateThumbnail(outBitmap, canvas);
    }
    //---------------------------------------------------------------------------

    @Override
    public View onCreateView(String name, Context context, AttributeSet attrs)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(name, context, attrs);
        if (res.invoked)
            return (View)res.methodReturns;
        else
            return super.onCreateView(name, context, attrs);
    }
    public View super_onCreateView(String name, Context context, AttributeSet attrs)
    {
        return super.onCreateView(name, context, attrs);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onDestroy()
    {
        super.onDestroy();
        QtApplication.invokeDelegate();
    }
    //---------------------------------------------------------------------------


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        if (QtApplication.m_delegateObject != null && QtApplication.onKeyDown != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onKeyDown, keyCode, event);
        else
            return super.onKeyDown(keyCode, event);
    }
    public boolean super_onKeyDown(int keyCode, KeyEvent event)
    {
        return super.onKeyDown(keyCode, event);
    }
    //---------------------------------------------------------------------------


    @Override
    public boolean onKeyMultiple(int keyCode, int repeatCount, KeyEvent event)
    {
        if (QtApplication.m_delegateObject != null && QtApplication.onKeyMultiple != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onKeyMultiple, keyCode, repeatCount, event);
        else
            return super.onKeyMultiple(keyCode, repeatCount, event);
    }
    public boolean super_onKeyMultiple(int keyCode, int repeatCount, KeyEvent event)
    {
        return super.onKeyMultiple(keyCode, repeatCount, event);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.onKeyDown != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onKeyUp, keyCode, event);
        else
            return super.onKeyUp(keyCode, event);
    }
    public boolean super_onKeyUp(int keyCode, KeyEvent event)
    {
        return super.onKeyUp(keyCode, event);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onLowMemory()
    {
        if (!QtApplication.invokeDelegate().invoked)
            super.onLowMemory();
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onMenuItemSelected(int featureId, MenuItem item)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(featureId, item);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onMenuItemSelected(featureId, item);
    }
    public boolean super_onMenuItemSelected(int featureId, MenuItem item)
    {
        return super.onMenuItemSelected(featureId, item);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onMenuOpened(int featureId, Menu menu)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(featureId, menu);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onMenuOpened(featureId, menu);
    }
    public boolean super_onMenuOpened(int featureId, Menu menu)
    {
        return super.onMenuOpened(featureId, menu);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onNewIntent(Intent intent)
    {
        if (!QtApplication.invokeDelegate(intent).invoked)
            super.onNewIntent(intent);
    }
    public void super_onNewIntent(Intent intent)
    {
        super.onNewIntent(intent);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(item);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onOptionsItemSelected(item);
    }
    public boolean super_onOptionsItemSelected(MenuItem item)
    {
        return super.onOptionsItemSelected(item);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onOptionsMenuClosed(Menu menu)
    {
        if (!QtApplication.invokeDelegate(menu).invoked)
            super.onOptionsMenuClosed(menu);
    }
    public void super_onOptionsMenuClosed(Menu menu)
    {
        super.onOptionsMenuClosed(menu);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onPanelClosed(int featureId, Menu menu)
    {
        if (!QtApplication.invokeDelegate(featureId, menu).invoked)
            super.onPanelClosed(featureId, menu);
    }
    public void super_onPanelClosed(int featureId, Menu menu)
    {
        super.onPanelClosed(featureId, menu);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onPause()
    {
        super.onPause();
        QtApplication.invokeDelegate();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onPostCreate(Bundle savedInstanceState)
    {
        super.onPostCreate(savedInstanceState);
        QtApplication.invokeDelegate(savedInstanceState);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onPostResume()
    {
        super.onPostResume();
        QtApplication.invokeDelegate();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onPrepareDialog(int id, Dialog dialog)
    {
        if (!QtApplication.invokeDelegate(id, dialog).invoked)
            super.onPrepareDialog(id, dialog);
    }
    public void super_onPrepareDialog(int id, Dialog dialog)
    {
        super.onPrepareDialog(id, dialog);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onPrepareOptionsMenu(Menu menu)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(menu);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onPrepareOptionsMenu(menu);
    }
    public boolean super_onPrepareOptionsMenu(Menu menu)
    {
        return super.onPrepareOptionsMenu(menu);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onPreparePanel(int featureId, View view, Menu menu)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(featureId, view, menu);
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onPreparePanel(featureId, view, menu);
    }
    public boolean super_onPreparePanel(int featureId, View view, Menu menu)
    {
        return super.onPreparePanel(featureId, view, menu);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onRestart()
    {
        super.onRestart();
        QtApplication.invokeDelegate();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState)
    {
        if (!QtApplication.invokeDelegate(savedInstanceState).invoked)
            super.onRestoreInstanceState(savedInstanceState);
    }
    public void super_onRestoreInstanceState(Bundle savedInstanceState)
    {
        super.onRestoreInstanceState(savedInstanceState);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onResume()
    {
        super.onResume();
        QtApplication.invokeDelegate();
    }
    //---------------------------------------------------------------------------

    @Override
    public Object onRetainNonConfigurationInstance()
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate();
        if (res.invoked)
            return res.methodReturns;
        else
            return super.onRetainNonConfigurationInstance();
    }
    public Object super_onRetainNonConfigurationInstance()
    {
        return super.onRetainNonConfigurationInstance();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onSaveInstanceState(Bundle outState)
    {
        if (!QtApplication.invokeDelegate(outState).invoked)
            super.onSaveInstanceState(outState);
    }
    public void super_onSaveInstanceState(Bundle outState)
    {
        super.onSaveInstanceState(outState);

    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onSearchRequested()
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate();
        if (res.invoked)
            return (Boolean)res.methodReturns;
        else
            return super.onSearchRequested();
    }
    public boolean super_onSearchRequested()
    {
        return super.onSearchRequested();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onStart()
    {
        super.onStart();
        QtApplication.invokeDelegate();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onStop()
    {
        super.onStop();
        QtApplication.invokeDelegate();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onTitleChanged(CharSequence title, int color)
    {
        if (!QtApplication.invokeDelegate(title, color).invoked)
            super.onTitleChanged(title, color);
    }
    public void super_onTitleChanged(CharSequence title, int color)
    {
        super.onTitleChanged(title, color);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.onTouchEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onTouchEvent, event);
        else
            return super.onTouchEvent(event);
    }
    public boolean super_onTouchEvent(MotionEvent event)
    {
        return super.onTouchEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onTrackballEvent(MotionEvent event)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.onTrackballEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onTrackballEvent, event);
        else
            return super.onTrackballEvent(event);
    }
    public boolean super_onTrackballEvent(MotionEvent event)
    {
        return super.onTrackballEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onUserInteraction()
    {
        if (!QtApplication.invokeDelegate().invoked)
            super.onUserInteraction();
    }
    public void super_onUserInteraction()
    {
        super.onUserInteraction();
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onUserLeaveHint()
    {
        if (!QtApplication.invokeDelegate().invoked)
            super.onUserLeaveHint();
    }
    public void super_onUserLeaveHint()
    {
        super.onUserLeaveHint();
    }
    //---------------------------------------------------------------------------

    @Override
    public void onWindowAttributesChanged(LayoutParams params)
    {
        if (!QtApplication.invokeDelegate(params).invoked)
            super.onWindowAttributesChanged(params);
    }
    public void super_onWindowAttributesChanged(LayoutParams params)
    {
        super.onWindowAttributesChanged(params);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onWindowFocusChanged(boolean hasFocus)
    {
        if (!QtApplication.invokeDelegate(hasFocus).invoked)
            super.onWindowFocusChanged(hasFocus);
    }
    public void super_onWindowFocusChanged(boolean hasFocus)
    {
        super.onWindowFocusChanged(hasFocus);
    }
    //---------------------------------------------------------------------------

    //////////////// Activity API 5 /////////////
//@ANDROID-5
    @Override
    public void onAttachedToWindow()
    {
        if (!QtApplication.invokeDelegate().invoked)
            super.onAttachedToWindow();
    }
    public void super_onAttachedToWindow()
    {
        super.onAttachedToWindow();
    }
    //---------------------------------------------------------------------------

    @Override
    public void onBackPressed()
    {
        if (!QtApplication.invokeDelegate().invoked)
            super.onBackPressed();
    }
    public void super_onBackPressed()
    {
        super.onBackPressed();
    }
    //---------------------------------------------------------------------------

    @Override
    public void onDetachedFromWindow()
    {
        if (!QtApplication.invokeDelegate().invoked)
            super.onDetachedFromWindow();
    }
    public void super_onDetachedFromWindow()
    {
        super.onDetachedFromWindow();
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.onKeyLongPress != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onKeyLongPress, keyCode, event);
        else
            return super.onKeyLongPress(keyCode, event);
    }
    public boolean super_onKeyLongPress(int keyCode, KeyEvent event)
    {
        return super.onKeyLongPress(keyCode, event);
    }
    //---------------------------------------------------------------------------
//@ANDROID-5

//////////////// Activity API 8 /////////////
//@ANDROID-8
@Override
    protected Dialog onCreateDialog(int id, Bundle args)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(id, args);
        if (res.invoked)
            return (Dialog)res.methodReturns;
        else
            return super.onCreateDialog(id, args);
    }
    public Dialog super_onCreateDialog(int id, Bundle args)
    {
        return super.onCreateDialog(id, args);
    }
    //---------------------------------------------------------------------------

    @Override
    protected void onPrepareDialog(int id, Dialog dialog, Bundle args)
    {
        if (!QtApplication.invokeDelegate(id, dialog, args).invoked)
            super.onPrepareDialog(id, dialog, args);
    }
    public void super_onPrepareDialog(int id, Dialog dialog, Bundle args)
    {
        super.onPrepareDialog(id, dialog, args);
    }
    //---------------------------------------------------------------------------
//@ANDROID-8
    //////////////// Activity API 11 /////////////

//@ANDROID-11
    @Override
    public boolean dispatchKeyShortcutEvent(KeyEvent event)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.dispatchKeyShortcutEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.dispatchKeyShortcutEvent, event);
        else
            return super.dispatchKeyShortcutEvent(event);
    }
    public boolean super_dispatchKeyShortcutEvent(KeyEvent event)
    {
        return super.dispatchKeyShortcutEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onActionModeFinished(ActionMode mode)
    {
        if (!QtApplication.invokeDelegate(mode).invoked)
            super.onActionModeFinished(mode);
    }
    public void super_onActionModeFinished(ActionMode mode)
    {
        super.onActionModeFinished(mode);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onActionModeStarted(ActionMode mode)
    {
        if (!QtApplication.invokeDelegate(mode).invoked)
            super.onActionModeStarted(mode);
    }
    public void super_onActionModeStarted(ActionMode mode)
    {
        super.onActionModeStarted(mode);
    }
    //---------------------------------------------------------------------------

    @Override
    public void onAttachFragment(Fragment fragment)
    {
        if (!QtApplication.invokeDelegate(fragment).invoked)
            super.onAttachFragment(fragment);
    }
    public void super_onAttachFragment(Fragment fragment)
    {
        super.onAttachFragment(fragment);
    }
    //---------------------------------------------------------------------------

    @Override
    public View onCreateView(View parent, String name, Context context, AttributeSet attrs)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(parent, name, context, attrs);
        if (res.invoked)
            return (View)res.methodReturns;
        else
            return super.onCreateView(parent, name, context, attrs);
    }
    public View super_onCreateView(View parent, String name, Context context,
            AttributeSet attrs) {
        return super.onCreateView(parent, name, context, attrs);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onKeyShortcut(int keyCode, KeyEvent event)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.onKeyShortcut != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onKeyShortcut, keyCode,event);
        else
            return super.onKeyShortcut(keyCode, event);
    }
    public boolean super_onKeyShortcut(int keyCode, KeyEvent event)
    {
        return super.onKeyShortcut(keyCode, event);
    }
    //---------------------------------------------------------------------------

    @Override
    public ActionMode onWindowStartingActionMode(Callback callback)
    {
        QtApplication.InvokeResult res = QtApplication.invokeDelegate(callback);
        if (res.invoked)
            return (ActionMode)res.methodReturns;
        else
            return super.onWindowStartingActionMode(callback);
    }
    public ActionMode super_onWindowStartingActionMode(Callback callback)
    {
        return super.onWindowStartingActionMode(callback);
    }
    //---------------------------------------------------------------------------
//@ANDROID-11
    //////////////// Activity API 12 /////////////

//@ANDROID-12
    @Override
    public boolean dispatchGenericMotionEvent(MotionEvent ev)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.dispatchGenericMotionEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.dispatchGenericMotionEvent, ev);
        else
            return super.dispatchGenericMotionEvent(ev);
    }
    public boolean super_dispatchGenericMotionEvent(MotionEvent event)
    {
        return super.dispatchGenericMotionEvent(event);
    }
    //---------------------------------------------------------------------------

    @Override
    public boolean onGenericMotionEvent(MotionEvent event)
    {
        if (QtApplication.m_delegateObject != null  && QtApplication.onGenericMotionEvent != null)
            return (Boolean) QtApplication.invokeDelegateMethod(QtApplication.onGenericMotionEvent, event);
        else
            return super.onGenericMotionEvent(event);
    }
    public boolean super_onGenericMotionEvent(MotionEvent event)
    {
        return super.onGenericMotionEvent(event);
    }
    //---------------------------------------------------------------------------
//@ANDROID-12
}
