package jp.araobp.nema

data class NemaGpgga(
        val utcOfPosition: String,
        val latitude: Float,
        val northOrSouth: Direction,
        val longitude: Float,
        val eastOrWest: Direction,
        val gpsQualityIndicator: Int,
        val numberOfSatelitesInUse: Int,
        val horizontalDilusionOfPosition: Float,
        val antennaAltitudeAboveBelowMeanSeaLevel: Float,
        val anntenaHeightUnit: String,
        val geoidalSeperation: Float,
        val geoidalSeperationUnit: String,
        val ageInSecondsSinceLastUpdate: Float
)

// data class gprmc(
// val timestamp: String,
// val validity: String,
// val currentLatitude: Float,
// val northOrSouth: Direction,
// val currentLongitude: Float,
// val eastOrWest: Direction,
// val speedInKnots: Float,
// val trueCourse: Float,
// val dateStamp: String,
// val variation: Float,
// val eastOrWestVariation: Direction
// )