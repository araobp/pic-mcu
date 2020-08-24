package jp.araobp.twelite

interface ITweiliteReceiver {
    fun onMessage(message: TweliteMessage)
}