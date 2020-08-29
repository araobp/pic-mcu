package jp.araobp.mpu9250

import android.content.Context
import jp.araobp.mpu9250.serial.Mpu9250Interface

class Properties(val context: Context) {

    companion object {
        const val PREFS_NAME = "mpu9250"
    }

    var baudrate = 115200
    var accelRange = Mpu9250Interface.AccelRange.G_2
    var gyroRange = Mpu9250Interface.GyroRange.DPS_250
    var magnetoMagnify = 1
    var defaultClassLabels = "stand_up,sit_down"
    var classLabels = defaultClassLabels.split(",").toMutableList()

    init {
        load()
    }

    private fun load() {
        val prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)

        baudrate = prefs.getInt("baudrate", 115200)

        var _accelRange = prefs.getString("accelRange", Mpu9250Interface.AccelRange.G_2.toString())
        _accelRange?.let {
            accelRange = Mpu9250Interface.AccelRange.valueOf(it)
        }

        var _gyroRange = prefs.getString("gyroRange", Mpu9250Interface.GyroRange.DPS_250.toString())
        _gyroRange?.let {
            gyroRange = Mpu9250Interface.GyroRange.valueOf(it)
        }
        magnetoMagnify = prefs.getInt("magnetoMagnify", 1)

        classLabels = prefs.getString("classLabel", defaultClassLabels)!!.split(",").toMutableList()
    }

    fun save() {
        val editor = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE).edit()
        editor.putInt("baudrate", baudrate)
        editor.putString("accelRange", accelRange.toString())
        editor.putString("gyroRange", gyroRange.toString())
        editor.putInt("magnetoMagnify", magnetoMagnify)
        editor.putString("classLabels", classLabels.joinToString(","))
        editor.apply()
    }
}