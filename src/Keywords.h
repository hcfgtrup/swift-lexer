#pragma once //защита от повторного включения заголовочного файла
#include <unordered_set> //подключаем структуру данных "хеш-сет" для быстрого поиска
#include <string> //подключаем работу со строками

//функция, возвращающая ссылку на статическое множество (unordered_set) ключевых слов языка Swift
inline const std::unordered_set<std::string>& swiftKeywords() {
    //объявляем и инициализируем константное множество строк, содержащее все ключевые слова Swift
    static const std::unordered_set<std::string> kw = {
        "associatedtype","class","deinit","enum","extension","fileprivate",
        "func","import","init","inout","internal","let","open","operator",
        "private","protocol","public","static","struct","subscript",
        "typealias","var","break","case","continue","default","defer","do",
        "else","fallthrough","for","guard","if","in","repeat","return",
        "switch","where","while","as","catch","false","is","nil",
        "rethrows","super","self","throw","throws","true","try",
        "Any","Self"
    };
    return kw; //возвращаем готовое множество наверх
}