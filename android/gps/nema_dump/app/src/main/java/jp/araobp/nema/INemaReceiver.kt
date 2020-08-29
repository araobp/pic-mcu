package jp.araobp.nema

interface INemaReceiver {
    fun onNemaSentence(src: Int, seq: Int, lqi: Int, len: Int, sentence: Any?)
}