// Проверяем числа во всех формах, которые определены в спецификации Swift
// Целые, дробные, экспонента, hex, oct, bin, hex-float, разделители

let decimal = 42
let negative = -100
let zero = 0
let big = 1_000_000
let float = 3.14
let negFloat = -2.71828
let exp = 1e10
let negExp = 2.5e-3
let hex = 0xFF
let hexBig = 0xDEADBEEF
let octal = 0o17
let binary = 0b1010
let hexFloat = 0xFp2
let hexFloatNeg = 0xFp-2
let hexFloatFrac = 0x1.8p3

var result = decimal + big
result = result * 2
result = result - 100
result = result / 3
result = result % 7