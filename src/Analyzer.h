#pragma once//защита от повторного включения
#include "Token.h"//нужен тип Token и TokenType
#include <map>//std::map, нужен для сортировки идентификаторов и подсчётов
#include <vector>//std::vector, нужен для списков вхождений
#include <string>//std::string, нужен для имён
#include <iostream>//std::ostream, нужен для вывода отчёта
#include <algorithm>//std::sort, нужен для сортировки по частоте

struct IdentifierInfo {//информация об одном вхождении идентификатора
    std::string file;//файл, где встретился
    int line;//строка, где встретился
};

class Analyzer {//класс, который собирает статистику и печатает отчёт
public:
    long long total = 0;//общее число лексем
    std::map<TokenType, long long> counts;//счётчик по типам
    std::map<std::string, std::vector<IdentifierInfo>> identifiers;//идентификаторы: имя -> список вхождений
    std::map<std::string, long long> operatorCounts;//операторы: значение -> количество
    std::map<std::string, long long> delimiterCounts;//разделители: значение -> количество
    std::map<std::string, long long> keywordCounts;//ключевые слова: значение -> количество
    std::map<std::string, long long> numberCounts;//числа: значение -> количество
    std::map<std::string, long long> stringCounts;//строки: значение -> количество
    std::vector<Token> allTokens;//все токены по порядку, нужны для дампа

    void add(const Token& t) {//добавить токен в статистику
        total++;//увеличиваем общий счётчик
        counts[t.type]++;//увеличиваем счётчик типа
        allTokens.push_back(t);//сохраняем токен для дампа
        if (t.type == TokenType::IDENTIFIER) {//если идентификатор
            identifiers[t.value].push_back({t.file, t.line});//запоминаем его вхождение
        } else if (t.type == TokenType::OPERATOR) {//если оператор
            operatorCounts[t.value]++;//считаем конкретное значение
        } else if (t.type == TokenType::DELIMITER) {//если разделитель
            delimiterCounts[t.value]++;//считаем конкретное значение
        } else if (t.type == TokenType::KEYWORD) {//если ключевое слово
            keywordCounts[t.value]++;//считаем конкретное значение
        } else if (t.type == TokenType::NUMBER) {//если число
            numberCounts[t.value]++;//считаем конкретное значение
        } else if (t.type == TokenType::STRING) {//если строка
            stringCounts[t.value]++;//считаем конкретное значение
        }
    }

    static std::string typeName(TokenType t) {//перевод типа в строку для дампа
        switch (t) {
            case TokenType::KEYWORD:    return "KEYWORD";//ключевое слово
            case TokenType::IDENTIFIER: return "IDENTIFIER";//идентификатор
            case TokenType::NUMBER:     return "NUMBER";//число
            case TokenType::STRING:     return "STRING";//строка
            case TokenType::OPERATOR:   return "OPERATOR";//оператор
            case TokenType::DELIMITER:  return "DELIMITER";//разделитель
            case TokenType::UNKNOWN:    return "UNKNOWN";//ошибка
        }
        return "?";//на всякий случай
    }

    static std::string typeNameRu(TokenType t) {//перевод типа в русскую строку для отчёта
        switch (t) {
            case TokenType::KEYWORD:    return "Ключевые слова";//ключевое слово
            case TokenType::IDENTIFIER: return "Идентификаторы";//идентификатор
            case TokenType::NUMBER:     return "Числа";//число
            case TokenType::STRING:     return "Строки";//строка
            case TokenType::OPERATOR:   return "Операторы";//оператор
            case TokenType::DELIMITER:  return "Разделители";//разделитель
            case TokenType::UNKNOWN:    return "Ошибки";//ошибка
        }
        return "?";//на всякий случай
    }

    //печатает словарь "значение -> количество" в виде списка через запятую
    static void printValuesList(std::ostream& os, const std::map<std::string, long long>& counts) {
        if (counts.empty()) {//если словарь пустой
            os << "нет\n";//сообщаем, что нет
            return;
        }
        //копируем в вектор, чтобы отсортировать по частоте
        std::vector<std::pair<std::string, long long>> items(counts.begin(), counts.end());
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {//сортируем
            if (a.second != b.second) return a.second > b.second;//сначала по убыванию частоты
            return a.first < b.first;//при равенстве — по алфавиту
        });
        os << "всего уникальных значений: " << items.size() << "\n";//сколько разных
        bool first = true;//флаг для запятых
        for (const auto& [value, cnt] : items) {//проходим по списку
            if (!first) os << ", ";//разделитель между значениями
            first = false;
            os << value << " (" << cnt << ")";//печатаем значение и частоту (без лишних кавычек — значение уже включает свои)
        }
        os << "\n";//перевод строки в конце
    }

    void dump(std::ostream& os) const {//полный дамп всех токенов
        os << "дамп лексем\n";//заголовок
        os << "формат: строка:колонка  тип         значение\n";//подсказка по формату
        for (const auto& t : allTokens) {//проходим по всем токенам
            std::string typeStr = typeName(t.type);//берём имя типа
            while (typeStr.size() < 12) typeStr += ' ';//выравниваем по ширине
            os << t.line << ":" << t.column << "\t" << typeStr << t.value << "\n";//печатаем строку
        }
        os << "\n";
    }

    void report(std::ostream& os) const {//обычный отчёт
        os << "общая статистика\n";//заголовок
        os << "всего лексем: " << total << "\n\n";//общее число

        os << "распределение по типам\n";//заголовок
        std::vector<TokenType> order = {//фиксированный порядок вывода
            TokenType::KEYWORD, TokenType::IDENTIFIER, TokenType::NUMBER,
            TokenType::STRING, TokenType::OPERATOR, TokenType::DELIMITER,
            TokenType::UNKNOWN
        };
        for (auto t : order) {//проходим по типам
            auto it = counts.find(t);//ищем в счётчике
            long long c = (it == counts.end()) ? 0 : it->second;//если нет, то 0
            double pct = total ? (100.0 * c / total) : 0.0;//считаем процент
            os << typeNameRu(t) << ": " << c << " (" << pct << "%)\n";//печатаем русское имя, число, процент
        }
        os << "\n";

        os << "ключевые слова\n";//заголовок
        printValuesList(os, keywordCounts);//печатаем список через запятую
        os << "\n";

        os << "операторы\n";//заголовок
        printValuesList(os, operatorCounts);//печатаем список через запятую
        os << "\n";

        os << "разделители\n";//заголовок
        printValuesList(os, delimiterCounts);//печатаем список через запятую
        os << "\n";

        os << "числа\n";//заголовок
        printValuesList(os, numberCounts);//печатаем список через запятую
        os << "\n";

        os << "строки\n";//заголовок
        printValuesList(os, stringCounts);//печатаем список через запятую
        os << "\n";

        os << "идентификаторы (лексикографический порядок)\n";//заголовок
        if (identifiers.empty()) {//если идентификаторов нет
            os << "нет\n";//сообщаем
        } else {
            for (const auto& [name, occs] : identifiers) {//std::map уже отсортировал по алфавиту
                os << name << " — " << occs.size() << " вхожд.\n";//имя и число вхождений
                for (const auto& o : occs) {//по каждому вхождению
                    os << "    " << o.file << ":" << o.line << "\n";//файл и строка с отступом
                }
            }
        }
    }
};