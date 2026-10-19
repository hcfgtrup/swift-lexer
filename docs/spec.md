#спецификация языка Swift (лексическая структура)

##источник

Полная спецификация лексической структуры Swift приложена в файле
`docs/swift-specification.pdf` — это выдержка из официальной книги Apple
«The Swift Programming Language», глава **Lexical Structure**.
Источник: https://docs.swift.org/swift-book/documentation/the-swift-programming-language/lexicalstructure/

##краткая выжимка — какие лексемы мы распознаём

|Тип |Примеры |
|-----|---------|
|Ключевое слово |`let`, `var`, `func`, `if`, `else`, `guard`, `await`, `async` |
|Идентификатор |`x`, `myFunc`, `_temp`, `` `class` `` (raw-идентификатор) |
|Число |`42`, `3.14`, `0xFF`, `0b1010`, `0o17`, `1e10`, `0xFp2` (hex-float), `1_000_000` |
|Строка |`"hello"`, `"a\nb"`, `"""многострочная"""` |
|Оператор |`+`, `==`, `->`, `??`, `...`, `..<` |
|Разделитель |`(`, `)`, `{`, `}`, `[`, `]`, `,`, `;`, `:` |

##ключевые слова

Список полностью соответствует официальной спецификации (см. PDF, раздел
"keywords and punctuation"). Всего около **100 ключевых слов**, разбитых
на 4 категории:

1. **объявления:** `associatedtype`, `borrowing`, `class`, `consuming`, `deinit`,
   `enum`, `extension`, `fileprivate`, `func`, `import`, `init`, `inout`,
   `internal`, `let`, `nonisolated`, `open`, `operator`, `precedencegroup`,
   `private`, `protocol`, `public`, `rethrows`, `static`, `struct`,
   `subscript`, `typealias`, `var`.
2. **инструкции:** `break`, `case`, `catch`, `continue`, `default`, `defer`,
   `do`, `else`, `fallthrough`, `for`, `guard`, `if`, `in`, `repeat`,
   `return`, `throw`, `switch`, `where`, `while`.
3. **выражения и типы:** `Any`, `as`, `await`, `false`, `is`, `nil`, `self`,
   `Self`, `super`, `throws`, `true`, `try`.
4. **контекстные:** `associativity`, `async`, `convenience`, `didSet`,
   `dynamic`, `final`, `get`, `indirect`, `infix`, `lazy`, `left`,
   `mutating`, `none`, `nonmutating`, `optional`, `override`, `package`,
   `postfix`, `precedence`, `prefix`, `Protocol`, `required`, `right`,
   `set`, `some`, `Type`, `unowned`, `weak`, `willSet`.

##комментарии

- `//однострочный` — до конца строки.
- `/*многострочный*/` — поддерживает **вложенность** (`/*/*...*/*/`).

##ошибки лексического уровня

- **неизвестный символ** — попадает в отчёт как `Ошибка` (мягкая, разбор продолжается).
- **незакрытая строка** — фатальная ошибка, файл пропускается.
- **незакрытый многострочный комментарий** — фатальная ошибка, файл пропускается.
- **незакрытый raw-идентификатор** `` `...` `` — фатальная ошибка, файл пропускается.

##что НЕ поддержано и почему

1. **регулярные выражения `/.../`.** лексически неотличимы от операции деления.
   различение — задача синтаксического анализатора (нужен контекст: стоит ли `/`
   в позиции, где ожидается операнд, или между операндами). в рамках лабораторной
   работы по лексическому анализу не реализуем.
2. **Unicode-идентификаторы** (например, `let имя = 5`). требуют посимвольного
   разбора UTF-8 или использования `std::wstring` с правильной локалью. для
   базового лексического анализатора достаточно ASCII.
3. **extended-строки** `#"..."#` и `##"..."##`. редкий синтаксис, требует
   подсчёта количества `#` вокруг открывающей и закрывающей кавычек.
4. **интерполяция строк** `"x = \(x)"`. требует лексического анализа
   вложенного выражения внутри строки — это выходит за рамки задачи.
5. **ключевые слова с префиксом `#`** (`#if`, `#available`, `#selector`).
   лексер распознаёт `#` как отдельный оператор, а следующее слово — как
   идентификатор. для подсчёта статистики этого достаточно.

##команды для проверки

```bash
#локально
lexer.exe examples/hello.swift
lexer.exe examples/errors.swift
lexer.exe examples/unknown.swift

#docker
docker run --rm -v %cd%/examples:/data swift-lexer /data/hello.swift