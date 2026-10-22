#pragma once//защита от повторного включения заголовочного файла
#include "Token.h"//подключаем структуру Token, лексер её заполняет
#include "Keywords.h"//подключаем список ключевых слов, чтобы отличать их от идентификаторов
#include <string>//строки, нужны для хранения исходника и результата
#include <cctype>//isdigit/isalpha/isspace/isxdigit, нужны для классификации символов
#include <stdexcept>//runtime_error, нужен для фатальных ошибок
#include <vector>//vector, нужен для списка многосимвольных операторов

class Lexer {//класс, который читает текст посимвольно и выдаёт токены
public:
    Lexer(const std::string& source, const std::string& filename)//конструктор получает весь текст файла и его имя
        : src(source), file(filename), pos(0), line(1), col(1) {}//запоминаем текст, имя, начинаем с позиции 0, строки 1, колонки 1

    bool nextToken(Token& out) {//главный метод, возвращает true, если токен есть, и false, если файл кончился
        skipWhitespaceAndComments();//пропускаем пробелы и комментарии, они не токены
        if (pos >= src.size()) return false;//если дошли до конца файла, сообщаем что токенов больше нет

        out.file = file;//запоминаем в токене имя файла
        out.line = line;//запоминаем номер строки
        out.column = col;//запоминаем номер колонки

        char c = src[pos];//берём текущий символ

        //важно: проверка extended-строки должна идти ДО обработки #, иначе #"..."# разобьётся на # + строка + #
        if (c == '#' && peek(1) == '"') {//если это начало extended-строки #"..."#
            out.type = TokenType::STRING;//это строка
            out.value = readString();//читаем строку целиком, readString сам разберётся с #
            return true;//токен готов
        }

        if (c == '#') {//если символ #, это либо #-ключевое слово, либо оператор #
            std::string hashtag = readHashKeyword();//пробуем прочитать "#" + слово
            if (!hashtag.empty() && swiftKeywords().count(hashtag)) {//если получилось и слово есть в списке ключевых
                out.type = TokenType::KEYWORD;//это ключевое слово
                out.value = hashtag;//например "#available"
                return true;//токен готов
            }
            out.type = TokenType::OPERATOR;//иначе это просто оператор #
            out.value = "#";//значение — решётка
            advance();//сдвигаемся на один символ
            return true;//токен готов
        }

        if (c == '$') {//если символ $, это $-идентификатор ($0, $1, $published)
            std::string dollarId = readDollarIdentifier();//читаем $ + цифры или буквы
            if (dollarId.size() > 1) {//если после $ что-то есть
                out.type = TokenType::IDENTIFIER;//это идентификатор
                out.value = dollarId;//например "$0" или "$published"
                return true;//токен готов
            }
            out.type = TokenType::UNKNOWN;//если $ без продолжения, это ошибка
            out.value = "$";//значение — доллар
            return true;//токен готов
        }

        if (c == '`') {//если символ `, это raw-идентификатор в backticks (`class`)
            out.type = TokenType::IDENTIFIER;//это идентификатор
            out.value = readRawIdentifier();//читаем содержимое между backticks
            return true;//токен готов
        }

        if (std::isdigit(static_cast<unsigned char>(c))) {//если символ цифра
            out.type = TokenType::NUMBER;//это число
            out.value = readNumber();//читаем число целиком
            return true;//токен готов
        }

        if (c == '"') {//если открывающая кавычка, это обычная или многострочная строка
            out.type = TokenType::STRING;//это строка
            out.value = readString();//читаем строку целиком
            return true;//токен готов
        }

        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {//если буква или подчёркивание
            std::string word = readIdentifier();//читаем слово целиком
            if (swiftKeywords().count(word)) {//если слово есть в списке ключевых
                out.type = TokenType::KEYWORD;//это ключевое слово
            } else {
                out.type = TokenType::IDENTIFIER;//иначе это идентификатор
            }
            out.value = word;//сохраняем текст
            return true;//токен готов
        }

        std::string op = readOperatorOrDelimiter();//пробуем прочитать оператор или разделитель
        if (!op.empty()) {//если что-то прочитали
            if (op.size() == 1 && std::string("(){}[];,:").find(op[0]) != std::string::npos) {//если это одиночный символ из списка разделителей
                out.type = TokenType::DELIMITER;//это разделитель
            } else {
                out.type = TokenType::OPERATOR;//иначе это оператор
            }
            out.value = op;//сохраняем текст
            return true;//токен готов
        }

        out.type = TokenType::UNKNOWN;//если ничего не подошло, это неизвестный символ
        out.value = std::string(1, c);//значение — сам символ
        advance();//сдвигаемся, чтобы не зациклиться
        return true;//токен готов
    }

private:
    std::string src;//исходный текст файла
    std::string file;//имя файла
    size_t pos;//текущая позиция в тексте
    int line;//текущая строка
    int col;//текущая колонка

    char peek(size_t offset = 0) const {//посмотреть символ впереди без сдвига
        if (pos + offset >= src.size()) return '\0';//если за концом файла, вернуть нулевой символ
        return src[pos + offset];//вернуть символ по смещению
    }

    void advance() {//сдвинуть позицию на один символ
        if (pos < src.size()) {//если не конец файла
            if (src[pos] == '\n') { line++; col = 1; }//если перенос строки, увеличиваем строку и сбрасываем колонку
            else { col++; }//иначе увеличиваем колонку
            pos++;//увеличиваем позицию
        }
    }

    void skipWhitespaceAndComments() {//пропустить пробелы и комментарии
        while (pos < src.size()) {//пока не конец файла
            char c = src[pos];//берём текущий символ
            if (std::isspace(static_cast<unsigned char>(c))) {//если пробел/таб/перенос
                advance();//пропускаем
            } else if (c == '/' && peek(1) == '/') {//если однострочный комментарий //
                while (pos < src.size() && src[pos] != '\n') advance();//пропускаем до конца строки
            } else if (c == '/' && peek(1) == '*') {//если многострочный комментарий /*
                int depth = 1;//счётчик вложенности
                advance(); advance();//пропускаем /*
                while (pos < src.size() && depth > 0) {//пока не закроются все комментарии
                    if (src[pos] == '/' && peek(1) == '*') {//если вложенный /*
                        depth++; advance(); advance();//увеличиваем глубину
                    } else if (src[pos] == '*' && peek(1) == '/') {//если закрывающий */
                        depth--; advance(); advance();//уменьшаем глубину
                    } else {
                        advance();//иначе просто идём дальше
                    }
                }
                if (depth > 0) {//если дошли до конца, а комментарий не закрыт
                    throw std::runtime_error("Незакрытый многострочный комментарий в " + file + " строка " + std::to_string(line));//это фатальная ошибка
                }
            } else {
                break;//если это не пробел и не комментарий, выходим из цикла
            }
        }
    }

    std::string readHashKeyword() {//прочитать #-ключевое слово или вернуть пустую строку, если это не оно
        size_t savedPos = pos;//запоминаем позицию для отката
        int savedLine = line;//запоминаем строку
        int savedCol = col;//запоминаем колонку
        std::string result;//строка результата
        result += '#';//берём решётку
        advance();//сдвигаемся
        if (!std::isalpha(static_cast<unsigned char>(peek()))) {//если после # не буква
            pos = savedPos; line = savedLine; col = savedCol;//откатываемся
            return "";//это не #-ключевое слово
        }
        while (pos < src.size()) {//читаем буквы и цифры
            char c = peek();
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
                result += c; advance();
            } else break;//если не буква и не цифра, стоп
        }
        return result;//например "#available"
    }

    std::string readDollarIdentifier() {//прочитать $-идентификатор ($0, $1, $published)
        std::string result = "$";//начинаем с доллара
        advance();//сдвигаемся
        while (pos < src.size()) {//читаем цифры и буквы
            char c = peek();
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
                result += c; advance();
            } else break;//если не подходит, стоп
        }
        return result;//например "$0"
    }

    std::string readRawIdentifier() {//прочитать raw-идентификатор в backticks
        std::string result;//строка результата
        advance();//пропускаем открывающий `
        while (pos < src.size()) {//читаем до закрывающего `
            char c = peek();
            if (c == '`') {//если закрывающий backtick
                advance();//пропускаем
                return result;//возвращаем содержимое без backticks
            } else if (c == '\n') {//если перенос строки, ошибка
                throw std::runtime_error("Незакрытый raw-идентификатор в " + file + " строка " + std::to_string(line));
            } else {
                result += c; advance();//добавляем символ
            }
        }
        throw std::runtime_error("Незакрытый raw-идентификатор в " + file + " строка " + std::to_string(line));//дошли до конца, не закрыли
    }

    std::string readNumber() {//прочитать число во всех формах
        std::string result;//строка результата
        if (peek() == '0' && (peek(1) == 'x' || peek(1) == 'X' || peek(1) == 'o' || peek(1) == 'O' || peek(1) == 'b' || peek(1) == 'B')) {//если префикс 0x/0o/0b
            char base = peek(1);//запоминаем систему счисления
            result += peek(); advance();//добавляем 0
            result += peek(); advance();//добавляем букву системы
            //читаем целую часть числа в соответствующей системе счисления
            while (pos < src.size()) {
                char c = peek();
                //для hex запрещаем читать p/P как часть целой части — это начало экспоненты
                //без этого 0xFp-2 разобьётся на 0xFp + - + 2 вместо одного числа
                if ((base == 'x' || base == 'X') && (c == 'p' || c == 'P')) break;
                if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
                    result += c; advance();
                } else break;
            }
            if ((base == 'x' || base == 'X') && peek() == '.') {//если hex-float с дробной частью
                if (std::isxdigit(static_cast<unsigned char>(peek(1)))) {//если после точки есть hex-цифра
                    result += peek(); advance();//добавляем точку
                    while (pos < src.size() && (std::isxdigit(static_cast<unsigned char>(peek())) || peek() == '_')) {
                        result += peek(); advance();//читаем hex-цифры после точки
                    }
                }
            }
            if ((base == 'x' || base == 'X') && (peek() == 'p' || peek() == 'P')) {//если hex-float с экспонентой p
                char afterP = peek(1);//смотрим, что идёт после p
                bool validExp = false;//флаг, что экспонента валидна
                if (std::isdigit(static_cast<unsigned char>(afterP))) {//p + цифра
                    validExp = true;
                } else if ((afterP == '+' || afterP == '-') && std::isdigit(static_cast<unsigned char>(peek(2)))) {//p + знак + цифра
                    validExp = true;
                }
                if (validExp) {
                    result += peek(); advance();//добавляем p или P
                    if (peek() == '+' || peek() == '-') { result += peek(); advance(); }//добавляем знак
                    while (pos < src.size() && std::isdigit(static_cast<unsigned char>(peek()))) {
                        result += peek(); advance();//читаем десятичные цифры экспоненты
                    }
                }
            }
            return result;//возвращаем число
        }
        while (pos < src.size()) {//читаем обычное число
            char c = peek();
            if (std::isdigit(static_cast<unsigned char>(c)) || c == '_') {
                result += c; advance();//добавляем цифры
            } else if (c == '.' && std::isdigit(static_cast<unsigned char>(peek(1)))) {//если дробная часть
                result += c; advance();//добавляем точку
                while (pos < src.size() && (std::isdigit(static_cast<unsigned char>(peek())) || peek() == '_')) {
                    result += peek(); advance();//читаем цифры после точки
                }
            } else if ((c == 'e' || c == 'E') && (std::isdigit(static_cast<unsigned char>(peek(1))) || ((peek(1) == '+' || peek(1) == '-') && std::isdigit(static_cast<unsigned char>(peek(2)))))) {//если экспонента e
                result += c; advance();//добавляем e
                if (peek() == '+' || peek() == '-') { result += peek(); advance(); }//добавляем знак
                while (pos < src.size() && std::isdigit(static_cast<unsigned char>(peek()))) {
                    result += peek(); advance();//читаем цифры экспоненты
                }
            } else break;//если не подходит, стоп
        }
        return result;//возвращаем число
    }

    std::string readString() {//прочитать строку: обычную, многострочную или extended #"..."#
        if (peek() == '#' && peek(1) == '"') {//если это extended-строка #"..."#
            int hashCount = 0;//считаем количество # перед кавычкой
            while (peek() == '#') { hashCount++; advance(); }//считаем решётки
            advance();//пропускаем открывающую кавычку
            std::string closing = "\"" + std::string(hashCount, '#');//формируем закрывающую последовательность
            std::string result = std::string(hashCount, '#') + "\"";//начало результата
            while (pos < src.size()) {//читаем до закрывающей последовательности
                if (src.compare(pos, closing.size(), closing) == 0) {//если встретили закрывашку
                    result += closing;//добавляем её
                    for (size_t i = 0; i < closing.size(); ++i) advance();//сдвигаемся на её длину
                    return result;//возвращаем строку
                }
                result += peek(); advance();//иначе добавляем символ
            }
            throw std::runtime_error("Незакрытая extended-строка в " + file + " строка " + std::to_string(line));//дошли до конца, не закрыли
        }

        if (peek() == '"' && peek(1) == '"' && peek(2) == '"') {//если многострочная строка """..."""
            std::string result = "\"\"\"";//начало
            advance(); advance(); advance();//пропускаем три кавычки
            while (pos < src.size()) {//читаем до трёх кавычек
                if (peek() == '"' && peek(1) == '"' && peek(2) == '"') {//если нашли """
                    result += "\"\"\"";//добавляем
                    advance(); advance(); advance();//сдвигаемся
                    return result;//возвращаем строку
                }
                result += peek(); advance();//иначе добавляем символ
            }
            throw std::runtime_error("Незакрытая многострочная строка в " + file + " строка " + std::to_string(line));//не закрыли
        }

        std::string result;//для обычной строки
        result += '"'; advance();//берём открывающую кавычку
        while (pos < src.size()) {//читаем до закрывающей
            char c = peek();
            if (c == '\\') {//если экранирование
                result += c; advance();//добавляем слэш
                if (pos < src.size()) { result += peek(); advance(); }//и экранируемый символ
            } else if (c == '"') {//если закрывающая кавычка
                result += c; advance();//добавляем
                return result;//возвращаем строку
            } else if (c == '\n') {//если перенос строки, ошибка
                throw std::runtime_error("Незакрытая строка в " + file + " строка " + std::to_string(line));
            } else {
                result += c; advance();//добавляем обычный символ
            }
        }
        throw std::runtime_error("Незакрытая строка в " + file + " строка " + std::to_string(line));//не закрыли
    }

    std::string readIdentifier() {//прочитать слово (идентификатор или ключевое слово)
        std::string result;//строка результата
        while (pos < src.size()) {//пока не конец
            char c = peek();
            if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {//если буква, цифра или _
                result += c; advance();//добавляем
            } else break;//иначе стоп
        }
        return result;//возвращаем слово
    }

    std::string readOperatorOrDelimiter() {//прочитать оператор или разделитель
        static const std::vector<std::string> multiOps = {//список многосимвольных операторов, важен порядок: сначала длинные
            "...", "..<", "===", "!==", "->", "??", "&&", "||",
            "==", "!=", "<=", ">=", "+=", "-=", "*=", "/=", "%=",
            "<<", ">>", "&=", "|=", "^="
        };
        for (const auto& op : multiOps) {//проходим по списку
            if (src.compare(pos, op.size(), op) == 0) {//если текущий текст совпал
                for (size_t i = 0; i < op.size(); ++i) advance();//сдвигаемся на длину оператора
                return op;//возвращаем оператор
            }
        }
        char c = peek();//если многосимвольный не подошёл, проверяем одиночный
        if (std::string("+-*/%=<>!&|^~?:.,;(){}[]@#").find(c) != std::string::npos) {//если символ из списка
            std::string result(1, c);//делаем строку из одного символа
            advance();//сдвигаемся
            return result;//возвращаем
        }
        return "";//не оператор и не разделитель
    }
};