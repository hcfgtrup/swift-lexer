// Проверяем мягкие ошибки — неизвестные символы.
// Лексер их распознаёт, считает как UNKNOWN и продолжает разбор.

let good = 1
let bad1 = $
let bad2 = \
let bad3 = $
let stillGood = 2
let bad4 = $
let bad5 = \
let tail = 3