package jp.araobp.analyzer

import android.graphics.Color
import android.view.SurfaceView

class WaveformMonitor(val surfaceView: SurfaceView, val maxNumEntries: Int) {

    private val mBufRecords = arrayOf(
        ArrayList<Short>(maxNumEntries),
        ArrayList(maxNumEntries),
        ArrayList(maxNumEntries),
        ArrayList(maxNumEntries),
        ArrayList(maxNumEntries),
        ArrayList(maxNumEntries)
    )
    private val colors = arrayOf(RED_STROKE, GREEN_STROKE, YELLOW_STROKE, BLUE_STROKE, MAGENTA_STROKE, CYAN_STROKE)

    fun update6axis(data: Array<Short>) {

        mBufRecords.forEachIndexed { axis, shorts ->
            // Bounded array list
            if (shorts.size >= maxNumEntries) {
                shorts.removeAt(0)
            }
            shorts.add(data[axis])
        }

        val canvas = surfaceView.holder.lockCanvas()
        canvas.drawColor(Color.BLACK)
        val xRatio = canvas.width.toFloat() / maxNumEntries.toFloat()
        val yZeroLine = surfaceView.height.toFloat() / 2F
        val yMax = yZeroLine * 9F / 10F
        val yMaxLine = yZeroLine - yMax
        val yMinLine = yZeroLine + yMax
        val scale = yMax / Short.MAX_VALUE

        // Y zero line
        canvas.drawLine(
            0F,
            yZeroLine,
            canvas.width.toFloat(),
            yZeroLine,
            DARK_GRAY_STROKE
        )

        // Y max/min lines
        canvas.drawLine(
            0F,
            yMaxLine,
            canvas.width.toFloat(),
            yMaxLine,
            DARK_GRAY_STROKE
        )
        canvas.drawLine(
            0F,
            yMinLine,
            canvas.width.toFloat(),
            yMinLine,
            DARK_GRAY_STROKE
        )

        mBufRecords.forEachIndexed { axis, shorts ->
            for (i in shorts.size - 1 downTo 1) {
                val v = -shorts[i] * scale + yZeroLine
                val vNext = -shorts[i - 1] * scale + yZeroLine
                canvas.drawLine(
                    (shorts.size - i - 1).toFloat() * xRatio,
                    v,
                    (shorts.size - i).toFloat() * xRatio,
                    vNext,
                    colors[axis]
                )
            }
        }

        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}