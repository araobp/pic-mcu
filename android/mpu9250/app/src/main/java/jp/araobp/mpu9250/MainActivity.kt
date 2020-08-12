package jp.araobp.mpu9250

import android.app.Dialog
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.widget.EditText
import kotlinx.android.synthetic.main.activity_main.*
import java.lang.NumberFormatException

class MainActivity : AppCompatActivity() {

    companion object {
            val TAG: String = this::class.java.simpleName
            const val BAUDRATE = "baudrate"
        }

    private lateinit var mProps: Properties

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        mProps = Properties(this)

        // Settings dialog
        buttonSettings.setOnClickListener {

            mProps.load()

            val dialog = Dialog(this)
            dialog.setContentView(R.layout.settings)

            val editTextBaudrate = dialog.findViewById<EditText>(R.id.editTextBaudrate)
            editTextBaudrate.setText(mProps.baudrate.toString())

            editTextBaudrate.addTextChangedListener(object : TextWatcher {
                override fun beforeTextChanged(p0: CharSequence?, p1: Int, p2: Int, p3: Int) =
                    Unit

                override fun onTextChanged(p0: CharSequence?, p1: Int, p2: Int, p3: Int) = Unit
                override fun afterTextChanged(p0: Editable?) {
                    try {
                        mProps.baudrate = editTextBaudrate.text.toString().toInt()
                    } catch (e: NumberFormatException) {  // ""
                        mProps.baudrate = 0
                    }
                }
            })
            
            dialog.setOnDismissListener {
                mProps.save()
            }

            dialog.show()
        }

        buttonAccelerometer.setOnClickListener {
            val intent = Intent(this, AccelGyroActivity::class.java).apply {
                putExtra(BAUDRATE, mProps.baudrate)
            }
            startActivity(intent)
        }
    }
}