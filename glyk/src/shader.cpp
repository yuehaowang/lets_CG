#include <vector>
#include <iostream>
#include "glyk/shader.h"
#include "glyk/loader.h"


std::map<Shader::ShaderKey, Shader::ShaderProgram> Shader::shader_list = std::map<Shader::ShaderKey, Shader::ShaderProgram>();

bool operator< (const Shader::ShaderKey& k1, const Shader::ShaderKey& k2) {
    return (k1.vert_shader_file < k2.vert_shader_file) || (k1.vert_shader_file == k2.vert_shader_file && k1.frag_shader_file < k2.frag_shader_file);
}

Shader::Shader()
: vert_shader_file("")
, frag_shader_file("")
{

}

Shader::Shader(const std::string& vert_shader_path, const std::string& frag_shader_path)
{
    CreateProgram(vert_shader_path, frag_shader_path);
}

void Shader::Employ()
{
    if (vert_shader_file.empty() || frag_shader_file.empty()) {
        return;
    }

    if (shader_list.find(ShaderKey(vert_shader_file, frag_shader_file)) != shader_list.end()) {
        ShaderProgram& prog = shader_list[ShaderKey(vert_shader_file, frag_shader_file)];
        prog.ref_count++;
    }

}

void Shader::Dismiss()
{
    if (vert_shader_file.empty() || frag_shader_file.empty()) {
        return;
    }

    if (shader_list.find(ShaderKey(vert_shader_file, frag_shader_file)) != shader_list.end()) {
        ShaderProgram& prog = shader_list[ShaderKey(vert_shader_file, frag_shader_file)];
        prog.ref_count--;

        if (prog.ref_count == 0) {
            glDeleteProgram(prog.program_id);
            shader_list.erase(ShaderKey(vert_shader_file, frag_shader_file));
        }
    }
}

void Shader::CreateProgram(const std::string& vert_shader_path, const std::string& frag_shader_path)
{
    vert_shader_file = vert_shader_path;
    frag_shader_file = frag_shader_path;

    std::map<ShaderKey, ShaderProgram>::iterator search = shader_list.find(ShaderKey(vert_shader_path, frag_shader_path));
    if (search != shader_list.end()) {
        program_id = search->second.program_id;
        return;
    }

    printf("-- Compiling vertex shader %s...\n", vert_shader_path.c_str());
    GLuint vert_shader_id = CompileShader(Loader::LoadPlainText(vert_shader_path), GL_VERTEX_SHADER);
    printf("-- Compiling fragment shader %s...\n", frag_shader_path.c_str());
    GLuint frag_shader_id = CompileShader(Loader::LoadPlainText(frag_shader_path), GL_FRAGMENT_SHADER);

    program_id = glCreateProgram();
    glAttachShader(program_id, vert_shader_id);
    glAttachShader(program_id, frag_shader_id);
    glLinkProgram(program_id);

    shader_list.insert(std::pair<ShaderKey, ShaderProgram>(ShaderKey(vert_shader_path, frag_shader_path), ShaderProgram(program_id)));
}

GLuint Shader::CompileShader(const std::string& src, GLenum shader_type)
{
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