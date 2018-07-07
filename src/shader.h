#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>

#include "gl_header_files.h"


class Shader
{

private:

    GLuint program_id;
    GLuint CompileShader(const std::string& src, GLenum shader_type);

public:

    Shader(const std::string& vert_shader_path, const std::string& frag_shader_path);
    const GLuint Id() const;

};


#endif

