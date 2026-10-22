#include "Lexer.h"//нужен лексер
#include "Analyzer.h"//нужен анализатор
#include <fstream>//нужен ifstream для чтения файла
#include <sstream>//нужен ostringstream для чтения файла в строку
#include <iostream>//нужны cout и cerr
#include <cstring>//нужен strcmp для разбора аргументов
#include <stdexcept>//нужен runtime_error

#ifdef _WIN32//если Windows
#include <windows.h>//нужен SetConsoleOutputCP для UTF-8
#endif

static std::string readFile(const std::string& path) {//прочитать файл целиком в строку
    std::ifstream in(path, std::ios::binary);//открываем в бинарном режиме
    if (!in) throw std::runtime_error("Не удалось открыть файл: " + path);//если не открылся, ошибка
    std::ostringstream ss;//создаём строковый поток
    ss << in.rdbuf();//перекладываем содержимое файла
    return ss.str();//возвращаем строку
}

int main(int argc, char** argv) {//точка входа
#ifdef _WIN32//если Windows
    SetConsoleOutputCP(CP_UTF8);//переключаем вывод консоли в UTF-8
    SetConsoleCP(CP_UTF8);//переключаем ввод консоли в UTF-8
#endif

    bool dumpMode = false;//флаг режима дампа
    int fileStart = 1;//с какого аргумента идут файлы
    if (argc >= 2 && (std::strcmp(argv[1], "--dump") == 0 || std::strcmp(argv[1], "-d") == 0)) {//если первый аргумент --dump или -d
        dumpMode = true;//включаем дамп
        fileStart = 2;//файлы начинаются со второго аргумента
    }

    if (argc <= fileStart) {//если файлов нет
        std::cerr << "Использование: " << argv[0] << " [--dump|-d] <файл1.swift> [файл2.swift ...]\n";//подсказка
        std::cerr << "  --dump, -d  — вывести полный список всех лексем с типами и позициями\n";//описание
        return 1;//ошибка
    }

    Analyzer analyzer;//создаём анализатор
    int errorCount = 0;//счётчик ошибок

    for (int i = fileStart; i < argc; ++i) {//проходим по всем файлам
        std::string filename = argv[i];//берём имя
        try {
            std::string source = readFile(filename);//читаем файл
            Lexer lexer(source, filename);//создаём лексер
            Token t;//текущий токен
            while (lexer.nextToken(t)) {//пока есть токены
                analyzer.add(t);//добавляем в анализатор
                if (t.type == TokenType::UNKNOWN) {//если неизвестный символ
                    std::cerr << "Ошибка: неизвестный символ '" << t.value << "' в " << t.file << ":" << t.line << ":" << t.column << "\n";//печатаем
                    errorCount++;//увеличиваем счётчик
                }
            }
        } catch (const std::exception& e) {//если фатальная ошибка
            std::cerr << "Фатальная ошибка: " << e.what() << "\n";//печатаем
            errorCount++;//увеличиваем счётчик
        }
    }

    if (dumpMode) analyzer.dump(std::cout);//если режим дампа, печатаем его
    analyzer.report(std::cout);//печатаем обычный отчёт

    if (errorCount > 0) {//если были ошибки
        std::cout << "\nОбнаружено ошибок: " << errorCount << "\n";//печатаем итог
        return 2;//возвращаем код 2
    }
    return 0;//успех
}