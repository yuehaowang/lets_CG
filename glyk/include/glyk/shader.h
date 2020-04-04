#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <map>
#include "glyk/gl_header_files.h"


class Mesh;

class Shader
{

protected:
 
    GLuint program_id;
    std::string vert_shader_file;
    std::string frag_shader_file;

    GLuint CompileShader(const std::string& src, GLenum shader_type);
    void Employ();
    void Dismiss();

    struct ShaderKey
    {
        std::string vert_shader_file;
        std::string frag_shader_file;

        ShaderKey(const std::string& vert, const std::string& frag)
        : vert_shader_file(vert)
        , frag_shader_file(frag)
        {

        }
    };

    struct ShaderProgram
    {
        GLuint program_id;
        unsigned int ref_count;

        ShaderProgram()
        : program_id(0)
        , ref_count(0)
        {
        
        }

        ShaderProgram(GLuint id)
        : program_id(id)
        , ref_count(0)
        {
        
        }
    };

    friend bool operator< (const ShaderKey& k1, const ShaderKey& k2);
    friend class Mesh;

public:

    static std::map<ShaderKey, ShaderProgram> shader_list;

    Shader();
    Shader(const std::string& vert_shader_path, const std::string& frag_shader_path);
    void CreateProgram(const std::string& vert_shader_path, const std::string& frag_shader_path);
    const GLuint Id() const;

};


#endif

