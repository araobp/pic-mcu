package jp.araobp.mpu9250

import android.app.Dialog
import android.content.pm.ActivityInfo
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import jp.araobp.mpu9250.analyzer.MagnetoViewer
import jp.araobp.mpu9250.analyzer.Oscilloscope
import jp.araobp.mpu9250.serial.Ak8963Data
import jp.araobp.mpu9250.serial.IDataReceiver
import jp.araobp.mpu9250.serial.Mpu9250Data
import jp.araobp.mpu9250.serial.Mpu9250Interface
import kotlinx.android.synthetic.main.activity_analyzer.*

class AnalyzerActivity : AppCompatActivity() {

    companion object {
        const val MAX_NUM_ENTRIES_6AXES = 256
        const val MAX_NUM_ENTRIES_3AXES = 128

    }

    private lateinit var mpu9250Interface: Mpu9250Interface

    private lateinit var mProps: Properties

    private var oscilloscope: Oscilloscope? = null
    private var magnetoViewer: MagnetoViewer? = null

    private fun enableDumpWindow(visible: Boolean) {
        if (visible) {
            textViewDumpTitle.visibility = View.VISIBLE
            textViewDump.visibility = View.VISIBLE
        } else {
            textViewDumpTitle.visibility = View.GONE
            textViewDump.visibility = View.GONE
        }
    }

    private fun setAccelRangeValue(spinnerItem: String) {
        mpu9250Interface.pause()
        mProps.accelRange = Mpu9250Interface.AccelRange.valueOf(spinnerItem)
        mProps.save()
        mpu9250Interface.setAccelRange(mProps.accelRange)
        mpu9250Interface.resume()
    }

    private fun setGyroRangeValue(spinnerItem: String) {
        mpu9250Interface.pause()
        mProps.gyroRange = Mpu9250Interface.GyroRange.valueOf(spinnerItem)
        mProps.save()
        mpu9250Interface.setGyroRange(mProps.gyroRange)
        mpu9250Interface.resume()
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE

        setContentView(R.layout.activity_analyzer)

        setTitle("Analyzer")

        fullscreen(window)

        textViewDump.movementMethod = ScrollingMovementMethod()

        mProps = Properties(this)

        surfaceView6axis.post {
            oscilloscope = Oscilloscope(
                surfaceView = surfaceView6axis, maxNumEntries = MAX_NUM_ENTRIES_6AXES
            )
        }

        surfaceView3axis.post {
            magnetoViewer = MagnetoViewer(
                surfaceView = surfaceView3axis,
                maxNumEntries = MAX_NUM_ENTRIES_3AXES,
                props = mProps
            )
        }

        buttonEdit.setOnClickListener {
            val dialog = Dialog(this)
            dialog.setContentView(R.layout.class_edit)
            val listViewLabels = dialog.findViewById<ListView>(R.id.listViewLabels)
            val buttonAdd = dialog.findViewById<Button>(R.id.buttonAdd)
            val buttonDelete = dialog.findViewById<Button>(R.id.buttonDelete)
            val editTextClassLabel = dialog.findViewById<EditText>(R.id.editTextClassLabel)

            var adapterClasses = ArrayAdapter(
                this,
                android.R.layout.simple_list_item_1,
                mProps.classLabels.toList()
            )
            listViewLabels.adapter = adapterClasses

            var posListViewLabels: Int? = null

            listViewLabels.setOnItemClickListener { _, _, position, _ ->
                posListViewLabels = position
            }

            buttonDelete.setOnClickListener {
                posListViewLabels?.let {
                    mProps.classLabels.removeAt(it)
                    adapterClasses = ArrayAdapter(
                        this,
                        android.R.layout.simple_list_item_1,
                        mProps.classLabels.toList()
                    )
                    listViewLabels.adapter = adapterClasses
                    mProps.save()
                }
            }

            buttonAdd.setOnClickListener {
                var classLabel = editTextClassLabel.text.toString()
                if (classLabel.matches(Regex("[A-Za-z0-9_]+"))) {
                    classLabel = classLabel.toCharArray()
                        .filter { it in 'a'..'z' || it in 'A'..'Z' || it in '0'..'9' || it == '_' }
                        .joinToString(separator = "") // Remove non-alphanumeric chars
                    if (!mProps.classLabels.contains(classLabel)) {
                        mProps.classLabels.add(classLabel)
                        adapterClasses = ArrayAdapter(
                            this,
                            android.R.layout.simple_list_item_1,
                            mProps.classLabels.toList()
                        )
                        listViewLabels.adapter = adapterClasses
                        mProps.save()
                    }
                }
            }

            dialog.setOnDismissListener {
                spinnerLabel.adapter = adapterClasses
                fullscreen(window)
            }

            dialog.show()
        }

        toggleButtonCapture.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) magnetoViewer?.clear()
        }

        spinnerAccelRange.setSelection(mProps.accelRange.ordinal)
        spinnerGyroRange.setSelection(mProps.gyroRange.ordinal)

        val pos = resources.getStringArray(R.array.magneto_magnification).toList().indexOf(mProps.magnetoMagnify.toString())
        spinnerMagnify.setSelection(pos)

        spinnerAccelRange.onItemSelectedListener =
            object : AdapterView.OnItemSelectedListener {
                override fun onNothingSelected(p0: AdapterView<*>?) = Unit
                override fun onItemSelected(
                    p0: AdapterView<*>?,
                    p1: View?,
                    p2: Int,
                    p3: Long
                ) {
                    setAccelRangeValue(spinnerAccelRange.selectedItem.toString())
                }
            }

        spinnerGyroRange.onItemSelectedListener =
            object : AdapterView.OnItemSelectedListener {
                override fun onNothingSelected(p0: AdapterView<*>?) = Unit
                override fun onItemSelected(
                    p0: AdapterView<*>?,
                    p1: View?,
                    p2: Int,
                    p3: Long
                ) {
                    setGyroRangeValue(spinnerGyroRange.selectedItem.toString())
                }
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
                    var magnetoMagnify = spinnerMagnify.selectedItem.toString()
                    mProps.magnetoMagnify = magnetoMagnify.toInt()
                    mProps.save()
                }
            }

        toggleButtonDump.setOnCheckedChangeListener { _, isChecked ->
            enableDumpWindow(isChecked)
        }

        buttonExit.setOnClickListener {
            finish()
        }

        enableDumpWindow(toggleButtonDump.isChecked)
    }

    override fun onResume() {
        super.onResume()

        mpu9250Interface = Mpu9250Interface(this, mProps.baudrate,
            object : IDataReceiver {
                override fun onMpu9250Data(data: Mpu9250Data) {
                    if (toggleButtonCapture.isChecked) {
                        textViewDump.post {
                            textViewDump.append(data.toString() + "\n")
                        }
                        oscilloscope?.update(data)
                    }
                }

                override fun onAk8963Data(data: Ak8963Data) {
                    if (toggleButtonCapture.isChecked) {
                        textViewDump.post {
                            textViewDump.append(data.toString() + "\n")
                        }
                        magnetoViewer?.update(data)
                    }
                }
            })

        setAccelRangeValue(spinnerAccelRange.selectedItem.toString())
        setGyroRangeValue(spinnerGyroRange.selectedItem.toString())
    }

    override fun onPause() {
        super.onPause()
        mpu9250Interface.destroy()
    }

}