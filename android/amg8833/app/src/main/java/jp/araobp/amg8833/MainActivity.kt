package jp.araobp.amg8833

import android.content.pm.ActivityInfo
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.widget.addTextChangedListener
import jp.araobp.amg8833.`interface`.Amg8833Data
import jp.araobp.amg8833.`interface`.Amg8833Interface
import jp.araobp.amg8833.`interface`.IAmg8833Receiver
import jp.araobp.amg8833.analyzer.RawImage
import kotlinx.android.synthetic.main.activity_main.*
import java.lang.NumberFormatException


class MainActivity : AppCompatActivity() {

    private lateinit var amg8833Interface: Amg8833Interface

    private lateinit var rawImage: RawImage

    private lateinit var props: Properties

    val amg8833receiver = object : IAmg8833Receiver {
        override fun onAmg8833Data(amg8833Data: Amg8833Data) {
            rawImage.draw(
                amg8833Data.data,
                checkBoxNormalize.isChecked,
                checkBoxTemp.isChecked,
                RawImage.Colormap.valueOf(spinnerColormap.selectedItem.toString())
            )
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT

        setContentView(R.layout.activity_main)

        rawImage = RawImage(surfaceView)

        props = Properties(this)
        props.load()
        editTextSensorId.setText(props.dstId.toString())

        editTextSensorId.addTextChangedListener {
            try {
                props.dstId = editTextSensorId.text.toString().toInt()
                props.save()

                amg8833Interface.destroy()

                amg8833Interface = Amg8833Interface(this,
                    props.dstId.toByte(),
                    amg8833receiver
                )
            } catch(e: NumberFormatException) {
                // NOP
            }
        }
    }

    override fun onResume() {
        super.onResume()

        amg8833Interface = Amg8833Interface(this,
            props.dstId.toByte(),
            amg8833receiver)
    }

    override fun onPause() {
        super.onPause()
        amg8833Interface.destroy()
    }

}