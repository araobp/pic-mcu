package jp.araobp.mpu9250

import android.view.View
import android.view.Window

fun fullscreen(window: Window) {
    window.decorView.systemUiVisibility =
        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_FULLSCREEN or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
}
