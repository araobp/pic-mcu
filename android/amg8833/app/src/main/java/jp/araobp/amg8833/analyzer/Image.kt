package jp.araobp.amg8833.analyzer

import android.graphics.Color
import android.util.Log
import android.view.SurfaceView
import kotlin.math.roundToInt

class Image(val surfaceView: SurfaceView) {

    companion object {
        val TAG: String = this::class.java.simpleName
    }

    enum class Colormap {
        Grayscale,
        Okinawa,
        Heatmap,
        Devil,
        Rainbow
    }

    fun draw(
        data: ByteArray,
        minMaxNormalization: Boolean,
        showTemperature: Boolean,
        colorMap: Colormap,
        interpolationRepeat: Int = 0
    ) {

        val src = data.toUByteArray().reversed().toUByteArray()

        val pixels: UByteArray = when (minMaxNormalization) {
            true -> minMaxNormalize(src)
            false -> src
        }

        val canvas = surfaceView.holder.lockCanvas()
        canvas.drawColor(Color.DKGRAY)

        val width = canvas.width.toFloat()
        val height = canvas.height.toFloat()

        var tempList: List<String>? = null

        if (showTemperature) {
            tempList = tempList(src)
        }

        var numRows = 8
        var numCols = 8

        val finalPixels = if (interpolationRepeat > 0) {
            val srcMat = pixels.toMat()
            val dstMat = interpolate(srcMat, interpolationRepeat)
            numRows = dstMat.rows()
            numCols = dstMat.cols()
            convertToUByteArray(dstMat)
        } else {
            pixels
        }

        //Log.d(TAG, "numRows: $numRows, numCols: $numCols, numPixels: ${finalPixels.size}")

        val xStep = width / numCols
        val yStep = xStep
        val yMargin = (height - width) / 2F
        val textMargin = xStep / 5F

        for (row in 0 until numRows) {
            for (col in 0 until numCols) {
                val idx = row * numCols + col
                val pixel = finalPixels[idx]
                val left = xStep * col
                val top = yStep * row + yMargin
                val right = xStep * (col + 1)
                val bottom = yStep * (row + 1) + yMargin
                val brightness = when (colorMap) {
                    Colormap.Grayscale -> paintGrayscale(pixel)
                    Colormap.Okinawa -> paintOkinawa(pixel)
                    Colormap.Heatmap -> paintHeatmap(pixel)
                    Colormap.Devil -> paintDevil(pixel)
                    Colormap.Rainbow -> paintRainbow(pixel)
                }
                canvas.drawRect(left, top, right, bottom, brightness)

                tempList?.let {
                    if (interpolationRepeat == 0) {
                        canvas.drawText(
                            it[idx],
                            left + textMargin,
                            top + TEMP_COLOR.textSize + textMargin,
                            TEMP_COLOR
                        )
                    }

                }
            }
        }

        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}