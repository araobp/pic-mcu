package jp.araobp.mpu9250

import android.app.Dialog
import android.content.pm.ActivityInfo
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.view.KeyEvent
import android.view.View
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import jp.araobp.mpu9250.analyzer.FeatureCollector
import jp.araobp.mpu9250.analyzer.IMagnetoEventListener
import jp.araobp.mpu9250.analyzer.MagnetoViewer
import jp.araobp.mpu9250.analyzer.Oscilloscope
import jp.araobp.mpu9250.serial.Ak8963Data
import jp.araobp.mpu9250.serial.IDataReceiver
import jp.araobp.mpu9250.serial.Mpu9250Data
import jp.araobp.mpu9250.serial.Mpu9250Interface
import kotlinx.android.synthetic.main.activity_analyzer.*

class AnalyzerActivity : AppCompatActivity() {

    companion object {
        const val Fs = 80  // 80 Hz
        const val MAX_NUM_ENTRIES_6AXES = 256
        const val MAX_NUM_ENTRIES_3AXES = 128
        const val DATA_COLLECTION_DURATION = 3  // 3 sec
        const val FEATURE_COLLECTOR_ENTRIES = 260
    }

    private lateinit var mpu9250Interface: Mpu9250Interface

    private lateinit var mProps: Properties

    private var mOscilloscope: Oscilloscope? = null
    private var mMagnetoViewer: MagnetoViewer? = null
    private lateinit var featureCollector: FeatureCollector

    private var mSave = false

    private fun enableDumpWindow(visible: Boolean) {
        if (visible) {
            textViewOutputTitle.visibility = View.VISIBLE
            textViewOutput.visibility = View.VISIBLE
        } else {
            textViewOutputTitle.visibility = View.GONE
            textViewOutput.visibility = View.GONE
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

    private fun classLabel() = spinnerClassLabels.selectedItem.toString()

    private fun updateFeatureFileCnt() {
        val classLabel = classLabel()
        textViewCnt.post {
            textViewCnt.text =
                "${featureCollector.fileCntPerLabel(classLabel)}/${featureCollector.fileCnt()}"
        }
    }

    private fun startSaving() {
        textViewSave.isEnabled = true
        featureCollector.getReady(
            classLabel(),
            FEATURE_COLLECTOR_ENTRIES,
            mProps.accelRange,
            mProps.gyroRange
        )
        mSave = true
    }

    override fun dispatchKeyEvent(event: KeyEvent): Boolean {
        if (event.keyCode == KeyEvent.KEYCODE_VOLUME_UP && event.action == KeyEvent.ACTION_DOWN) {
            startSaving()
        }
        return true
    }

    override fun onCreate(savedInstanceState: Bundle?) {

        super.onCreate(savedInstanceState)

        requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT

        setContentView(R.layout.activity_analyzer)

        setTitle("Analyzer")

        supportActionBar?.let {
            it.setDisplayHomeAsUpEnabled(true) //show back button
        }

        fullscreen(window)

        textViewOutput.movementMethod = ScrollingMovementMethod()

        mProps = Properties(this)

        featureCollector = FeatureCollector(this)

        surfaceView6axis.post {
            mOscilloscope = Oscilloscope(
                surfaceView = surfaceView6axis, maxNumEntries = MAX_NUM_ENTRIES_6AXES
            )
        }

        surfaceView3axis.post {
            mMagnetoViewer = MagnetoViewer(
                surfaceView = surfaceView3axis,
                maxNumEntries = MAX_NUM_ENTRIES_3AXES,
                props = mProps
            )
        }

        val adapterClasses =
            ArrayAdapter(this, android.R.layout.simple_spinner_item, mProps.classLabels)
        adapterClasses.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        spinnerClassLabels.adapter = adapterClasses

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
                spinnerClassLabels.adapter = adapterClasses
                fullscreen(window)
            }

            dialog.show()
        }

        toggleButtonCapture.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) mMagnetoViewer?.clear()
        }

        spinnerAccelRange.setSelection(mProps.accelRange.ordinal)
        spinnerGyroRange.setSelection(mProps.gyroRange.ordinal)

        val pos = resources.getStringArray(R.array.magneto_magnification).toList()
            .indexOf(mProps.magnetoMagnify.toString())
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

        buttonCalibrate.setOnClickListener {
            buttonCalibrate.isEnabled = false

            mMagnetoViewer?.startCalibration(
                object : IMagnetoEventListener {
                    override fun onCalibrationFinished() {
                        buttonCalibrate.post {
                            buttonCalibrate.isEnabled = true
                        }
                    }
                }
            )
        }

        toggleButtonDump.setOnCheckedChangeListener { _, isChecked ->
            enableDumpWindow(isChecked)
        }

        enableDumpWindow(toggleButtonDump.isChecked)
    }

    override fun onResume() {
        super.onResume()

        mpu9250Interface = Mpu9250Interface(this, mProps.baudrate,
            object : IDataReceiver {
                override fun onMpu9250Data(data: Mpu9250Data) {
                    if (toggleButtonCapture.isChecked) {
                        textViewOutput.post {
                            textViewOutput.append(data.toString() + "\n")
                        }
                        mOscilloscope?.update(data)
                    }
                    if (mSave) {
                        if (featureCollector.add(data)) {
                            mSave = false
                            textViewSave.post {
                                textViewSave.isEnabled = false
                            }
                            updateFeatureFileCnt()
                        }
                    }
                }

                override fun onAk8963Data(data: Ak8963Data) {
                    if (toggleButtonCapture.isChecked) {
                        textViewOutput.post {
                            textViewOutput.append(data.toString())
                        }
                        mMagnetoViewer?.update(data)
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

    override fun onSupportNavigateUp(): Boolean {
        finish()
        return true
    }
}