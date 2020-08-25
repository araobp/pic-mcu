package jp.araobp.amg8833.analyzer

import android.graphics.Color
import org.opencv.core.CvType
import org.opencv.core.Mat
import org.opencv.core.Size
import org.opencv.imgproc.Imgproc
import org.opencv.imgproc.Imgproc.*
import kotlin.math.roundToInt

val MAX_VALUE = UByte.MAX_VALUE.toFloat()
val TEMP_SCALE = 0.25F
val BIN_THRES = 130.0

internal fun tempList(data: UByteArray) = data.map { (it.toFloat() * TEMP_SCALE).roundToInt().toString() }.toList()

internal fun minMaxNormalize(src: UByteArray): UByteArray {

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

internal fun UByteArray.toMat(rows: Int, cols: Int): Mat {
    check(rows * cols == this.size)

    val mat = Mat(rows, cols, CvType.CV_8U)
    mat.put(0, 0, this.toByteArray())
    return mat
}

internal fun convertToUByteArray(mat: Mat): UByteArray {
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
internal fun interpolate(src: Mat, repeat: Int): Mat {
    val dst = src.clone()
    for (i in 1 .. repeat) {
        resize(dst, dst, Size(dst.cols()*4.0, dst.rows()*4.0), 0.0, 0.0, INTER_CUBIC);
    }
    return dst
}

internal fun binarize(src: Mat): Mat {
    val dst = Mat()
    threshold(src, dst, BIN_THRES, 255.0, THRESH_BINARY);
    return dst
}