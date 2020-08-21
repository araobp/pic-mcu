package jp.araobp.mpu9250

import android.content.pm.ActivityInfo
import android.os.Bundle
import android.view.View
import android.widget.AdapterView
import androidx.appcompat.app.AppCompatActivity
import jp.araobp.mpu9250.analyzer.MagnetoViewer
import jp.araobp.mpu9250.serial.Ak8963Data
import jp.araobp.mpu9250.serial.IDataReceiver
import jp.araobp.mpu9250.serial.Mpu9250Data
import jp.araobp.mpu9250.serial.Mpu9250Interface
import kotlinx.android.synthetic.main.activity_accel_gyro.*
import kotlinx.android.synthetic.main.activity_accel_gyro.surfaceViewAnalyzer
import kotlinx.android.synthetic.main.activity_accel_gyro.textViewDump
import kotlinx.android.synthetic.main.activity_accel_gyro.textViewDumpTitle
import kotlinx.android.synthetic.main.activity_accel_gyro.toggleButtonCapture
import kotlinx.android.synthetic.main.activity_accel_gyro.toggleButtonDump
import kotlinx.android.synthetic.main.activity_magneto.*

class MagnetoActivity : AppCompatActivity() {

    companion object {
        val TAG: String = this::class.java.simpleName
        const val MAX_NUM_ENTRIES = 256
    }

    private var baudrate: Int = 115200
    private lateinit var mpu9250Interface: Mpu9250Interface

    private var magnetoViewer: MagnetoViewer? = null

    private fun updateViewer(data: Ak8963Data) {
        magnetoViewer?.update3axis(
            arrayOf(
                data.mx,
                data.my,
                data.mz
            )
        )
    }

    private fun enableDumpWindow(visible: Boolean) {
        if (visible) {
            textViewDumpTitle.visibility = View.VISIBLE
            textViewDump.visibility = View.VISIBLE
        } else {
            textViewDumpTitle.visibility = View.GONE
            textViewDump.visibility = View.GONE
        }
    }

    private fun fullscreen() {
        window.decorView.systemUiVisibility =
            View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_FULLSCREEN or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE

        setContentView(R.layout.activity_magneto)

        setTitle("Magnetometer")

        fullscreen()

        baudrate = intent.getIntExtra(MainActivity.BAUDRATE, 115200)

        surfaceViewAnalyzer.post {
            magnetoViewer = MagnetoViewer(
                surfaceView = surfaceViewAnalyzer, maxNumEntries = MAX_NUM_ENTRIES
            )
        }

        spinnerMagnify.onItemSelectedListener =
            object : AdapterView.OnItemSelectedListener {
                override fun onNothingSelected(p0: AdapterView<*>?) = Unit
                override fun onItemSelected(
                    p0: AdapterView<*>?,
                    p1: View?,
                    p2: Int,
                    p3: Long
                ) {
                    magnetoViewer?.magnify = spinnerMagnify.selectedItem.toString().toFloat()
                }
            }

        toggleButtonDump.setOnCheckedChangeListener { _, isChecked ->
            enableDumpWindow(isChecked)
        }

        toggleButtonCapture.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                mpu9250Interface.resume()
            } else {
                mpu9250Interface.pause()
            }
        }

        buttonClear.setOnClickListener {
            magnetoViewer?.clear()
        }

        enableDumpWindow(toggleButtonDump.isChecked)
    }

    override fun onResume() {
        super.onResume()
        mpu9250Interface = Mpu9250Interface(this, baudrate,
            object : IDataReceiver {
                override fun onMpu9250Data(data: Mpu9250Data) {
                    // NOP
                }

                override fun onAk8963Data(data: Ak8963Data) {
                    if (toggleButtonCapture.isChecked) {
                        textViewDump.post {
                            textViewDump.append(data.toString() + "\n")
                        }
                        updateViewer(data)
                    }
                }
            })
    }

    override fun onPause() {
        super.onPause()
        mpu9250Interface.destroy()
    }

}