#pragma once
#include <vector>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <cmath>
#include "objloader.hpp"
#include "refine.hpp"


class TriangleMesh
{
public:
	// number of triangles
	int triangleCount;
	// verteces, normals and indices data
	std::vector<Eigen::Vector3f> out_vertices;
	std::vector<Eigen::Vector3f> out_normals;
	std::vector<int> out_v_index;
	std::vector<int> out_vn_index;


	explicit TriangleMesh(std::string filePos)
	{
		std::vector <Eigen::Vector2f> out_uvs;
		std::vector <int> out_vt_index;
		loadOBJ_index(filePos.c_str(), out_vertices, out_uvs, out_normals, out_v_index, out_vt_index, out_vn_index);
		
		triangleCount = out_v_index.size() / 3;
	}

	void subdivision(int division_time = 3)
	{
		std::vector<ref::point> P_list(out_vertices.size());
		std::unordered_map<std::string, ref::edge> L_list;
		std::vector<ref::face> F_list(int(out_v_index.size() / 3));
		ref::build_data_struct(
			out_vertices, out_normals,
			out_v_index, out_vn_index,
			P_list, L_list, F_list);

		std::vector<ref::point> P_list_new;
		std::vector<ref::face> F_list_new;

		ref::loop_subdivision(
			P_list, L_list, F_list,
			P_list_new, F_list_new);
		
		out_vertices.clear();
		out_normals.clear();
		out_v_index.clear();
		out_vn_index.clear();

		for (auto it = P_list_new.begin(); it != P_list_new.end(); ++it) {
			out_vertices.push_back(it->pos);
			out_normals.push_back(it->normal);
		}

		for (auto it = F_list_new.begin(); it != F_list_new.end(); ++it) {
			out_v_index.push_back(it->v[0]);
			out_v_index.push_back(it->v[1]);
			out_v_index.push_back(it->v[2]);

			out_vn_index.push_back(it->v[0]);
			out_vn_index.push_back(it->v[1]);
			out_vn_index.push_back(it->v[2]);
		}

		// Finally, update the new size
		triangleCount = out_v_index.size() / 3;

		if (--division_time != 0) {
			subdivision(division_time);
		}
	}

	void save(std::string path) {
		std::ofstream out_file;
		out_file.open(path);

		out_file << "# vertices" << std::endl;
		for (int i = 0; i < out_vertices.size(); i++) {
			Eigen::Vector3f p = out_vertices[i];

			out_file << "v " << p.x() << " " << p.y() << " " << p.z() << std::endl;
		}

		out_file << "# normals" << std::endl;
		for (int i = 0; i < out_normals.size(); i++) {
			Eigen::Vector3f n= out_normals[i];

			out_file << "vn " << n.x() << " " << n.y() << " " << n.z() << std::endl;
		}

		out_file << "# faces" << std::endl;
		for (int i = 0; i < out_v_index.size(); i += 3) {
			out_file << "f " << out_v_index[i] + 1 << "//" << out_vn_index[i] + 1 << " " \
					 << out_v_index[i + 1] + 1 << "//" << out_vn_index[i + 1] + 1 << " " \
					 << out_v_index[i + 2] + 1 << "//" << out_vn_index[i + 2] + 1 << std::endl;
		}

		out_file.close();
	}

};
