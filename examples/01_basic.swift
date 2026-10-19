// Пример базового кода на Swift: функции, переменные, условия
// Проверяем: ключевые слова, идентификаторы, простые операторы

import Foundation

let pi = 3.14159
var counter: Int = 0
let message = "Hello, Swift!"

func greet(name: String) -> String {
    return "Привет, \(name)!"
}

func factorial(n: Int) -> Int {
    if n <= 1 {
        return 1
    } else {
        return n * factorial(n: n - 1)
    }
}

struct Point {
    var x: Double
    var y: Double

    func distance(to other: Point) -> Double {
        let dx = x - other.x
        let dy = y - other.y
        return sqrt(dx * dx + dy * dy)
    }
}

class Animal {
    var name: String
    init(name: String) {
        self.name = name
    }
    func speak() -> String {
        return "..."
    }
}

for i in 1...10 {
    counter += i
}

while counter > 0 {
    counter -= 1
    if counter == 5 {
        break
    }
}

print(factorial(n: 5))
print(greet(name: "Мир"))