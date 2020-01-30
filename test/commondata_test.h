#pragma once

#include <string>

const std::string c_exampleCities = R"(0 A B D
                                       *0 B A D
                                       0 C A
                                       1 D A B)";
/* City network described above:
 * A---B
 * | \ |
 * |  \|
 * C   D
 *
 * A, B, C disease type 0,
 * D disease type 1
 * B is the start city
 */
