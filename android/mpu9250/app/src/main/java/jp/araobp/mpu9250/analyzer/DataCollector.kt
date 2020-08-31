package jp.araobp.mpu9250.analyzer

import jp.araobp.mpu9250.serial.Mpu9250Data
import java.lang.Exception

class DataCollector {

    private var mMpu9250DataArray =  ArrayList<Mpu9250Data>()

    private var mLabel = "Unknown"
    private var mGo = false
    private var mNumEntries = 0

    fun getReady(label: String, numEntries: Int) {
        mMpu9250DataArray.clear()
        mLabel = label
        mNumEntries = numEntries
        mGo = true
    }

    fun add(data: Mpu9250Data): Boolean {
        if (mGo) {
            mMpu9250DataArray.add(data)
            if (mMpu9250DataArray.size >= mNumEntries) {
               saveData()
                mGo = false
            }
        } else {
            throw IllegalStateException("Call getReady() before add()")
        }
        return !mGo
    }

    fun saveData() {

    }


}