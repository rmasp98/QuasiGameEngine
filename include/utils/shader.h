#ifndef QGE_SHADER_H
#define QGE_SHADER_H

#include <GL/glew.h>
#include <string>
#include <vector>

namespace quasi_game_engine {

   uint LoadShaders(std::vector<std::string> file_paths);

} // namespace quasi_game_engine

#endif // QGE_SHADER_H
