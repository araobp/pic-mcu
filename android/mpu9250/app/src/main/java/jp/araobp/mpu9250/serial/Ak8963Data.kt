package jp.araobp.mpu9250.serial

data class Ak8963Data (
    val seq: UInt,
    val mx: Short,
    val my: Short,
    val mz: Short
) {
    fun toCsv() = "$seq,$mx,$my,$mz\r\n"
}
