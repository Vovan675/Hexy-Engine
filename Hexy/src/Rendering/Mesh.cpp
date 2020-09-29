#include "pch.h"
#include "Mesh.h"

namespace Hexy
{
	Mesh::Mesh(const std::string& path)
	{
		Load(path);
		m_baseMaterial = std::make_shared<Material>(ShaderLibrary::Use("assets/shaders/SimpleShader.vert", "assets/shaders/SimpleShader.frag"));
	}

	Mesh::~Mesh()
	{
		
	}

	void Mesh::Bind()
	{
		m_va->Bind();
	}

    void Mesh::Load(const std::string& path)
    {
        m_path = path;
        m_submeshes.clear();
        m_materials.clear();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
            aiProcess_Triangulate |
            aiProcess_GenUVCoords |
            aiProcess_GenNormals |
            aiProcess_CalcTangentSpace |
            aiProcess_ValidateDataStructure |
            aiProcess_OptimizeMeshes |
            aiProcess_SortByPType
        );

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        unsigned int vertexCount = 0;
        unsigned int indexCount = 0;
        unsigned int numMeshes = scene->mNumMeshes;
        for (int m = 0; m < numMeshes; m++) {
            aiMesh* mesh = scene->mMeshes[m];

            Submesh& submesh = m_submeshes.emplace_back(vertexCount, indexCount, mesh->mNumFaces * 3, mesh->mMaterialIndex);

            vertexCount += mesh->mNumVertices;
            indexCount += mesh->mNumFaces * 3;

            for (int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                vertex.pos = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z }; //-V108
                vertex.uv = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                vertex.normal = { mesh->mNormals[i].x , mesh->mNormals[i].y, mesh->mNormals[i].z };
                vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y,  mesh->mTangents[i].z };
                vertices.push_back(vertex);

                glm::vec3& min = submesh.boundingBox.min;
                min.x = vertex.pos.x < min.x ? vertex.pos.x : min.x;
                min.y = vertex.pos.y < min.y ? vertex.pos.y : min.y;
                min.z = vertex.pos.z < min.z ? vertex.pos.z : min.z;

                glm::vec3& max = submesh.boundingBox.max;
                max.x = vertex.pos.x > max.x ? vertex.pos.x : max.x;
                max.y = vertex.pos.y > max.y ? vertex.pos.y : max.y;
                max.z = vertex.pos.z > max.z ? vertex.pos.z : max.z;
            }

            for (int i = 0; i < mesh->mNumFaces; i++)
            {
                indices.push_back(mesh->mFaces[i].mIndices[0]);
                indices.push_back(mesh->mFaces[i].mIndices[1]);
                indices.push_back(mesh->mFaces[i].mIndices[2]);
            }
        }

        //Setting up materials
        for (int i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* aiMat = scene->mMaterials[i];

            aiColor3D diffuse;
            aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

            float metallic, shininess, roughness;
            aiMat->Get(AI_MATKEY_REFLECTIVITY, metallic);
            aiMat->Get(AI_MATKEY_SHININESS, shininess); //Shininess
            roughness = 1.0 - sqrt(shininess * 0.01);


            auto material = std::make_shared<Material>(ShaderLibrary::Use("assets/shaders/SimpleShader.vert", "assets/shaders/SimpleShader.frag"));

            aiString texturePath;
            if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == aiReturn_SUCCESS)
            {
                std::filesystem::path path(m_path);
                path = path.parent_path();
                path /= texturePath.data;
                material->Set("u_AlbedoTexture", Texture2D::Create(path.string()));
                material->Set("u_useAlbedoTexture", 1.0f);
            }
            else
            {
                material->Set("u_Albedo", { diffuse.r, diffuse.g, diffuse.b });
            }

            if (aiMat->GetTexture(aiTextureType_NORMALS, 0, &texturePath) == aiReturn_SUCCESS)
            {
                std::filesystem::path path(m_path);
                path = path.parent_path();
                path /= texturePath.data;
                material->Set("u_NormalTexture", Texture2D::Create(path.string()));
                material->Set("u_useNormalTexture", 1.0f);
            }

            /*
            for (int i = 0; i < aiMat->mNumProperties; i++)
            {
                auto p = aiMat->mProperties[i];
                if (p->mType == aiPTI_String)
                {
                    std::cout << std::string(p->mData, p->mDataLength) << std::endl;
                }
            }
            */
            //TODO: FIX METALNESS TEXTURE LOADING
            if (aiMat->GetTexture(aiTextureType::aiTextureType_REFLECTION, 0, &texturePath) == aiReturn_SUCCESS)
            {
                std::filesystem::path path(m_path);
                path = path.parent_path();
                path /= texturePath.data;
                material->Set("u_MetalnessTexture", Texture2D::Create(path.string()));
                material->Set("u_useMetalnessTexture", 1.0f);
            }
            else
            {
                material->Set("u_Metalness", metallic);
            }

            if (aiMat->GetTexture(aiTextureType_SHININESS, 0, &texturePath) == aiReturn_SUCCESS)
            {
                std::filesystem::path path(m_path);
                path = path.parent_path();
                path /= texturePath.data;
                material->Set("u_RoughnessTexture", Texture2D::Create(path.string()));
                material->Set("u_useRoughnessTexture", 1.0f);
            }
            else
            {
                material->Set("u_Roughness", roughness);
            }

            m_materials.push_back(material);
        }
        ApplyTransformations(scene->mRootNode, glm::mat4(1));

        importer.FreeScene();

        m_va = VertexArray::Create();
        auto vb = VertexBuffer::Create(vertices.data(), sizeof(Vertex) * vertices.size());
        vb->SetLayout({
            {Hexy::ElementType::Float3}, //POSITION
            {Hexy::ElementType::Float2}, //UV
            {Hexy::ElementType::Float3}, //NORMAL
            {Hexy::ElementType::Float3}, //TANGENT
            });
        m_va->SetVertexBuffer(vb);

        auto ib = IndexBuffer::Create(indices.data(), indices.size());
        m_va->SetIndexBuffer(ib);
    }

    std::shared_ptr<Mesh> Mesh::Create(const std::string& path)
    {
        return std::make_shared<Mesh>(path);
    }

    void Mesh::ApplyTransformations(aiNode* node, const glm::mat4& transform)
    {
        glm::mat4 t = transform * ConvertAssimpMat4(node->mTransformation);
        for (int i = 0; i < node->mNumMeshes; i++) 
        {
            m_submeshes[node->mMeshes[i]].transform = t;
            m_submeshes[node->mMeshes[i]].boundingBox = m_submeshes[node->mMeshes[i]].boundingBox * t;
        }

        for (int i = 0; i < node->mNumChildren; i++)
        {
            ApplyTransformations(node->mChildren[i], t);
        }
    }

    glm::mat4 Mesh::ConvertAssimpMat4(const aiMatrix4x4& m)
    {
        glm::mat4 o;
        o[0][0] = m.a1; o[0][1] = m.b1; o[0][2] = m.c1; o[0][3] = m.d1;
        o[1][0] = m.a2; o[1][1] = m.b2; o[1][2] = m.c2; o[1][3] = m.d2;
        o[2][0] = m.a3; o[2][1] = m.b3; o[2][2] = m.c3; o[2][3] = m.d3;
        o[3][0] = m.a4; o[3][1] = m.b4; o[3][2] = m.c4; o[3][3] = m.d4;
        return o;
    }
}
