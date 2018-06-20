
#include "utils/shader.h"


#include <iostream>
#include <fstream>


namespace quasi_game_engine {

	GLuint LoadShaders(std::vector<std::string> file_paths) {

		GLint result = GL_FALSE;
		GLint info_log_length;
		std::vector<GLuint> ids;
		GLuint program_id = glCreateProgram();

		GLuint vert_index = 0, geometry_index = 0, fragment_index = 0;

		//Loop over each shader
		for (GLuint i_shader=0; i_shader < file_paths.size(); i_shader++) {
			//Determine which tyoe of shader
			GLuint shader_id = 0;
			if ((file_paths[i_shader].compare(file_paths[i_shader].size() - 3, 3, ".vs") == 0) && (!vert_index++))
				shader_id = glCreateShader(GL_VERTEX_SHADER);
			else if ((file_paths[i_shader].compare(file_paths[i_shader].size() - 3, 3, ".gs") == 0) && (!geometry_index++))
				shader_id = glCreateShader(GL_GEOMETRY_SHADER);
			else if ((file_paths[i_shader].compare(file_paths[i_shader].size() - 3, 3, ".fs") == 0) && (!fragment_index++))
				shader_id = glCreateShader(GL_FRAGMENT_SHADER);
			else {
				std::cerr << "Shaders do not have the extension: .vs, .gs, or .fs; or two of the same shader is loaded" << std::endl;
				return 0;
			}

			// Read code from shader
			std::string shader_code;
			std::ifstream shader_stream(file_paths[i_shader].c_str(), std::ios::in);
			if (shader_stream.is_open()) {
				std::string line = "";
				while(getline(shader_stream, line))
					shader_code += "\n" + line;

				shader_stream.close();
			} else {
				std::cerr << "Impossible to open " << file_paths[i_shader] << ". Are you in the right directory? Don't forget to read the FAQ!" << std::endl;
				return 0;
			}


			// Compile shader
			std::cout << "Compiling shader: " << file_paths[i_shader] << std::endl;
			char const * source_pointer = shader_code.c_str();
			glShaderSource(shader_id, 1, &source_pointer , NULL);
			glCompileShader(shader_id);

			// Check shader
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 0) {
				GLchar* shader_error_message = new GLchar[info_log_length+1];
				glGetShaderInfoLog(shader_id, info_log_length, NULL, shader_error_message);
				std::cout << shader_error_message << std::endl;
				delete[] shader_error_message;
			}

			//Attach shader and add to array from later deletion
			glAttachShader(program_id, shader_id);
			ids.push_back(shader_id);
		}

		if (vert_index && fragment_index) {
			// Link the program
			std::cout << "Linking program" << std::endl;
			glLinkProgram(program_id);

			// Error check the linking
			glGetProgramiv(program_id, GL_LINK_STATUS, &result);
			glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
			if (info_log_length > 0) {
				GLchar* program_error_message = new GLchar[info_log_length+1];
				glGetProgramInfoLog(program_id, info_log_length, NULL, program_error_message);
				std::cout << program_error_message << std::endl;
			}

			// Delete the shaders
			for (GLuint i_shader=0; i_shader < ids.size(); i_shader++)
				glDeleteShader(ids[i_shader]);

			return program_id;
		} else {
			std::cerr << "You have not loaded at least a vertex and fragment shader" << std::endl;
			return 0;
		}
	}

} // namespace quasi_game_engine
