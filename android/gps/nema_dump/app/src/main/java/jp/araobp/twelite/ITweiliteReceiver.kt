package jp.araobp.twelite

interface ITweiliteReceiver {
    fun onMessage(message: Message)
}