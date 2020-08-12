package jp.araobp.mpu9250

data class Mpu9250Data (
    val seq: UInt,
    val ax: Float,
    val ay: Float,
    val az: Float,
    val gx: Float,
    val gy: Float,
    val gz: Float
)
