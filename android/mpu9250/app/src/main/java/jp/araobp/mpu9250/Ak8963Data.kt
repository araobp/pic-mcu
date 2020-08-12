package jp.araobp.mpu9250

data class Ak8963Data (
    val seq: UInt,
    val mx: Float,
    val my: Float,
    val mz: Float
)
