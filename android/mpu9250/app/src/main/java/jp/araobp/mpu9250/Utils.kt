package jp.araobp.mpu9250

import android.content.Context
import android.net.wifi.WifiManager
import android.view.View
import android.view.Window
import java.math.BigInteger
import java.net.InetAddress

fun fullscreen(window: Window) {
    window.decorView.systemUiVisibility =
        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_FULLSCREEN or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
}

// [Reference] https://stackoverflow.com/questions/20846120/alternative-for-formatter-formatipaddressint
fun ipAddress(context: Context): String {
    val wifiManager =
        context.applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
    val wifiinfo = wifiManager.connectionInfo
    val ipAddress = BigInteger.valueOf(wifiinfo.ipAddress.toLong()).toByteArray()
    return InetAddress.getByAddress(ipAddress.reversedArray()).hostAddress
}