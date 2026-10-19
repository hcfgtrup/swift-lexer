// Проверяем операторы: арифметические, сравнения, логические, диапазоны
// и разделители: скобки, запятые, точки с запятой, двоеточия

let a = 10
let b = 20
let c = a + b - 5 * 2 / 3 % 4

let equal = a == b
let notEqual = a != b
let less = a < b
let greater = a > b
let lessEq = a <= b
let greaterEq = a >= b

let logic1 = true && false || true
let logic2 = !logic1

let range1 = 1...10
let range2 = 1..<10

let opt: Int? = nil
let value = opt ?? 0

let tuple = (1, 2, 3)
let first = tuple.0

let dict = ["a": 1, "b": 2, "c": 3]
let arr = [10, 20, 30, 40]

let combined = a += 5