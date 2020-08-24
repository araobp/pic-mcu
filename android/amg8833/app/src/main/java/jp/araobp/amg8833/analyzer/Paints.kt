package jp.araobp.amg8833.analyzer

import android.graphics.Color
import android.graphics.Paint

fun paintGrayscale(brightness: Int) = Paint().apply {
    style = Paint.Style.FILL_AND_STROKE
    color = Color.rgb(brightness, brightness, brightness)
}

fun paintOkinawa(brightness: Int) = Paint().apply {
    style = Paint.Style.FILL_AND_STROKE
    color = Color.argb(0xff, 128 - brightness / 2, brightness, 128 + brightness / 2)
}

fun paintHeatmap(brightness: Int) = Paint().apply {
    style = Paint.Style.FILL_AND_STROKE
    color = Color.argb(0xff, brightness, 0, 255 - brightness)
}

val TEMP_COLOR = Paint().apply {
    color = Color.GREEN
    style = Paint.Style.FILL_AND_STROKE
    textSize = 64F
}
