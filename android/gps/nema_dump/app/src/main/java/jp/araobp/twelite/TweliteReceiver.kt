package jp.araobp.twelite

import android.content.Context
import android.util.Log
import jp.araobp.uart.UsbSerial

class TweliteReceiver(context: Context, val receiver: ITweiliteReceiver): UsbSerial(
        context, 115200, 8, 8, 0, 0) {

    companion object {
        private val TAG = javaClass.simpleName
    }

    private enum class State {
        HEADER_DETECTING,
        LEN,
        BODY_RECEIVING
    }

    private var mIdx = 0
    private var state:State = State.HEADER_DETECTING
    private var headerBytes = byteArrayOf(0xA5.toByte(), 0x5A.toByte(), 0x80.toByte())

    private var len: UInt = 0U
    private var body = ArrayList<Byte>()

    // TWELITE body parser
    private fun parseBody(size: Int): Object {
        val src = body[0].toUByte().toInt()
        val seq = body[2].toUByte().toInt()
        val lqi = body[11].toUByte().toInt()
        val len = body[13].toUByte().toInt()
        val data = body.slice(14 until size)
        return Message(src, seq, lqi, len, data.toByteArray()) as Object
    }

    // TWELITE parser
    override fun parse(messageFraction: ByteArray, size: Int): Object? {
        var o: Object? = null

        for (i in 0 until size) {
            val b = messageFraction[i]

            when (state) {
                State.HEADER_DETECTING -> {
                    if (b == headerBytes[mIdx]) {
                        if (mIdx++ == 2) {
                            state = State.LEN
                        }
                    } else {
                        mIdx = 0
                    }
                }

                State.LEN -> {
                    len = b.toUInt() + 4U  // including the length of header
                    mIdx++
                    body = ArrayList()
                    state = State.BODY_RECEIVING
                }

                State.BODY_RECEIVING -> {
                    body.add(b)
                    if (mIdx.toUInt() >= len) {
                        o = parseBody((len - 4U).toInt())
                        mIdx = 0
                        len = 0U
                        state = State.HEADER_DETECTING
                    }
                    mIdx++
                }
            }
        }
        return o
    }

    override fun onData(data: Object) {
        Log.d(TAG, data.toString())
        receiver.onMessage(data as Message)
    }

}