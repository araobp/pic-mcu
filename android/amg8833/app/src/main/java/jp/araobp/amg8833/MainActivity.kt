package jp.araobp.amg8833

import android.os.Bundle
import android.widget.FrameLayout
import androidx.appcompat.app.AppCompatActivity
import jp.araobp.amg8833.`interface`.Amg8833Data
import jp.araobp.amg8833.`interface`.Amg8833Interface
import jp.araobp.amg8833.`interface`.IAmg8833Receiver
import jp.araobp.amg8833.analyzer.RawImage
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    private lateinit var amg8833Interface: Amg8833Interface

    private lateinit var rawImage: RawImage

    val dst = 2.toByte()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        rawImage = RawImage(surfaceView)

    }

    override fun onResume() {
        super.onResume()

        amg8833Interface = Amg8833Interface(this,
            dst,
            object : IAmg8833Receiver {
                override fun onAmg8833Data(amg8833Data: Amg8833Data) {
                    rawImage.draw(amg8833Data.data, checkBoxNormalize.isChecked, checkBoxTemp.isChecked)
                }
            }
        )
    }

    override fun onPause() {
        super.onPause()
        amg8833Interface.destroy()
    }

}