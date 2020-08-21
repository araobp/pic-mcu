package jp.araobp.mpu9250.serial

data class Mpu9250Data (
    val seq: UInt,
    val ax: Short,
    val ay: Short,
    val az: Short,
    val gx: Short,
    val gy: Short,
    val gz: Short
)
