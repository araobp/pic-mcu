package jp.araobp.nema

/**
 * This is a private extension, not a NEMA standard sentence
 */
data class GpsStatus (
    val fixed: String,
    val elapsedTime: Int
)