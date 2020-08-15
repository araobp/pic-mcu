package jp.araobp.analyzer

import android.graphics.Color
import android.view.SurfaceView
import java.time.Year

class WaveformMonitor(val surfaceView: SurfaceView, val maxNumEntries: Int, val scale: Float) {

    private val mBufRecords = arrayOf(ArrayList<Float>(maxNumEntries),ArrayList(maxNumEntries),ArrayList(maxNumEntries))
    private val prevVerticals = FloatArray(3)

    private val colors = arrayOf(RED_STROKE, GREEN_STROKE, YELLOW_STROKE)

    fun update3axis(data: Array<Float>) {

        mBufRecords.forEachIndexed { idx, shorts ->
            if (shorts.size >= maxNumEntries) {
                shorts.removeAt(0)
            }
            shorts.add(data[idx])
        }

        val canvas = surfaceView.holder.lockCanvas()
        canvas.drawColor(Color.BLACK)
        val ratioHorizontal = canvas.width.toFloat() / maxNumEntries.toFloat()
        val offsetVertical = surfaceView.height / 2

        data.forEachIndexed { idx, fl ->
            prevVerticals[idx] = -mBufRecords[idx][0] * scale + offsetVertical
        }

        canvas.drawLine(
            0F,
            offsetVertical.toFloat(),
            canvas.width.toFloat(),
            offsetVertical.toFloat(),
            DARK_GRAY_STROKE
        )

        mBufRecords.forEachIndexed { idx, shorts ->
            for (i in 1 until shorts.size) {
                val v = -shorts[i] * scale + offsetVertical
                canvas.drawLine(
                    (i - 1).toFloat() * ratioHorizontal,
                    prevVerticals[idx],
                    i.toFloat() * ratioHorizontal,
                    v,
                    colors[idx]
                )
                prevVerticals[idx] = v
            }
        }

        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}