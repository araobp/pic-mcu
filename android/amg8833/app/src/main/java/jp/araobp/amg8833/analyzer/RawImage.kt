package jp.araobp.amg8833.analyzer

import android.graphics.Color
import android.graphics.PorterDuff
import android.view.SurfaceView
import jp.araobp.amg8833.R

class RawImage(val surfaceView: SurfaceView) {

    fun draw(data: ByteArray) {

        val normalizedData = minMaxNormalize(data.toUByteArray()).reversed()

        val canvas = surfaceView.holder.lockCanvas()
        canvas.drawColor(Color.DKGRAY)

        val width = canvas.width.toFloat()
        val height = canvas.height.toFloat()

        val xStep = width / 8F
        val yStep = xStep
        val yMargin = (height - width) / 2F

        for (row in 0 until 8) {
            for (col in 0 until 8) {
                val pixel = normalizedData[row * 8 + col]
                val left = xStep * col
                val top = yStep * row + yMargin
                val right = xStep * (col + 1)
                val bottom = yStep * (row + 1) + yMargin
                val paint = paintGrayscale(pixel.toInt())

                canvas.drawRect(left, top, right, bottom, paint)
            }
        }
        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}