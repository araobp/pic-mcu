package jp.araobp.mpu9250.serial

import jp.araobp.mpu9250.serial.Ak8963Data
import jp.araobp.mpu9250.serial.Mpu9250Data

interface IDataReceiver {

    fun onMpu9250Data(data: Mpu9250Data)

    fun onAk8963Data(data: Ak8963Data)

}