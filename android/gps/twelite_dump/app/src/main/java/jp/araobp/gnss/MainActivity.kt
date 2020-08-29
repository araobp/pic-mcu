package jp.araobp.gnss

import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    lateinit var mNemaReceiver: NemaReceiver

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_main)

        textView.movementMethod = ScrollingMovementMethod()

        mNemaReceiver = NemaReceiver(this,
            object: INemaReceiver {
               override fun onNemaSentence(sentence: String) {
                   Log.d("onNemaSentence", sentence)
                   var text: String? = null
                   if (toggleButtonFilter.isChecked) {
                       if (sentence.startsWith("\$GPGGA")) text = sentence
                   } else {
                       text = sentence
                   }
                   text?.let {
                       runOnUiThread {
                           textView.append(
                                   it + '\n'
                           )
                       }
                   }
               }
            })
    }

    override fun onDestroy() {
        mNemaReceiver.destroy()
        super.onDestroy()
    }

}
