package jp.araobp.mpu9250

import android.content.Context
import kotlin.math.roundToInt

class Properties(val context: Context) {

    companion object {
        const val PREFS_NAME = "mpu9250"
    }

    var baudrate = 115200

    init {
        load()
    }

    fun load() {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        baudrate = prefs.getInt("baudrate", 115200)
    }

    fun save() {
        val editor = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit()
        editor.putInt("baudrate", baudrate)
        editor.apply()
    }
}