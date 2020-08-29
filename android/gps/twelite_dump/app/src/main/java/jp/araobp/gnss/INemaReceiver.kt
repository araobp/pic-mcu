package jp.araobp.gnss

interface INemaReceiver {
    fun onNemaSentence(sentence: String)
}