package jp.araobp.mpu9250

import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_accel_gyro.*

class AccelGyroActivity : AppCompatActivity() {

    companion object {
        val TAG: String = this::class.java.simpleName
    }

    private var baudrate: Int = 9800
    private lateinit var dataReceiver: DataReceiver

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_accel_gyro)

        textViewMessageDump.movementMethod = ScrollingMovementMethod()

        baudrate = intent.getIntExtra(MainActivity.BAUDRATE, 9600)

    }

    override fun onResume() {
        super.onResume()
        dataReceiver = DataReceiver(this, baudrate,
            object : IDataReceiver {
                override fun onMpu9250Data(data: Mpu9250Data) {
                    if (toggleButtonStart.isChecked) {
                        textViewRealtime.post {
                            textViewRealtime.append(data.toString() + "\n")
                        }
                    }
                }

                override fun onAk8963Data(data: Ak8963Data) {
                    if (toggleButtonStart.isChecked) {
                        textViewMessageDump.post {
                            textViewRealtime.append(data.toString() + "\n")
                        }
                    }
                }
            })
    }

    override fun onPause() {
        super.onPause()
        dataReceiver.destroy()
    }
}