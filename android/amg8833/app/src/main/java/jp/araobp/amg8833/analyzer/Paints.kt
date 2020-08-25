package jp.araobp.amg8833.analyzer

import android.graphics.Color
import android.graphics.Paint
import kotlin.math.floor

internal val TEMP_COLOR = Paint().apply {
    color = Color.GREEN
    style = Paint.Style.FILL_AND_STROKE
    textSize = 64F
}

internal fun paintGrayscale(brightness: UByte) = Paint().apply {
    val v = brightness.toInt()
    style = Paint.Style.FILL_AND_STROKE
    color = Color.rgb(v, v, v)
}

internal fun paintOkinawa(brightness: UByte) = Paint().apply {
    val v = brightness.toInt()
    style = Paint.Style.FILL_AND_STROKE
    color = Color.argb(0xff, 128 - v / 2, v, 128 + v / 2)
}

internal fun paintHeatmap(brightness: UByte) = Paint().apply {
    val v = brightness.toInt()
    style = Paint.Style.FILL_AND_STROKE
    color = Color.argb(0xff, v, 255 - v, 255 - v)
}

internal fun paintDevil(brightness: UByte) = Paint().apply {
    val v = brightness.toInt()
    style = Paint.Style.FILL_AND_STROKE
    color = Color.argb(0xff, v, v / 2, v / 2)
}

// Reference: https://www.particleincell.com/2014/colormap/
internal fun paintRainbow(brightness: UByte) = Paint().apply {

    val v = brightness

    val a = (UByte.MAX_VALUE - v).toDouble() / 80
    val X = floor(a).toInt()
    val Y = floor(255 * (a - X)).toInt()

    var r = 0
    var g = 0
    var b = 0

    when (X) {
        0 -> {
            r = 255
            g = Y
            b = 0
        }
        1 -> {
            r = 255 - Y
            g = 255
            b = 0
        }
        2 -> {
            r = 0
            g = 255
            b = Y
        }
        3 -> {
            r = 0
            g = 255 - Y
            b = 255
        }
        4 -> {
            r = 0
            g = 0
            b = 255
        }
    }

    style = Paint.Style.FILL_AND_STROKE
    color = Color.argb(0xff, r, g, b)
}

