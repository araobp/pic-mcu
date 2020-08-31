package jp.araobp.mpu9250.analyzer

import android.graphics.Color
import android.view.SurfaceView
import jp.araobp.mpu9250.Properties
import jp.araobp.mpu9250.serial.Ak8963Data
import kotlin.collections.ArrayList
import kotlin.concurrent.thread

class MagnetoViewer(val surfaceView: SurfaceView, val maxNumEntries: Int, val props: Properties) {

    companion object {
        const val POINT_RADIUS = 3F
    }

    private val mBufRecords = arrayOf(
        ArrayList<Short>(maxNumEntries),
        ArrayList(maxNumEntries),
        ArrayList(maxNumEntries)
    )

    fun clear() = mBufRecords.forEach { it.clear() }

    var mListener: IMagnetoEventListener? = null
    var mCalibrate = false
    var mCalibNumSamples = 0

    private var mOffsets = arrayOf(0F, 0F, 0F)
    private var mScales = arrayOf(1F, 1F, 1F)

    fun startCalibration(listener: IMagnetoEventListener) {
        clear()
        mListener = listener
        mCalibrate = true
    }


    fun update(data: Ak8963Data) {

        val array = arrayOf(
            data.mx,
            data.my,
            data.mz
        )

        mBufRecords.forEachIndexed { axis, shorts ->
            // Bounded array list
            if (shorts.size >= maxNumEntries) {
                shorts.removeAt(0)
            }
            shorts.add(array[axis])
        }

        val canvas = surfaceView.holder.lockCanvas()

        val width = canvas.width.toFloat()
        val height = canvas.height.toFloat()

        canvas.drawColor(Color.BLACK)

        val xCenter = width / 2F
        val yCenter = height / 2F

        val range = height / 2F / Short.MAX_VALUE

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
            val mx = (mBufRecords[0][i] - mOffsets[0]) * mScales[0] * range * props.magnetoMagnify
            val my = (mBufRecords[1][i] - mOffsets[1]) * mScales[1] * range * props.magnetoMagnify
            val mz = (mBufRecords[2][i] - mOffsets[2]) * mScales[2] * range * props.magnetoMagnify

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

        // Graph legends
        val textStartX = width - 100
        canvas.drawText("xy", textStartX, height - 150, RED_TEXT)
        canvas.drawText("yz", textStartX, height - 100, GREEN_TEXT)
        canvas.drawText("zx", textStartX, height - 50, YELLOW_TEXT)

        surfaceView.holder.unlockCanvasAndPost(canvas)

        if (mCalibrate) {
            if (++mCalibNumSamples >= maxNumEntries) {
                mCalibrate = false
                mCalibNumSamples = 0
                performCalibration()
            }
        }
    }

    private fun performCalibration() {
        thread {
            val minMaxHalves = arrayOf(0F, 0F, 0F)

            mBufRecords.forEachIndexed { idx, arrayList ->
                val magMin = arrayList.min()!!
                val magMax = arrayList.max()!!
                mOffsets[idx] = (magMax + magMin) / 2F
                minMaxHalves[idx] = (magMax - magMin) / 2F
            }

            val average = minMaxHalves.sum() / 3F
            minMaxHalves.forEachIndexed { idx, scale ->
                mScales[idx] = average / scale
            }

            mListener?.onCalibrationFinished()
        }
    }

}