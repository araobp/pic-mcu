package jp.araobp.twelite

import android.content.Context
import jp.araobp.usbserial.FtdiDriver
import kotlin.experimental.xor

class TweliteDriver(context: Context, val receiver: ITweiliteReceiver) : FtdiDriver(
    context, 115200, 8, 8, 0, 0
) {

    companion object {
        private val TAG = javaClass.simpleName

        val BYTE = 0xA0.toByte()
        val RESPONSE_MSG_DISABLED = 0x07.toByte()
        val ACK_ENABLED = 0x01.toByte()
        val RESEND = 0x02.toByte()
        val TERMINATOR = 0xFF.toByte()
        val EOT = 0x04.toByte()

        val NUM_RETRY = 0x03.toByte()

    }

    private enum class State {
        HEADER_DETECTING,
        LEN,
        BODY_RECEIVING
    }

    private var mIdx = 0
    private var mState: State = State.HEADER_DETECTING
    private var mHeaderBytes = byteArrayOf(0xA5.toByte(), 0x5A.toByte(), 0x80.toByte())

    private var mLen: UInt = 0U
    private var mBody = ArrayList<Byte>()

    // TWELITE body parser
    private fun parseBody(size: Int): TweliteMessage {
        val src = mBody[0].toByte()
        val seq = mBody[2].toByte()
        val lqi = mBody[11].toByte()
        val len = mBody[13].toByte()
        val data = mBody.slice(14 until size)
        return TweliteMessage(src, seq, lqi, len, data.toByteArray())
    }

    // TWELITE parser
    override fun parse(messageFraction: ByteArray, size: Int) {

        for (i in 0 until size) {
            val b = messageFraction[i]

            when (mState) {
                State.HEADER_DETECTING -> {
                    if (b == mHeaderBytes[mIdx]) {
                        if (mIdx++ == 2) {
                            mState = State.LEN
                        }
                    } else {
                        mIdx = 0
                    }
                }

                State.LEN -> {
                    mLen = b.toUInt() + 4U  // including the length of header
                    mIdx++
                    mBody = ArrayList()
                    mState = State.BODY_RECEIVING
                }

                State.BODY_RECEIVING -> {
                    mBody.add(b)
                    if (mIdx.toUInt() >= mLen) {
                        val message = parseBody((mLen - 4U).toInt())
                        mIdx = 0
                        mLen = 0U
                        mState = State.HEADER_DETECTING
                        receiver.onMessage(message)
                    }
                    mIdx++
                }
            }
        }
    }

    private var seq = 0.toUByte()

    fun send(dst: Byte, message: ByteArray) {
        val twMessage = ArrayList<Byte>()

        twMessage.add(0xA5.toByte())
        twMessage.add(0x5A.toByte())
        twMessage.add(0x80.toByte())
        twMessage.add((message.size + 8).toByte())
        twMessage.add(dst)
        twMessage.add(BYTE)
        twMessage.add(seq.toByte())
        twMessage.add(RESPONSE_MSG_DISABLED)
        twMessage.add(ACK_ENABLED)  // ACK enabled
        twMessage.add(RESEND)  // Resend
        twMessage.add(NUM_RETRY)  // The number of retries
        twMessage.add(TERMINATOR)  // Terminator

        var cs =
            dst xor BYTE xor seq.toByte() xor RESPONSE_MSG_DISABLED xor ACK_ENABLED xor RESEND xor NUM_RETRY xor TERMINATOR

        message.forEach {
            twMessage.add(it)
            cs = cs xor it
        }

        twMessage.add(cs)

        tx(twMessage.toByteArray())

        seq++
    }

}