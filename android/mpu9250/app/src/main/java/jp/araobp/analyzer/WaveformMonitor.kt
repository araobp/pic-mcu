package jp.araobp.analyzer

import android.graphics.Color
import android.view.SurfaceView

class WaveformMonitor(val surfaceView: SurfaceView, val maxNumEntries: Int, val scale: Float) {

    private val mBufRecords = arrayOf(ArrayList<Float>(maxNumEntries),ArrayList(maxNumEntries),ArrayList(maxNumEntries))
    private val colors = arrayOf(RED_STROKE, GREEN_STROKE, YELLOW_STROKE)

    fun update3axis(data: Array<Float>) {

        mBufRecords.forEachIndexed { axis, shorts ->
            if (shorts.size >= maxNumEntries) {
                shorts.removeAt(0)
            }
            shorts.add(data[axis])
        }

        val canvas = surfaceView.holder.lockCanvas()
        canvas.drawColor(Color.BLACK)
        val ratioHorizontal = canvas.width.toFloat() / maxNumEntries.toFloat()
        val offsetVertical = surfaceView.height / 2

        canvas.drawLine(
            0F,
            offsetVertical.toFloat(),
            canvas.width.toFloat(),
            offsetVertical.toFloat(),
            DARK_GRAY_STROKE
        )

        mBufRecords.forEachIndexed { axis, shorts ->
            for (i in shorts.size - 1 downTo 1) {
                val v = -shorts[i] * scale + offsetVertical
                val vNext = -shorts[i - 1] * scale + offsetVertical
                canvas.drawLine(
                    (shorts.size - i - 1).toFloat() * ratioHorizontal,
                    v,
                    (shorts.size - i).toFloat() * ratioHorizontal,
                    vNext,
                    colors[axis]
                )
            }
        }

        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}