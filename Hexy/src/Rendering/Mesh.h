#pragma once
#include "Buffers.h"
#include "VertexArray.h"
#include "Material.h"
#include <Math/AABB.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>"

namespace Hexy 
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 tangent;
	};

	struct Submesh
	{
		unsigned int baseVertex = 0;
		unsigned int baseIndex = 0;
		unsigned int vertexCount = 0;
		unsigned int materialIndex = 0;

		glm::mat4 transform = glm::mat4(1);

		AABB boundingBox;

		Submesh(unsigned int baseVertex, unsigned int baseIndex, unsigned int vertexCount, unsigned int materialIndex) :
			baseVertex(baseVertex), baseIndex(baseIndex), vertexCount(vertexCount), materialIndex(materialIndex) {}
	};

	class Mesh
	{
	private:
		std::string m_path;
		std::shared_ptr<VertexArray> m_va;
		std::shared_ptr<Material> m_baseMaterial;
		std::vector<std::shared_ptr<Material>> m_materials;
		std::vector<Submesh> m_submeshes;
	public:
		Mesh() = default;
		Mesh(const std::string& path);

		~Mesh();

		void Bind();
		void Load(const std::string& path);

		inline std::shared_ptr<VertexArray> GetVertexArray() { return m_va; }
		inline const std::vector<Submesh>& GetSubmeshes() { return m_submeshes; }

		inline const std::string& GetPath() { return m_path; }
		inline const std::vector<std::shared_ptr<Material>>& GetMaterials() { return m_materials; }

		static std::shared_ptr<Mesh> Create(const std::string& path);
	private:
		void ApplyTransformations(aiNode* node, const glm::mat4& transform);
		glm::mat4 ConvertAssimpMat4(const aiMatrix4x4& matrix);
	};
	
}