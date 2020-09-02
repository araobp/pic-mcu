package jp.araobp.mpu9250

import android.content.pm.ActivityInfo
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.view.KeyEvent
import androidx.appcompat.app.AppCompatActivity
import jp.araobp.mpu9250.serial.Ak8963Data
import jp.araobp.mpu9250.serial.IDataReceiver
import jp.araobp.mpu9250.serial.Mpu9250Data
import jp.araobp.mpu9250.serial.Mpu9250Interface
import kotlinx.android.synthetic.main.activity_inference.*

class InferenceActivity : AppCompatActivity() {

    private lateinit var mProps: Properties

    private lateinit var mpu9250Interface: Mpu9250Interface

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT

        setContentView(R.layout.activity_inference)

        setTitle("Analyzer")

        fullscreen(window)

        textView.movementMethod = ScrollingMovementMethod()

        mProps = Properties(this)

    }

    override fun onResume() {
        super.onResume()

        mpu9250Interface = Mpu9250Interface(this, mProps.baudrate,
            object : IDataReceiver {
                override fun onMpu9250Data(data: Mpu9250Data) {
                        // TODO: call inference function
                }

                override fun onAk8963Data(data: Ak8963Data) {
                }
            })
    }

    override fun onPause() {
        super.onPause()
        mpu9250Interface.destroy()
    }

    override fun dispatchKeyEvent(event: KeyEvent): Boolean {
        if (event.keyCode == KeyEvent.KEYCODE_VOLUME_UP && event.action == KeyEvent.ACTION_DOWN) {
            toggleStart()
        }
        return true
    }

    private fun toggleStart() {

    }

}