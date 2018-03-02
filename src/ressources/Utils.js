.pragma library

/**
 * @brief niceNumPrec Return 'nice' number and precision suggestion from input.
 * @param number Input number.
 * @param logMode Return result suitable for log scale.
 * @return Nice number and suggested precision.
 */
function niceNumPrec(number, logMode) {
    var exponent = Math.floor(Math.log(number) / Math.LN10);
    var fraction = number / Math.pow(10, exponent);
    var nicePrecision = -exponent;
    var nicefrac;
    if (fraction <= 1)        nicefrac = 1;
    else if (fraction <= 2)   nicefrac = 2;
    else if (fraction <= 2.5) {nicefrac = 2.5; nicePrecision += 1}
    else if (fraction <= 5)   nicefrac = 5;
    else                      {nicefrac = 10; nicePrecision -= 1}
    var niceNumber = nicefrac * Math.pow(10, exponent);
    if (logMode)
        return [Math.ceil(Math.max(1, niceNumber)), Math.max(nicePrecision, 0)]
    else
        return [niceNumber, Math.max(nicePrecision, 0)]
}
