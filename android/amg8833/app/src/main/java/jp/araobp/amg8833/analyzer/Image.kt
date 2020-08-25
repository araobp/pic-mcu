package jp.araobp.amg8833.analyzer

import android.R.string
import android.graphics.Color
import android.view.SurfaceView
import org.opencv.core.CvType
import org.opencv.core.Mat
import org.opencv.imgproc.Imgproc.connectedComponents


class Image(val surfaceView: SurfaceView) {

    companion object {
        val TAG: String = this::class.java.simpleName
    }

    enum class Colormap {
        Grayscale,
        Okinawa,
        Heatmap,
        Devil,
        Rainbow,
        Rainbow2
    }

    fun draw(
        data: ByteArray,
        minMaxNormalization: Boolean,
        showTemperature: Boolean,
        colorMap: Colormap,
        interpolationRepeat: Int = 0,
        binarize: Boolean = false
    ) {

        val src = data.toUByteArray().reversed().toUByteArray()

        val pixels: UByteArray = when (minMaxNormalization) {
            true -> minMaxNormalize(src)
            false -> src
        }

        var tempList: List<String>? = null

        if (showTemperature) {
            tempList = tempList(src)
        }

        var mat = pixels.toMat(8, 8)

        if (interpolationRepeat > 0) {
            mat = interpolate(mat, interpolationRepeat)
        }

        val labels = Mat()
        val labelsStr = ArrayList<String>()

        if (binarize) {
            mat = binarize(mat)
            connectedComponents(mat, labels, 8, CvType.CV_16U);

            labelsStr.clear()
            for (row in 0 until mat.rows()) {
                for (col in 0 until mat.cols()) {
                    labelsStr.add(labels.get(row, col)[0].toInt().toString())
                }
            }
            tempList = labelsStr.toList()
        }

        //Log.d(TAG, "numRows: $numRows, numCols: $numCols, numPixels: ${finalPixels.size}")

        val finalPixels = convertToUByteArray(mat)

        val canvas = surfaceView.holder.lockCanvas()
        canvas.drawColor(Color.DKGRAY)

        val width = canvas.width.toFloat()
        val height = canvas.height.toFloat()

        val rows = mat.rows()
        val cols = mat.cols()

        val xStep = width / cols
        val yStep = xStep
        val yMargin = (height - width) / 2F
        val textMargin = xStep / 5F

        for (row in 0 until rows) {
            for (col in 0 until cols) {
                val idx = row * cols + col
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
                    Colormap.Rainbow2 -> paintRainbow2(pixel)
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