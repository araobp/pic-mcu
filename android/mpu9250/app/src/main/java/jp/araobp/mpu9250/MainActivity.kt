package jp.araobp.mpu9250

import android.app.Dialog
import android.content.Context
import android.content.Intent
import android.content.pm.ActivityInfo
import android.net.wifi.WifiManager
import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.widget.EditText
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import java.math.BigInteger
import java.net.InetAddress


class MainActivity : AppCompatActivity() {

    companion object {
            val TAG: String = this::class.java.simpleName
            const val BAUDRATE = "baudrate"
        }

    private lateinit var mProps: Properties

    // [Reference] https://stackoverflow.com/questions/20846120/alternative-for-formatter-formatipaddressint
    private fun ipAddress(): String {
        val wifiManager =
            applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        val wifiinfo = wifiManager.connectionInfo
        val ipAddress = BigInteger.valueOf(wifiinfo.ipAddress.toLong()).toByteArray()
        return InetAddress.getByAddress(ipAddress.reversedArray()).hostAddress
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_main)

        mProps = Properties(this)

        textViewIpAddress.text = "IP address: ${ipAddress()}"

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