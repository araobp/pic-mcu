package jp.araobp.amg8833.analyzer

import android.graphics.Color
import android.view.SurfaceView
import kotlin.math.roundToInt

class RawImage(val surfaceView: SurfaceView) {

    fun draw(data: ByteArray, minMaxNormalization: Boolean, showTemperature: Boolean) {

        val tempList = tempList(data.toUByteArray()).reversed()

        val pixels: List<Float> = when(minMaxNormalization) {
            true -> minMaxNormalize(data.toUByteArray()).reversed()
            false -> data.toUByteArray().reversed().map{ it.toFloat() }
        }

        val canvas = surfaceView.holder.lockCanvas()
        canvas.drawColor(Color.DKGRAY)

        val width = canvas.width.toFloat()
        val height = canvas.height.toFloat()

        val xStep = width / 8F
        val yStep = xStep
        val yMargin = (height - width) / 2F
        val textMargin = xStep/5F

        for (row in 0 until 8) {
            for (col in 0 until 8) {
                val idx = row * 8 + col
                val pixel = pixels[row * 8 + col].roundToInt()
                val left = xStep * col
                val top = yStep * row + yMargin
                val right = xStep * (col + 1)
                val bottom = yStep * (row + 1) + yMargin
                val brightness = paintOkinawa(pixel)
                canvas.drawRect(left, top, right, bottom, brightness)
                canvas.drawText(tempList[idx], left + textMargin, top+TEMP_COLOR.textSize+textMargin, TEMP_COLOR)
            }
        }


        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}