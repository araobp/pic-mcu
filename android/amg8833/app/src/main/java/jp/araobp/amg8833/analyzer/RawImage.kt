package jp.araobp.amg8833.analyzer

import android.graphics.Color
import android.view.SurfaceView
import kotlin.math.roundToInt

class RawImage(val surfaceView: SurfaceView) {

    enum class Colormap {
        Grayscale,
        Okinawa,
        Heatmap
    }

    fun draw(data: ByteArray, minMaxNormalization: Boolean, showTemperature: Boolean, colorMap: Colormap) {

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

        var tempList: List<String>? = null

        if (showTemperature) {
            tempList = tempList(data.toUByteArray()).reversed()
        }

        for (row in 0 until 8) {
            for (col in 0 until 8) {
                val idx = row * 8 + col
                val pixel = pixels[row * 8 + col].roundToInt()
                val left = xStep * col
                val top = yStep * row + yMargin
                val right = xStep * (col + 1)
                val bottom = yStep * (row + 1) + yMargin
                val brightness = when(colorMap) {
                    Colormap.Grayscale -> paintGrayscale(pixel)
                    Colormap.Okinawa -> paintOkinawa(pixel)
                    Colormap.Heatmap -> paintHeatmap(pixel)
                }
               canvas.drawRect(left, top, right, bottom, brightness)
                tempList?.let {
                    canvas.drawText(
                        it[idx],
                        left + textMargin,
                        top + TEMP_COLOR.textSize + textMargin,
                        TEMP_COLOR
                    )
                }
            }
        }


        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}