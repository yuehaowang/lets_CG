#include <vector>

#include "shader.h"
#include "loader.h"


Shader::Shader(const std::string& vert_shader_path, const std::string& frag_shader_path)
{
    GLuint vert_shader_id = CompileShader(Loader::LoadPlainText(vert_shader_path), GL_VERTEX_SHADER);
    GLuint frag_shader_id = CompileShader(Loader::LoadPlainText(frag_shader_path), GL_FRAGMENT_SHADER);

    program_id = glCreateProgram();
    glAttachShader(program_id, vert_shader_id);
    glAttachShader(program_id, frag_shader_id);
    glLinkProgram(program_id);

}

GLuint Shader::CompileShader(const std::string& src, GLenum shader_type)
{
    printf("Compiling %s...\n", (shader_type == GL_FRAGMENT_SHADER) ? "fragment shader" : "vertex shader");

    int info_log_length = 0;
    GLint compilation_res = GL_FALSE;

    GLuint shader_id = glCreateShader(shader_type);

    char const* src_c_str = src.c_str();

    glShaderSource(shader_id, 1, &src_c_str, NULL);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilation_res);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0){
        std::vector<char> error_msgs(info_log_length + 1);
        glGetShaderInfoLog(shader_id, info_log_length, NULL, &error_msgs[0]);
        
        printf("%s\n", &error_msgs[0]);
    }

    return shader_id;
}

const GLuint Shader::Id() const
{
    return program_id;
}