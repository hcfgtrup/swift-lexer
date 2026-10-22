#pragma once//защита от повторного включения заголовочного файла
#include <unordered_set>//хеш-сет, нужен для поиска за O(1), ключевых слов много
#include <string>//строки, нужны для хранения имён ключевых слов

inline const std::unordered_set<std::string>& swiftKeywords() {//функция возвращает ссылку на статическое множество, чтобы не копировать его каждый раз
    static const std::unordered_set<std::string> kw = {//сам список ключевых слов Swift, взят из официальной спецификации
        //ключевые слова объявлений (keywords used in declarations)
        "associatedtype", "borrowing", "class", "consuming", "deinit",
        "enum", "extension", "fileprivate", "func", "import", "init",
        "inout", "internal", "let", "nonisolated", "open", "operator",
        "precedencegroup", "private", "protocol", "public", "rethrows",
        "static", "struct", "subscript", "typealias", "var",

        //ключевые слова инструкций (keywords used in statements)
        "break", "case", "catch", "continue", "default", "defer", "do",
        "else", "fallthrough", "for", "guard", "if", "in", "repeat",
        "return", "switch", "throw", "where", "while",

        //ключевые слова выражений и типов (keywords used in expressions and types)
        "Any", "as", "await", "false", "is", "nil", "self", "Self",
        "super", "throws", "true", "try",

        //ключевые слова в особых контекстах (keywords reserved in particular contexts)
        "associativity", "async", "convenience", "didSet", "dynamic",
        "final", "get", "indirect", "infix", "lazy", "left", "mutating",
        "none", "nonmutating", "optional", "override", "package",
        "postfix", "precedence", "prefix", "Protocol", "required",
        "right", "set", "some", "Type", "unowned", "weak", "willSet",

        //ключевые слова с префиксом #, по спецификации это отдельная категория, лексер должен узнавать их целиком
        "#available", "#colorLiteral", "#else", "#elseif", "#endif",
        "#fileLiteral", "#if", "#imageLiteral", "#keyPath", "#selector",
        "#sourceLocation", "#unavailable"
    };
    return kw;//возвращаем готовое множество
}