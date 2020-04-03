#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;
layout(location = 3) in vec3 vertex_tangent;


struct Mesh {
    mat4 model;
};
uniform Mesh mesh;

struct Camera {
    mat4 view;
    mat4 projection;
    vec3 position;
};
uniform Camera camera;

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;
flat out mat3 frag_TBN;
flat out vec3 frag_cam_pos;


void main()
{
    vec4 world_pos = mesh.model * vec4(vertex_pos, 1.0);
    mat3 normal_mat = transpose(inverse(mat3(mesh.model)));

    mat3 TBN;
    if (length(vertex_tangent) > 0) {
        vec3 T = vertex_tangent;
        vec3 N = normalize(vertex_normal);
        T = normalize(normal_mat * (T - dot(T, N) * N));
        N = normalize(normal_mat * N);
        vec3 B = cross(T, N);
        TBN = mat3(T, B, N);
    } else {
        TBN = mat3(vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0));
    }

    frag_cam_pos = camera.position;
    frag_TBN = TBN;
    frag_pos  = world_pos.xyz;
    frag_normal = normal_mat * vertex_normal;
    frag_texcoord = vertex_texcoord;
    gl_Position = camera.projection * camera.view * world_pos;
}