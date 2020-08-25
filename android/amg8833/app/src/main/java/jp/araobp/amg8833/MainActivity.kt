package jp.araobp.amg8833

import android.content.pm.ActivityInfo
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.core.widget.addTextChangedListener
import jp.araobp.amg8833.`interface`.Amg8833Data
import jp.araobp.amg8833.`interface`.Amg8833Interface
import jp.araobp.amg8833.`interface`.IAmg8833Receiver
import jp.araobp.amg8833.analyzer.Image
import jp.araobp.amg8833.analyzer.binarize
import kotlinx.android.synthetic.main.activity_main.*
import org.opencv.android.OpenCVLoader
import java.lang.NumberFormatException


class MainActivity : AppCompatActivity() {

    companion object {
        const val DIFF_MAGNIFICATION = 5
    }

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

    var prevAmg8833Data: Amg8833Data? = null

    val amg8833receiver = object : IAmg8833Receiver {

        override fun onAmg8833Data(amg8833Data: Amg8833Data) {

            var data: ByteArray = amg8833Data.data

            if (checkBoxDiff.isChecked) {
                val diff = ByteArray(64)
                prevAmg8833Data?.let {
                    amg8833Data.data.forEachIndexed { idx, byte ->
                        // add 0x80(128) so that negative values can be seen.
                        diff[idx] =
                            ((byte.toInt() - it.data[idx].toInt()) * DIFF_MAGNIFICATION + 128).toByte()
                    }
                    data = diff
                }
                prevAmg8833Data = amg8833Data.clone()
            }

            rawImage.draw(
                data,
                checkBoxNormalize.isChecked,
                checkBoxTemp.isChecked,
                Image.Colormap.valueOf(spinnerColormap.selectedItem.toString()),
                spinnerInterpolate.selectedItem.toString().toInt(),
                checkBoxBinarize.isChecked
            )
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

                amg8833Interface = Amg8833Interface(
                    this,
                    props.dstId.toByte(),
                    amg8833receiver
                )
            } catch (e: NumberFormatException) {
                // NOP
            }
        }
    }

    override fun onResume() {
        super.onResume()

        amg8833Interface = Amg8833Interface(
            this,
            props.dstId.toByte(),
            amg8833receiver
        )
    }

    override fun onPause() {
        super.onPause()
        amg8833Interface.destroy()
    }

}