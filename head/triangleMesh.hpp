#pragma once
#include <vector>
#include "shape.hpp"
#include "objloader.hpp"

class TriangleMesh : public Shape
{
public:
	// number of triangles
	int triangleCount;
	// verteces, normals and indices data
	std::vector<Eigen::Vector3f> out_vertices;
	std::vector<Eigen::Vector3f> out_normals;
	std::vector<int> out_v_index;
	std::vector<int> out_vn_index;
	// uniform grid data
	bool isUniformExisting;
	std::vector<std::vector<int>> grid;
	Eigen::Vector3i gridDim;
	Eigen::Vector3f gridDeltaDist;
	
	explicit TriangleMesh(const Eigen::Vector3f& color, std::string filePos)
		: Shape(color)
	{
		std::vector <Eigen::Vector2f> out_uvs;
		std::vector <int> out_vt_index;
		loadOBJ_index(filePos.c_str(), out_vertices, out_uvs, out_normals, out_v_index, out_vt_index, out_vn_index);
		
		triangleCount = out_v_index.size() / 3;
		
		m_BoundingBox.lb = out_vertices[0].cwiseMin(out_vertices[1]);
		m_BoundingBox.ub = out_vertices[0].cwiseMax(out_vertices[1]);
		for (int i = 2; i < out_vertices.size(); i++)
		{
			m_BoundingBox.lb = m_BoundingBox.lb.cwiseMin(out_vertices[i]);
			m_BoundingBox.ub = m_BoundingBox.ub.cwiseMax(out_vertices[i]);
		}

		isUniformExisting = false;
	}

	// ray intersection with single triangle, result saves in @Interaction
	bool raySingleTriangleIntersection(Interaction& interaction, const Ray& ray, int v0_idx, int v1_idx, int v2_idx)
	{
		const Eigen::Vector3f& v0 = out_vertices[out_v_index[v0_idx]];
		const Eigen::Vector3f& v1 = out_vertices[out_v_index[v1_idx]];
		const Eigen::Vector3f& v2 = out_vertices[out_v_index[v2_idx]];

		Eigen::Vector3f v0v1 = v1 - v0;
		Eigen::Vector3f v0v2 = v2 - v0;
		Eigen::Vector3f pvec = ray.m_Dir.cross(v0v2);
		float det = v0v1.dot(pvec);

		// ray and triangle are parallel if det is close to 0
		if (det < 1e-10 && det > -1e-10)
			return false;

		float invDet = 1 / det;

		Eigen::Vector3f tvec = ray.m_Ori - v0;
		float u = tvec.dot(pvec) * invDet;
		if (u < 0 || u > 1)
			return false;

		Eigen::Vector3f qvec = tvec.cross(v0v1);
		float v = ray.m_Dir.dot(qvec) * invDet;
		if (v < 0 || u + v > 1)
			return false;
		
		float t = v0v2.dot(qvec) * invDet;
		if (t < ray.m_fMin || t > ray.m_fMax)
			return false;
		
		interaction.uv[0] = u;
		interaction.uv[1] = v;
		interaction.entryDist = t;
		interaction.entryPoint = ray.getPoint(interaction.entryDist);
		interaction.surfaceColor = color;
		interaction.normal = (u * out_normals[out_vn_index[v1_idx]] + v * out_normals[out_vn_index[v2_idx]] + (1 - u - v) * out_normals[out_vn_index[v0_idx]]).normalized();

		return true; // this ray hits the triangle 
	}

	// ray intersection with mesh, result saves in @Interaction
	bool rayIntersection(Interaction& interaction, const Ray& ray) override
	{
		Interaction finalInteraction;
		if (isUniformExisting) {
			Eigen::Vector3f diff = ray.m_Dir;
			for (int i = 0; i < 3; i++) {
				if (diff[i] == 0) {
					diff[i] = 1e-32;
				}
			}
			Eigen::Vector3f diffAbs = diff.cwiseAbs();

			Eigen::Vector3f v1 = ray.getPoint(interaction.entryDist);
			Eigen::Vector3f v2 = ray.getPoint(interaction.exitDist);

			Eigen::Vector3f startPointf = (v1 - m_BoundingBox.lb).cwiseQuotient(gridDeltaDist);
			Eigen::Vector3i startPoint(floor(startPointf[0]), floor(startPointf[1]), floor(startPointf[2]));
			startPoint = startPoint.cwiseMax(Eigen::Vector3i(0, 0, 0)).cwiseMin(gridDim - Eigen::Vector3i(1, 1, 1));
			Eigen::Vector3f endPointf = (v2 - m_BoundingBox.lb).cwiseQuotient(gridDeltaDist);
			Eigen::Vector3i endPoint(floor(endPointf[0]), floor(endPointf[1]), floor(endPointf[2]));
			endPoint = endPoint.cwiseMax(Eigen::Vector3i(0, 0, 0)).cwiseMin(gridDim - Eigen::Vector3i(1, 1, 1));

			Eigen::Vector3f tMax;
			Eigen::Vector3i step;
			for (int i = 0; i < 3; i++) {
				if (diff[i] >= 0) {
					step[i] = 1;
					tMax[i] = ((startPoint[i] + 1) * gridDeltaDist[i] + m_BoundingBox.lb[i]) - v1[i];
				}
				else {
					step[i] = -1;
					tMax[i] = v1[i] - (startPoint[i] * gridDeltaDist[i] + m_BoundingBox.lb[i]);
				}
			}

			tMax = tMax.cwiseQuotient(diffAbs);
			Eigen::Vector3f tDelta = gridDeltaDist.cwiseQuotient(diffAbs);

			for (int triIdx : grid[startPoint[2] * gridDim[1] * gridDim[0] + startPoint[1] * gridDim[0] + startPoint[0]]) {
				Interaction curInteraction;
				if (raySingleTriangleIntersection(curInteraction, ray, 3 * triIdx, 3 * triIdx + 1, 3 * triIdx + 2)) {
					if (finalInteraction.entryDist == -1 || curInteraction.entryDist < finalInteraction.entryDist) {
						finalInteraction = curInteraction;
					}
				}
			}
			if (finalInteraction.entryDist != -1) {
				Eigen::Vector3f curPointf = (ray.getPoint(finalInteraction.entryDist) - m_BoundingBox.lb).cwiseQuotient(gridDeltaDist);
				Eigen::Vector3i curPoint(floor(curPointf[0]), floor(curPointf[1]), floor(curPointf[2]));
				curPoint = curPoint.cwiseMax(Eigen::Vector3i(0, 0, 0)).cwiseMin(gridDim - Eigen::Vector3i(1, 1, 1));
				if (curPoint == startPoint) {
					interaction = finalInteraction;
					return true;
				}
				else {
					finalInteraction = Interaction();
				}
			}
			Eigen::Vector3i tempPoint = startPoint;
			while (tempPoint != endPoint) {
				if (tMax[0] < tMax[1]) {
					if (tMax[0] < tMax[2]) {
						tMax[0] = tMax[0] + tDelta[0];
						tempPoint[0] = tempPoint[0] + step[0];
						if (tempPoint[0] < 0 || tempPoint[0] >= gridDim[0]) {
							break;
						}
					} else {
						tMax[2] = tMax[2] + tDelta[2];
						tempPoint[2] = tempPoint[2] + step[2];
						if (tempPoint[2] < 0 || tempPoint[2] >= gridDim[2]) {
							break;
						}
					}
				} else {
					if (tMax[1] < tMax[2]) {
						tMax[1] = tMax[1] + tDelta[1];
						tempPoint[1] = tempPoint[1] + step[1];
						if (tempPoint[1] < 0 || tempPoint[1] >= gridDim[1]) {
							break;
						}
					} else {
						tMax[2] = tMax[2] + tDelta[2];
						tempPoint[2] = tempPoint[2] + step[2];
						if (tempPoint[2] < 0 || tempPoint[2] >= gridDim[2]) {
							break;
						}
					}
				}

				for (int triIdx : grid[tempPoint[2] * gridDim[1] * gridDim[0] + tempPoint[1] * gridDim[0] + tempPoint[0]]) {
					Interaction curInteraction;
					if (raySingleTriangleIntersection(curInteraction, ray, 3 * triIdx, 3 * triIdx + 1, 3 * triIdx + 2)) {
						if (finalInteraction.entryDist == -1 || curInteraction.entryDist < finalInteraction.entryDist) {
							finalInteraction = curInteraction;
						}
					}
				}
				if (finalInteraction.entryDist != -1) {
					Eigen::Vector3f curPointf = (ray.getPoint(finalInteraction.entryDist) - m_BoundingBox.lb).cwiseQuotient(gridDeltaDist);
					Eigen::Vector3i curPoint(floor(curPointf[0]), floor(curPointf[1]), floor(curPointf[2]));
					curPoint = curPoint.cwiseMax(Eigen::Vector3i(0, 0, 0)).cwiseMin(gridDim - Eigen::Vector3i(1, 1, 1));
					if (curPoint == tempPoint) {
						interaction = finalInteraction;
						return true;
					}
					else {
						finalInteraction = Interaction();
					}
				}
			}
		} else {
			for (int i = 0; i < triangleCount; i++) {
				Interaction curInteraction;
				if (raySingleTriangleIntersection(curInteraction, ray, 3 * i, 3 * i + 1, 3 * i + 2)) {
					if (finalInteraction.entryDist == -1 || curInteraction.entryDist < finalInteraction.entryDist) {
						finalInteraction = curInteraction;
					}
				}
			}
		}

		if (finalInteraction.entryDist != -1)
		{
			interaction = finalInteraction;
			return true;
		}
		return false;
	}

	// apply a certain transformation to all vertices, normals and bounding box
	void applyTransformation(const Eigen::Affine3f& t)
	{
		for (int i = 0; i < out_vertices.size(); i++)
		{
			out_vertices[i] = t * out_vertices[i];
		}

		Eigen::Matrix3f tInvTr = t.linear().inverse().transpose();
		for (int i = 0; i < out_normals.size(); i++)
		{
			out_normals[i] = (tInvTr * out_normals[i]).normalized();
		}

		m_BoundingBox.lb = out_vertices[0].cwiseMin(out_vertices[1]);
		m_BoundingBox.ub = out_vertices[0].cwiseMax(out_vertices[1]);
		for (int i = 2; i < out_vertices.size(); i++)
		{
			m_BoundingBox.lb = m_BoundingBox.lb.cwiseMin(out_vertices[i]);
			m_BoundingBox.ub = m_BoundingBox.ub.cwiseMax(out_vertices[i]);
		}
	}

	void buildUniformGrid() {
		// 1. Calculate grid size
		float dim = powf(4 * triangleCount / std::fmaxf(m_BoundingBox.getVolume(), 0.001f), 1.f / 3);
		for (int i = 0; i < 3; i++) {
			gridDim[i] = (int)fmaxf(dim * m_BoundingBox.getDist(i), 1);
			gridDeltaDist[i] = m_BoundingBox.getDist(i) / gridDim[i];
		}
		std::cout << "triangleCount:" << triangleCount << std::endl;
		std::cout << "m_BoundingBox.lb:" << m_BoundingBox.lb[0] << " " << m_BoundingBox.lb[1] << " " << m_BoundingBox.lb[2] << std::endl;
		std::cout << "m_BoundingBox.ub:" << m_BoundingBox.ub[0] << " " << m_BoundingBox.ub[1] << " " << m_BoundingBox.ub[2] << std::endl;
		std::cout << "gridDim:" << gridDim[0] << " " << gridDim[1] << " " << gridDim[2] << std::endl;
		std::cout << "gridDeltaDist:" << gridDeltaDist[0] << " " << gridDeltaDist[1] << " " << gridDeltaDist[2] << std::endl;

		grid.resize(gridDim[0] * gridDim[1] * gridDim[2]);

		int gridContentCount = 0;

		// 2. Add triangles
		for (int t = 0; t < triangleCount; t++) {
			const Eigen::Vector3f& v0 = out_vertices[out_v_index[3 * t]];
			const Eigen::Vector3f& v1 = out_vertices[out_v_index[3 * t + 1]];
			const Eigen::Vector3f& v2 = out_vertices[out_v_index[3 * t + 2]];

			AABB triBoundingBox(v0, v1, v2);
			//std::cout << t << " triBoundingBox.lb:" << triBoundingBox.lb[0] << " " << triBoundingBox.lb[1] << " " << triBoundingBox.lb[2] << std::endl;
			//std::cout << t << " triBoundingBox.ub:" << triBoundingBox.ub[0] << " " << triBoundingBox.ub[1] << " " << triBoundingBox.ub[2] << std::endl;
			Eigen::Vector3f loopExtentMinf = (triBoundingBox.lb - m_BoundingBox.lb).cwiseQuotient(gridDeltaDist);
			Eigen::Vector3f loopExtentMaxf = (triBoundingBox.ub - m_BoundingBox.lb).cwiseQuotient(gridDeltaDist);
			Eigen::Vector3i loopExtentMin(floor(loopExtentMinf[0]), floor(loopExtentMinf[1]), floor(loopExtentMinf[2]));
			Eigen::Vector3i loopExtentMax(floor(loopExtentMaxf[0]), floor(loopExtentMaxf[1]), floor(loopExtentMaxf[2]));
			loopExtentMin = loopExtentMin.cwiseMin(gridDim - Eigen::Vector3i(1, 1, 1));
			loopExtentMax = loopExtentMax.cwiseMin(gridDim - Eigen::Vector3i(1, 1, 1));
			//std::cout << t << " loopExtentMin:" << loopExtentMin[0] << " " << loopExtentMin[1] << " " << loopExtentMin[2] << std::endl;
			//std::cout << t << " loopExtentMax:" << loopExtentMax[0] << " " << loopExtentMax[1] << " " << loopExtentMax[2] << std::endl;
			for (int i = loopExtentMin[0]; i <= loopExtentMax[0]; i++) {
				for (int j = loopExtentMin[1]; j <= loopExtentMax[1]; j++) {
					for (int k = loopExtentMin[2]; k <= loopExtentMax[2]; k++) {
						grid[k * gridDim[0] * gridDim[1] + j * gridDim[0] + i].push_back(t);
						gridContentCount++;
					}
				}
			}
		}
		std::cout << "gridContentCount:" << gridContentCount << std::endl << std::endl;

		isUniformExisting = true;
	}

};
