package jp.araobp.mpu9250

interface IDataReceiver {

    fun onMpu9250Data(data: Mpu9250Data)

    fun onAk8963Data(data: Ak8963Data)

}