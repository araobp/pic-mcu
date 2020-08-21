package jp.araobp.mpu9250.analyzer

import android.graphics.Color
import android.view.SurfaceView

class MagnetoViewer(val surfaceView: SurfaceView, val maxNumEntries: Int) {

    companion object {
        const val POINT_RADIUS = 3F
    }

    var magnify = 1F

    private val mBufRecords = arrayOf(
        ArrayList<Short>(maxNumEntries),
        ArrayList(maxNumEntries),
        ArrayList(maxNumEntries)
    )

    fun clear() = mBufRecords.forEach { it.clear() }

    fun update3axis(data: Array<Short>) {

        mBufRecords.forEachIndexed { axis, shorts ->
            // Bounded array list
            if (shorts.size >= maxNumEntries) {
                shorts.removeAt(0)
            }
            shorts.add(data[axis])
        }

        val canvas = surfaceView.holder.lockCanvas()

        val width = canvas.width.toFloat()
        val height = canvas.height.toFloat()

        canvas.drawColor(Color.BLACK)

        val xCenter = width / 2F
        val yCenter = height / 2F

        val scale = height / 2F / Short.MAX_VALUE

        // X zero line
        canvas.drawLine(
            xCenter,
            0F,
            xCenter,
            height,
            DARK_GRAY_STROKE
        )

        // Y zero line
        canvas.drawLine(
            0F,
            yCenter,
            width,
            yCenter,
            DARK_GRAY_STROKE
        )

        for (i in 0 until mBufRecords[0].size) {
            val mx = mBufRecords[0][i] * scale * magnify
            val my = mBufRecords[1][i] * scale * magnify
            val mz = mBufRecords[2][i] * scale * magnify

            // XY plane
            val xy_X = -mx + xCenter
            val xy_Y = my + yCenter
            canvas.drawCircle(xy_X, xy_Y, POINT_RADIUS, RED_POINT)

            // YZ plane
            val yz_X = -my + xCenter
            val yz_Y = mz + yCenter
            canvas.drawCircle(yz_X, yz_Y, POINT_RADIUS, GREEN_POINT)

            // ZX plane
            val zx_X = -mz + xCenter
            val zx_Y = mx + yCenter
            canvas.drawCircle(zx_X, zx_Y, POINT_RADIUS, YELLOW_POINT)
        }

        surfaceView.holder.unlockCanvasAndPost(canvas)
    }
}