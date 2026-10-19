#pragma once//защита от повторного включения заголовочного файла
#include <unordered_set>//подключаем структуру данных "хеш-сет" для быстрого поиска
#include <string>//подключаем работу со строками

//функция, возвращающая ссылку на статическое множество (unordered_set) ключевых слов языка Swift
inline const std::unordered_set<std::string>& swiftKeywords() {
    //объявляем и инициализируем константное множество строк, содержащее все ключевые слова Swift
    static const std::unordered_set<std::string> kw = {
        //ключевые слова объявлений
        "associatedtype", "borrowing", "class", "consuming", "deinit",
        "enum", "extension", "fileprivate", "func", "import", "init",
        "inout", "internal", "let", "nonisolated", "open", "operator",
        "precedencegroup", "private", "protocol", "public", "rethrows",
        "static", "struct", "subscript", "typealias", "var",

        //ключевые слова инструкций
        "break", "case", "catch", "continue", "default", "defer", "do",
        "else", "fallthrough", "for", "guard", "if", "in", "repeat",
        "return", "throw", "switch", "where", "while",

        //ключевые слова выражений и типов
        "Any", "as", "await", "false", "is", "nil", "self", "Self",
        "super", "throws", "true", "try",

        //ключевые слова в особых контекстах
        "associativity", "async", "convenience", "didSet", "dynamic",
        "final", "get", "indirect", "infix", "lazy", "left", "mutating",
        "none", "nonmutating", "optional", "override", "package",
        "postfix", "precedence", "prefix", "Protocol", "required",
        "right", "set", "some", "Type", "unowned", "weak", "willSet"
    };
    return kw;//возвращаем готовое множество наверх
}