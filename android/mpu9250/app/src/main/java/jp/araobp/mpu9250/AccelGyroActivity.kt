package jp.araobp.mpu9250

import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import jp.araobp.analyzer.WaveformMonitor
import kotlinx.android.synthetic.main.activity_accel_gyro.*

class AccelGyroActivity : AppCompatActivity() {

    companion object {
        val TAG: String = this::class.java.simpleName
        const val MAX_NUM_ENTRIES = 128
        const val SCALEDOWN = 1.0F
    }

    private var baudrate: Int = 9800
    private lateinit var dataReceiver: DataReceiver

    private var waveformMonitor: WaveformMonitor? = null

    private fun updateWaveform(data: Any) {

        when(data) {
            is Mpu9250Data -> {
                waveformMonitor?.update3axis(arrayOf(data.ax, data.ay, data.az))
            }
            is Ak8963Data -> {

            }
        }
    }

    private fun dumpWindow(visible: Boolean) {
        if (visible) {
            textViewDumpTItle.visibility = View.VISIBLE
            textViewDump.visibility = View.VISIBLE
        }
        else {
            textViewDumpTItle.visibility = View.GONE
            textViewDump.visibility = View.GONE
        }
    }

    private fun fullscreen() {
        window.decorView.systemUiVisibility =
            View.SYSTEM_UI_FLAG_HIDE_NAVIGATION or View.SYSTEM_UI_FLAG_FULLSCREEN or View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_accel_gyro)

        fullscreen()

        textViewDumpTItle.movementMethod = ScrollingMovementMethod()

        baudrate = intent.getIntExtra(MainActivity.BAUDRATE, 9600)

        surfaceViewAnalyzer.post {
            waveformMonitor = WaveformMonitor(
                surfaceView = surfaceViewAnalyzer, maxNumEntries = MAX_NUM_ENTRIES, scale = 100.0F
            )
        }

        toggleButtonDump.setOnCheckedChangeListener { _, isChecked ->
            dumpWindow(isChecked)
        }

        dumpWindow(toggleButtonDump.isChecked)
    }

    override fun onResume() {
        super.onResume()
        dataReceiver = DataReceiver(this, baudrate,
            object : IDataReceiver {
                override fun onMpu9250Data(data: Mpu9250Data) {
                    if (toggleButtonStart.isChecked) {
                        textViewDump.post {
                            textViewDump.append(data.toString() + "\n")
                        }
                        updateWaveform(data)
                    }
                }

                override fun onAk8963Data(data: Ak8963Data) {
                    if (toggleButtonStart.isChecked) {
                        textViewDumpTItle.post {
                            textViewDump.append(data.toString() + "\n")
                        }
                    }
                    updateWaveform(data)
                }
            })
    }

    override fun onPause() {
        super.onPause()
        dataReceiver.destroy()
    }
}