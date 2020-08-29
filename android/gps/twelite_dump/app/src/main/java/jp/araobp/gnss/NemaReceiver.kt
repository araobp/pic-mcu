package jp.araobp.gnss

import android.content.Context
import android.util.Log
import jp.araobp.uart.UsbSerial

class NemaReceiver(context: Context, val receiver: INemaReceiver): UsbSerial(context, 9600, 8, 8, 0, 0) {

    companion object {
        private val TAG = javaClass.simpleName
    }

    private val mReadBuf = CharArray(UsbSerial.READBUF_SIZE)
    private var mHead = false
    private var mIdx = 0

    // NEMA sentence parser
    override fun parse(messageFraction: ByteArray, len: Int): Object? {
        var o: Object? = null
        for (i in 0 until len) {
            val c = messageFraction[i].toChar()
            if (c == '\n') {
                o = mReadBuf.slice(0 until mIdx).joinToString("") as Object
                mIdx = 0
            } else {
                mReadBuf[mIdx++] = c
            }
        }
        return o
    }

    override fun onData(data: Object) {
        Log.d(TAG, data as String)
        receiver.onNemaSentence(data as String)
    }

}