package jp.araobp.mpu9250.analyzer

import android.content.Context
import jp.araobp.mpu9250.serial.Mpu9250Data
import jp.araobp.mpu9250.serial.Mpu9250Interface
import java.io.File
import java.io.FileWriter
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter

class FeatureCollector(val context: Context) {

    companion object {
        const val header = "sno,ax,ay,az,gx,gy,gz\r\n"
    }

    private var mMpu9250DataArray =  ArrayList<Mpu9250Data>()

    private var mClassLabel = "Unknown"
    private var mGo = false
    private var mNumEntries = 0
    private var mAccelRange = Mpu9250Interface.AccelRange.G_2.toString()
    private var mGyroRange = Mpu9250Interface.GyroRange.DPS_250.toString()

    private val regex = Regex("[a-zA-Z0-9_\\-]+.csv")
    private fun regexFeature(classLabel: String) = Regex("${classLabel}-[a-zA-Z0-9_\\-]+.csv")

    fun fileCnt(): Int {
        var cnt = 0
        context.filesDir.list().forEach {
            if (regex.matches(it)) ++cnt
        }
        return cnt
    }

    fun fileCntPerLabel(classLabel: String): Int {
        var cnt = 0
        context.filesDir.list().forEach { if (regexFeature(classLabel).matches(it)) ++cnt }
        return cnt
    }

    private fun dateString(): String {
        val current = LocalDateTime.now()
        val formatter = DateTimeFormatter.ofPattern("yyyyMMdd")
        return current.format(formatter)
    }

    private fun save(): Int {
        var cnt = fileCntPerLabel(mClassLabel)
        val fileName = "${dateString()}:${mClassLabel}:${mAccelRange}:${mGyroRange}:$cnt.csv"
        val file = File(context.filesDir, fileName)
        val fileWriter = FileWriter(file)
        fileWriter.append(header)
        mMpu9250DataArray.forEach {
            fileWriter.append(it.toCsv())
        }
        fileWriter.close()
        return ++cnt
    }

    fun getReady(classLabel: String, numEntries: Int, accelRange: Mpu9250Interface.AccelRange, gyroRange: Mpu9250Interface.GyroRange) {
        mMpu9250DataArray.clear()
        mClassLabel = classLabel
        mNumEntries = numEntries
        mAccelRange = accelRange.toString()
        mGyroRange = gyroRange.toString()
        mGo = true
    }

    fun add(data: Mpu9250Data): Boolean {
        if (mGo) {
            mMpu9250DataArray.add(data)
            if (mMpu9250DataArray.size >= mNumEntries) {
               save()
                mGo = false
            }
        } else {
            throw IllegalStateException("Call getReady() before add()")
        }
        return !mGo
    }

}