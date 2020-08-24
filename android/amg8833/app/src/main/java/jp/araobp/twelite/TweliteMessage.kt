package jp.araobp.twelite


data class TweliteMessage(
        val src: Byte,
        val seq: Byte,
        val lqi: Byte,
        val len: Byte,
        val data: ByteArray
)