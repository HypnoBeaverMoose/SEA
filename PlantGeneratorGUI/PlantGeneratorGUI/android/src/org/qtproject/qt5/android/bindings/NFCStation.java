package org.qtproject.qt5.android.bindings;

import android.util.Log;

public final class NFCStation
{
    public NFCStation()
    {
        Log.v("Qt", "NFCStation created!");
    }

    public static native void processPlants( int p1, int p2, int p3 );
    public static native void setLabelText( String text );
};
