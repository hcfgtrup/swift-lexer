# Swift Lexer (C++ → Swift)

Лексический анализатор языка Swift, написанный на C++17.

## Возможности
- Подсчёт общего числа лексем
- Абсолютная и относительная частота по типам
- Перечень идентификаторов в лексикографическом порядке
- Диагностика ошибок

## Сборка локально
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
cmake --build .

##Тестовые файлы

Все примеры лежат в `examples/`:

|Файл |Что проверяет |Ожидаемый результат |
|-----|--------------|---------------------|
|`01_basic.swift` |Реальный Swift-код: функции, классы, структуры, условия |~140 лексем, 0 ошибок |
|`02_numbers.swift` |Все формы чисел: dec, hex, oct, bin, float, exp, hex-float |~95 лексем, 0 ошибок |
|`03_operators.swift` |Все операторы и разделители |~110 лексем, 0 ошибок |
|`04_strings_comments.swift` |Строки, экранирование, многострочные, вложенные комментарии |~50 лексем, 0 ошибок |
|`05_errors_soft.swift` |Мягкие ошибки (UNKNOWN) — программа продолжает разбор |5 ошибок, разбор не падает |
|`06_errors_fatal.swift` |Фатальная ошибка (незакрытая строка) — файл пропускается |1 фатальная ошибка |

##Запуск всех тестов

```bash
cd build
lexer.exe ..\examples\01_basic.swift
lexer.exe ..\examples\02_numbers.swift
lexer.exe ..\examples\03_operators.swift
lexer.exe ..\examples\04_strings_comments.swift
lexer.exe ..\examples\05_errors_soft.swift
lexer.exe ..\examples\06_errors_fatal.swift
lexer.exe ..\examples\*.swift

## Использование
lexer.exe examples\hello.swift


## Docker
docker build -t swift-lexer .
docker run --rm -v ${PWD}/examples:/data swift-lexer /data/hello.swift
