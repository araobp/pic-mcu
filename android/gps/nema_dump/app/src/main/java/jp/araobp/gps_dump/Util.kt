package jp.araobp.gps_dump

import java.time.LocalDateTime
import java.time.format.DateTimeFormatter

fun localDateTime(): String {
    val current = LocalDateTime.now()
    val formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss.SSS")
    return current.format(formatter)
}