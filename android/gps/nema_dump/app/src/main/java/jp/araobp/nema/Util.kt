package jp.araobp.nema

import java.time.LocalDateTime
import java.time.format.DateTimeFormatter

// Convert dddmm.mmmm to Googla Map coordinates
fun String.toGooglMapCoordsF(): Float {
    val v = this.toFloat()/100F  // ddd.mmmmmm
    val ddd = v.toInt()
    val mmDotmmmm = (v - ddd) * 100F
    return ddd + mmDotmmmm /60F
}
