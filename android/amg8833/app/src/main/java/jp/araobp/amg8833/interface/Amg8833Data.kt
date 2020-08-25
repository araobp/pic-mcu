package jp.araobp.amg8833.`interface`

data class Amg8833Data (val data: ByteArray) {

    fun clone(): Amg8833Data {
        return Amg8833Data(data.clone())
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as Amg8833Data

        if (!data.contentEquals(other.data)) return false

        return true
    }

    override fun hashCode(): Int {
        return data.contentHashCode()
    }
}