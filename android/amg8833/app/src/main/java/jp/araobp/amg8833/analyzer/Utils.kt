package jp.araobp.amg8833.analyzer

import android.graphics.Color
import org.opencv.core.CvType
import org.opencv.core.Mat
import org.opencv.core.Size
import org.opencv.imgproc.Imgproc
import org.opencv.imgproc.Imgproc.INTER_CUBIC
import org.opencv.imgproc.Imgproc.resize
import kotlin.math.roundToInt

val MAX_VALUE = UByte.MAX_VALUE.toFloat()
val TEMP_SCALE = 0.25F

fun tempList(data: UByteArray) = data.map { (it.toFloat() * TEMP_SCALE).roundToInt().toString() }.toList()

fun minMaxNormalize(src: UByteArray): UByteArray {

    val dst = UByteArray(src.size)
    var min = UByte.MAX_VALUE.toFloat()
    var max = UByte.MIN_VALUE.toFloat()

    src.forEachIndexed { idx, _ ->
        val d = src[idx].toFloat()
        if (d > max) max = d
        if (d < min) min = d
    }

    val minmax = max - min

    src.forEachIndexed { idx, _ ->
        val f = src[idx].toFloat()
        dst[idx] = (MAX_VALUE * (f - min) / minmax).toUInt().toUByte()
    }

    return dst
}

fun UByteArray.toMat(): Mat {
    val mat = Mat(8, 8, CvType.CV_8U)
    mat.put(0, 0, this.toByteArray())
    return mat
}

fun convertToUByteArray(mat: Mat): UByteArray {
    val dst = ByteArray(mat.width() * mat.height())
    mat.get(0, 0, dst)
    return dst.toUByteArray()
}

/*
 * Interpolate image
 *
 * The reason why I use this function instead of cv::resize() is that
 * the original image (8x8) is too small for bicubic interpolation.
 * I found that repeating bicubic interpolation works well.
 */
fun interpolate(src: Mat, repeat: Int): Mat {
    val dst = src.clone()
    for (i in 1 .. repeat) {
        resize(dst, dst, Size(dst.cols()*4.0, dst.rows()*4.0), 0.0, 0.0, INTER_CUBIC);
    }
    return dst
}