package jp.araobp.nema

import android.content.Context
import android.util.Log
import jp.araobp.twelite.ITweiliteReceiver
import jp.araobp.twelite.Message
import jp.araobp.twelite.TweliteReceiver
import java.lang.Exception

class NemaSentenceReceiver(val context: Context, receiver: INemaReceiver) {

    companion object {
        private val TAG = javaClass.simpleName
        const val GPGGA = "\$GPGGA"
        const val GPSTS = "\$GPSTS"
    }

    val mTweliteReceiver = TweliteReceiver(context,
            object : ITweiliteReceiver {
                override fun onMessage(message: Message) {
                    val byteArray = message.data
                    val nemaSentence = parse(byteArray)
                    if (nemaSentence == null) {
                        Log.d("NemaParser", "GPS module starting...")
                    } else {
                        Log.d("NemaParser", nemaSentence.toString())
                    }

                    receiver.onNemaSentence(
                            message.src,
                            message.seq,
                            message.lqi,
                            message.len,
                            nemaSentence
                    )
                }
            }
    )

    fun destroy() {
        mTweliteReceiver.destroy()
    }

    fun parse(byteArray: ByteArray): Any? {
        var parsedObject: Any? = null
        val sentence = String(byteArray).split(",")
        try {
            when (sentence[0]) {
                GPGGA -> {
                    parsedObject = NemaGpgga(
                            utcOfPosition = sentence[1],
                            latitude = sentence[2].toGooglMapCoordsF(),
                            northOrSouth = if (sentence[3] == "N") Direction.NORTH else Direction.SOUTH,
                            longitude = sentence[4].toGooglMapCoordsF(),
                            eastOrWest = if (sentence[5] == "E") Direction.EAST else Direction.WEST,
                            gpsQualityIndicator = sentence[6].toInt(),
                            numberOfSatelitesInUse = sentence[7].toInt(),
                            horizontalDilusionOfPosition = sentence[8].toFloat(),
                            antennaAltitudeAboveBelowMeanSeaLevel = sentence[9].toFloat(),
                            anntenaHeightUnit = sentence[10],
                            geoidalSeperation = sentence[11].toFloat(),
                            geoidalSeperationUnit = sentence[12],
                            ageInSecondsSinceLastUpdate = if (sentence[13] != "") sentence[13].toFloat() else 0F
                    )
                }

                GPSTS -> {
                    parsedObject = GpsStatus(
                            fixed = sentence[1],
                            elapsedTime = sentence[2].toInt()
                    )
                }

                /*
            GPRMC -> {
                parsedObject = gprmc(
                        timestamp = sentence[1],
                        validity = sentence[2],
                        currentLatitude = sentence[3].toFloat(),
                        northOrSouth = if (sentence[4] == "N") Direction.NORTH else Direction.SOUTH,
                        currentLongitude = sentence[5].toFloat(),
                        eastOrWest = if (sentence[6] == "E") Direction.EAST else Direction.WEST,
                        speedInKnots = sentence[7].toFloat(),
                        trueCourse = sentence[8].toFloat(),
                        dateStamp = sentence[9],
                        variation = sentence[10].toFloat(),
                        eastOrWestVariation = if (sentence[6] == "E") Direction.EAST else Direction.WEST
                )
            }
            */
                else -> {
                    // NOP
                }
            }
        } catch (e: Exception) {
            Log.d(TAG, e.toString())
        }

        return parsedObject

    }

}