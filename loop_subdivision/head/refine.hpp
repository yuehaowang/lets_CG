#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <cmath>
#include "Eigen/Dense"
#include <algorithm>

#define PI 3.14159265358979323846f

#define ODD_INTERIOR_WEIGHTS (Eigen::Vector2f(3.0f / 8.0f, 1.0f / 8.0f))
#define ODD_BOUNDARY_WEIGHTS (Eigen::Vector2f(1.0f / 2.0f, 1.0f / 2.0f))
#define EVEN_BOUNDARY_WEIGHTS (Eigen::Vector3f(1.0f / 8.0f, 1.0f / 8.0f, 3.0f / 4.0f))
#define EVEN_INTERIOR_WEIGHTS(n) ((1.0f/(n)) * (5.0f/8.0f - powf(3.0f/8.0f + 1.0f/4.0f * cosf(2.0f * PI / (n)), 2)))


//////////////////////////////////////////////////////////////////////////////////////////////
// You should try to reduce the time complexity. For each time of loop subdivision (divide  //
// once), the time complexity of your program should not be lower than O(p^2+f*l) (p:number //
// of points, f: number of faces, l: number of lines). And try to explain how you reduce the//
// time complexity in you report and video.                                                 //
//                                                                                          //
// The following is a reference for you to do loop subdivision.                             //
//////////////////////////////////////////////////////////////////////////////////////////////
namespace ref {
	struct point {
		Eigen::Vector3f pos;
        Eigen::Vector2f uv;	      // you can ignore uv
        Eigen::Vector3f normal;	
        std::vector<int> n_point; // nearby vertices
        std::vector<int> b_point; // boundary vertices
        bool is_border;
        bool is_init;
        point()
        {
            is_border = false;
            is_init = false;
        }
        ~point() {}
	};

	struct face {
        int v[3];
        std::vector<std::string> n_line;
        face()
        {
            v[0] = v[1] = v[2] = -1;
        }
        face(int p1, int p2, int p3)
        {
            v[0] = p1;
            v[1] = p2;
            v[2] = p3;
        }
        ~face() {}
	};

    struct edge {
        int v1;
        int v2;
        int mid_v;
        std::vector<int> n_face;
        bool is_border;
        edge() : v1(-1), v2(-1), mid_v(-1), is_border(false) {}
        edge(int p1, int p2)
        {
            v1 = p1;
            v2 = p2;
            mid_v = -1;
            is_border = false;
        }
        std::string key()
        {
            int first = v1, second = v2;
            if (v1 > v2) {
                first = v2;
                second = v1;
            }
            return std::to_string(first) + "," + std::to_string(second);
        }
        std::pair<int, int> diagonal(const std::vector<face>& F_list) const
        {
            if (is_border || n_face.size() != 2) {
                return std::pair<int, int>(-1, -1);
            }

            const face& f1 = F_list[n_face[0]];
            const face& f2 = F_list[n_face[1]];
            int match_vec[3] = {-1, -1, -1};
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (f1.v[i] == f2.v[j]) {
                        match_vec[j] = i;
                        break;
                    }
                } 
            }

            std::pair<int, int> res;
            int l = 3, h = 0;
            for (int k = 0; k < 3; k++) {
                if (match_vec[k] == -1) {
                    h = k;
                } else {
                    l -= match_vec[k];
                }
            }
            res.first = f1.v[l];
            res.second = f2.v[h];

            return res;
        }
        ~edge() {}
    };
    
    void build_data_struct(
        const std::vector < Eigen::Vector3f >& out_vertices,
        const std::vector < Eigen::Vector3f >& out_normals,
        const std::vector < int >& out_v_index,
        const std::vector < int >& out_vn_index,
        std::vector<ref::point>& P_list,
        std::unordered_map<std::string, edge>& L_list,
        std::vector<ref::face>& F_list);
    

    void loop_subdivision(
        const std::vector<ref::point>& P_list,
        std::unordered_map<std::string, ref::edge> L_list,
        const std::vector<ref::face>& F_list,
        std::vector<ref::point>& P_list_new,
        std::vector<ref::face>& F_list_new);


    // void make_face(
    //     std::vector<ref::point> P_list,
    //     std::vector<ref::face> F_list,
    //     std::vector<Eigen::Vector3f>& points, 
    //     std::vector<Eigen::Vector3f>& normals, 
    //     std::vector<Eigen::Vector2f>& uvs
    // );
}


std::ostream& operator << (std::ostream& os, const ref::point& p)
{
    os << "point(" << p.pos.transpose()  << " | " << p.normal.transpose() << ")"; 
    return os;
}

std::ostream& operator << (std::ostream& os, const ref::edge& ln)
{
    os << "edge(" << ln.v1  << ", " << ln.v2 << ")"; 
    return os;
}

std::ostream& operator << (std::ostream& os, const ref::face& f)
{
    os << "face(" << f.v[0]  << ", "<< f.v[1]  << ", " << f.v[2] << ")"; 
    return os;
}


/* Time complexity: O(f) */
void ref::build_data_struct(
    const std::vector < Eigen::Vector3f >& out_vertices,
    const std::vector < Eigen::Vector3f >& out_normals,
    const std::vector < int >& out_v_index,
    const std::vector < int >& out_vn_index,
    std::vector<ref::point>& P_list,
    std::unordered_map<std::string, ref::edge>& L_list,
    std::vector<ref::face>& F_list)
{

    for (int i = 0; i < out_v_index.size(); i++) {
        int idx = out_v_index[i];
        if (P_list[idx].is_init) {
            continue;
        }
        /* Initialize a vertex if it is uninitialized */
        point* pt = &P_list[idx];
        pt->pos = out_vertices[idx];
        pt->normal = out_normals[out_vn_index[i]];
        pt->is_init = true;
    }

    /* For each face */
    for (int i = 0; i < out_v_index.size() / 3; i++) {
        int pts_idx[3] = {out_v_index[i * 3], out_v_index[i * 3 + 1], out_v_index[i * 3 + 2]};
        point* pts[3] = {&P_list[pts_idx[0]], &P_list[pts_idx[1]], &P_list[pts_idx[2]]};

        face* f = &F_list[i];

        /* For each vertex and edge of the face */
        for (int j = 0; j < 3; j++) {
            /* Current vertex, the next two vertices */
            int idx1 = j, idx2 = (j + 1) % 3, idx3 = (j + 2) % 3;

            /* Add the current vertex to the face */
            f->v[j] = pts_idx[idx1];
            /* The current edge */
            edge ln(pts_idx[idx1], pts_idx[idx2]);
            /* If the edge is not in the edge list, add it to the list. Time complexity: O(1) */
            if (L_list.find(ln.key()) == L_list.end()) {
                L_list[ln.key()] = ln;
                pts[idx1]->n_point.push_back(pts_idx[idx2]);
                pts[idx2]->n_point.push_back(pts_idx[idx1]);
            }
            L_list[ln.key()].n_face.push_back(i);
            f->n_line.push_back(ln.key());
        }
    }
    
    /* Find edges which are located on border */
    for (auto it = L_list.begin(); it != L_list.end(); ++it) {
        if (it->second.n_face.size() == 2) {         /* Shared by two faces: not border */
            it->second.is_border = false;
        } else if (it->second.n_face.size() == 1) {  /* Shared by one face: border */
            it->second.is_border = true;
            P_list[it->second.v1].is_border = true;
            P_list[it->second.v1].b_point.push_back(it->second.v2);
            P_list[it->second.v2].is_border = true;
            P_list[it->second.v2].b_point.push_back(it->second.v1);
        } else {
            std::cout << "ERROR: Not manifold. An edge is shared by " \
                    << it->second.n_face.size() << " faces" << std::endl;
            break;
        }
    }
}

/* Time complexity: O(f) */
void ref::loop_subdivision(
    const std::vector<ref::point>& P_list,
    std::unordered_map<std::string, ref::edge> L_list,
    const std::vector<ref::face>& F_list,
    std::vector<ref::point>& P_list_new,
    std::vector<ref::face>& F_list_new)
{
    P_list_new = std::vector<point>(P_list.size());

    for (auto it = F_list.begin(); it != F_list.end(); ++it) {
        const face& f = *it;

        /* Vertices of subdivided faces */
        std::vector<int> verts;
        
        /* Handle odd points */
        for (int k = 0; k < f.n_line.size(); ++k) {
            std::string edge_key = f.n_line[k];
            edge& ln = L_list[edge_key];

            verts.push_back(f.v[k]);
            if (ln.mid_v != -1) {
                verts.push_back(ln.mid_v);
                continue;
            }
            ln.mid_v = P_list_new.size();
            verts.push_back(ln.mid_v);

            const point& a = P_list[ln.v1];
            const point& b = P_list[ln.v2];

            point new_point;
            
            if (ln.is_border) {  /* Boundary points */
                Eigen::MatrixXf mat(3, 2);
                mat.col(0) << a.pos;
                mat.col(1) << b.pos;
                new_point.pos = mat * ODD_BOUNDARY_WEIGHTS;

                mat.col(0) << a.normal;
                mat.col(1) << b.normal;
                new_point.normal = mat * ODD_BOUNDARY_WEIGHTS;
            } else {            /* Interior points */
                std::pair<int, int> diag = ln.diagonal(F_list);
                if (diag.first < 0 || diag.second < 0) {
                    std::cout << "ERROR: Wrong diagonal vertices of " << ln << std::endl;
                    break;
                }
                const point& c = P_list[diag.first];
                const point& d = P_list[diag.second];

                Eigen::MatrixXf mat(3, 2);
                mat.col(0) << (a.pos + b.pos);
                mat.col(1) << (c.pos + d.pos);
                new_point.pos = mat * ODD_INTERIOR_WEIGHTS;

                mat.col(0) << (a.normal + b.normal);
                mat.col(1) << (c.normal + d.normal);
                new_point.normal = mat * ODD_INTERIOR_WEIGHTS;
            }
            
            /* Normalize normal of the new point */
            new_point.normal.normalize();
            /* Mark the new point to initialized */
            new_point.is_init = true;

            P_list_new.push_back(new_point);
        }

        /* Handle even points */
        int even_pts[3] = {verts[0], verts[2], verts[4]};
        for (int i = 0; i < 3; i++) {
            int pt_idx = even_pts[i];
            const point& ori_pt = P_list[pt_idx];

            point& new_point = P_list_new[pt_idx];

            if (new_point.is_init) {
                continue;
            }
            
            if (ori_pt.is_border) {   /* Boundary points */
                if (ori_pt.b_point.size() != 2) {
                    std::cout << "ERROR: Invalid boundary even points " << ori_pt.b_point.size() << std::endl;
                    break;
                }

                const point& a = P_list[ori_pt.b_point[0]];
                const point& b = P_list[ori_pt.b_point[1]];

                Eigen::MatrixXf mat(3, 3);
                mat.col(0) << a.pos;
                mat.col(1) << b.pos;
                mat.col(2) << ori_pt.pos;
                new_point.pos = mat * EVEN_BOUNDARY_WEIGHTS;

                mat.col(0) << a.normal;
                mat.col(1) << b.normal;
                mat.col(2) << ori_pt.normal;
                new_point.normal = mat * EVEN_BOUNDARY_WEIGHTS;
            } else {             /* Interior points */
                int n = ori_pt.n_point.size();
                if (n == 0) {
                    std::cout << "ERROR: Isolated even points" << std::endl;
                    break;
                }

                float beta = EVEN_INTERIOR_WEIGHTS(n);
                new_point.pos = (1 - n * beta) * ori_pt.pos;
                new_point.normal = (1 - n * beta) * ori_pt.normal;
                for (int j = 0; j < n; j++) {
                    const point& adj_point = P_list[ori_pt.n_point[j]];
                    new_point.pos += beta * adj_point.pos;
                    new_point.normal += beta * adj_point.normal;
                }
            }

            new_point.is_init = true;
        }

        /* Add new subdivided faces */
        F_list_new.push_back(face(verts[5], verts[0], verts[1]));
        F_list_new.push_back(face(verts[1], verts[2], verts[3]));
        F_list_new.push_back(face(verts[3], verts[4], verts[5]));
        F_list_new.push_back(face(verts[1], verts[3], verts[5]));
    }
}


// void ref::make_face(
//         std::vector<ref::point> P_list,
//         std::vector<ref::face> F_list,
//         std::vector<Eigen::Vector3f>& points, 
//         std::vector<Eigen::Vector3f>& normals, 
//         std::vector<Eigen::Vector2f>& uvs)
// {
// 	return;
// }
