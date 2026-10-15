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

## Использование
lexer.exe examples\hello.swift


## Docker
docker build -t swift-lexer .
docker run --rm -v ${PWD}/examples:/data swift-lexer /data/hello.swift
