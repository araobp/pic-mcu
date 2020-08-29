package jp.araobp.gps_dump

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.text.method.ScrollingMovementMethod
import kotlinx.android.synthetic.main.activity_main.*
import androidx.appcompat.app.AppCompatActivity
import jp.araobp.nema.GpsStatus
import jp.araobp.nema.INemaReceiver
import jp.araobp.nema.NemaGpgga
import jp.araobp.nema.NemaSentenceReceiver
import jp.araobp.twelite.R
import java.util.*
import java.util.concurrent.LinkedBlockingDeque
import java.util.concurrent.atomic.AtomicInteger
import kotlin.concurrent.scheduleAtFixedRate

class MainActivity : AppCompatActivity() {

    companion object {
        const val TIMEOUT = 60;
    }

    lateinit var mNemaSentenceReceiver: NemaSentenceReceiver

    private enum class ReceiverStatus {
        INACTIVE,
        STARTING,
        ACTIVE
    }

    private val latitudeHistory = LinkedBlockingDeque<Double>(10)
    private val longitudeHistory = LinkedBlockingDeque<Double>(10)

    private var receiverStatus = ReceiverStatus.INACTIVE

    // Default coordinates (Tokyo tower)
    private var lastLatitude = 35.6585805
    private var lastLongitude = 139.7432442

    private var timer = Timer()
    private var expire = AtomicInteger(0)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_main)

        textView.movementMethod = ScrollingMovementMethod()
        textViewStatus.text = receiverStatus.toString()

        mNemaSentenceReceiver = NemaSentenceReceiver(this,
                object : INemaReceiver {
                    override fun onNemaSentence(src: Int, seq: Int, lqi: Int, len: Int, sentence: Any?) {
                        if (receiverStatus == ReceiverStatus.INACTIVE) {
                            receiverStatus = ReceiverStatus.STARTING
                            textViewStatus.post {
                                textViewStatus.text = receiverStatus.toString()
                            }
                        }
                        sentence?.let {
                            when (it) {

                                is NemaGpgga -> {
                                    if (receiverStatus == ReceiverStatus.STARTING) {
                                        receiverStatus = ReceiverStatus.ACTIVE
                                        textViewStatus.post {
                                            textViewStatus.text = receiverStatus.toString()
                                        }
                                    }

                                    textView.post {
                                        val localDateTime = localDateTime()
                                        textView.append(">>> TWELITE $localDateTime\n")
                                        textView.append("src: $src, seq: $seq, lqi: $lqi, len: $len\n")
                                        textView.append(">>> \$GPGGA $localDateTime\n")
                                        textView.append(it.toString() + "\n\n")
                                    }

                                    // For calling Google map after this
                                    if (latitudeHistory.remainingCapacity() <= 0) latitudeHistory.take()
                                    latitudeHistory.add(it.latitude.toDouble())
                                    if (longitudeHistory.remainingCapacity() <= 0) longitudeHistory.take()
                                    longitudeHistory.add(it.longitude.toDouble())

                                    lastLatitude = it.latitude.toDouble()
                                    lastLongitude = it.longitude.toDouble()

                                }

                                is GpsStatus -> {
                                    textView.post {
                                        val localDateTime = localDateTime()
                                        textView.append(">>> TWELITE $localDateTime\n")
                                        textView.append("src: $src, seq: $seq, lqi: $lqi, len: $len\n")
                                        textView.append(">>> \$GPSTS $localDateTime\n")
                                        textView.append(it.toString() + "\n\n")
                                    }

                                    // Activity monitoring
                                    expire.set(TIMEOUT)
                                }
                            }
                        }
                    }
                }
        )

        buttonGoogleMap.setOnClickListener {
            var la: Double
            var lo: Double
            if (buttonMovingAverage.isChecked) {
                la = latitudeHistory.average()
                lo = longitudeHistory.average()
            } else {
                la = lastLatitude
                lo = lastLongitude
            }

            val gmmIntentUri = Uri.parse("geo:0,0?q=$la,$lo(GPS Tracker)")
            val mapIntent = Intent(Intent.ACTION_VIEW, gmmIntentUri)
            mapIntent.setPackage("com.google.android.apps.maps")
            mapIntent.resolveActivity(packageManager)?.let {
                startActivity(mapIntent)
            }
        }

        timer.scheduleAtFixedRate(1000, 1000) {
            val t = expire.get() - 1
            expire.set(t)
            if (t <= 0) receiverStatus = ReceiverStatus.INACTIVE
            textViewStatus.post {
                textViewStatus.text = receiverStatus.toString()
            }
        }
    }

    override fun onDestroy() {
        mNemaSentenceReceiver.destroy()
        super.onDestroy()
    }

}
