#ifndef QGE_SHADER_HPP
#define QGE_SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <vector>

namespace qge {

   GLuint LoadShaders(std::vector<std::string> filePaths);

} // namespae qge

#endif // QGE_SHADER_HPP
