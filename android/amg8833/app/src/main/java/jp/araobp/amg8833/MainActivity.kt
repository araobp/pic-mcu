package jp.araobp.amg8833

import android.content.pm.ActivityInfo
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.widget.addTextChangedListener
import jp.araobp.amg8833.`interface`.Amg8833Data
import jp.araobp.amg8833.`interface`.Amg8833Interface
import jp.araobp.amg8833.`interface`.IAmg8833Receiver
import jp.araobp.amg8833.analyzer.Image
import kotlinx.android.synthetic.main.activity_main.*
import org.opencv.android.OpenCVLoader
import java.lang.NumberFormatException


class MainActivity : AppCompatActivity() {

    private lateinit var amg8833Interface: Amg8833Interface

    private lateinit var rawImage: Image

    private lateinit var props: Properties

    /**
     * This is necessary to avoid the following error at runtime:
     *
     * "No implementation found for void org.opencv.core.Mat.n_delete(long) (tried Java_org_opencv
     * _core_Mat_n_1delete and Java_org_opencv_core_Mat_n_1delete__J)"
     */
    init {
        // OpenCV initialization
        OpenCVLoader.initDebug()
    }

    val amg8833receiver = object : IAmg8833Receiver {
        override fun onAmg8833Data(amg8833Data: Amg8833Data) {
            runOnUiThread {
                rawImage.draw(
                    amg8833Data.data,
                    checkBoxNormalize.isChecked,
                    checkBoxTemp.isChecked,
                    Image.Colormap.valueOf(spinnerColormap.selectedItem.toString()),
                    spinnerInterpolate.selectedItem.toString().toInt()
                )
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT

        setContentView(R.layout.activity_main)

        rawImage = Image(surfaceView)

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