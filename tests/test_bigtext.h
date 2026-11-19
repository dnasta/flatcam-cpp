/**
 * @file test_bigtext.h
 * @brief Textos largos de los goldens partidos en trozos (MSVC no acepta literales de más de 16.380 bytes ni
 * concatenaciones de más de 65.535). Los escribe tests/tools/split_long_literals.py.
 */
#ifndef FLATCAM_TESTS_BIGTEXT_H
#define FLATCAM_TESTS_BIGTEXT_H

#include <deque>
#include <initializer_list>
#include <string>

namespace fctest {

/**
 * @brief Une los trozos de un texto largo.
 * @param parts Trozos, en orden.
 * @return El texto entero; el puntero vale hasta el final del programa.
 */
inline const char* bigText(std::initializer_list<const char*> parts) {
    static std::deque<std::string> pool;  // deque: agregar no mueve los textos ya guardados
    std::string s;
    for (const char* p : parts) s += p;
    pool.push_back(std::move(s));
    return pool.back().c_str();
}

}  // namespace fctest

#endif
