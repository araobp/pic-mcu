package jp.araobp.twelite


data class Message(
        val src: Int,
        val seq: Int,
        val lqi: Int,
        val len: Int,
        val data: ByteArray
) {
    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Message

        if (src != other.src) return false
        if (seq != other.seq) return false
        if (lqi != other.lqi) return false
        if (len != other.len) return false
        if (!data.contentEquals(other.data)) return false

        return true
    }

    override fun hashCode(): Int {
        var result = src.hashCode()
        result = 31 * result + seq.hashCode()
        result = 31 * result + lqi.hashCode()
        result = 31 * result + len.hashCode()
        result = 31 * result + data.contentHashCode()
        return result
    }
}