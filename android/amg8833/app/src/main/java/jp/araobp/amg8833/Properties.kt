package jp.araobp.amg8833

import android.content.Context

class Properties(val context: Context) {

    companion object {
        const val PREFS_NAME = "amg8833"
    }

    var dstId = 0

    init {
        load()
    }

    fun load() {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        dstId = prefs.getInt("dstId", 0)
    }

    fun save() {
        val editor = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit()
        editor.putInt("dstId", dstId)
        editor.apply()
    }
}