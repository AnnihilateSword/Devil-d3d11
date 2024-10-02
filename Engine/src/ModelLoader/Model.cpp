#include "EnginePCH.h"

#include "Model.h"

using namespace DirectX;

namespace Devil
{
	/*********/
	/** Mesh */
	/*********/
	Mesh::Mesh(D3D11Renderer& renderer, std::vector<std::unique_ptr<D3D11Bindable>> bindPtrs)
	{
		if (!IsStaticInitialized())
		{
			AddStaticBind(std::make_unique<D3D11Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		for (auto& pb : bindPtrs)
		{
			if (auto pi = dynamic_cast<D3D11IndexBuffer*>(pb.get()))
			{
				AddIndexBuffer(std::unique_ptr<D3D11IndexBuffer>{ pi });
				pb.release();
			}
			else
			{
				AddBind(std::move(pb));
			}
		}

		AddBind(std::make_unique<D3D11TransformCbuf>(renderer, *this));
	}

	void Mesh::Draw(D3D11Renderer& renderer, FXMMATRIX accumulatedTransform) const noexcept
	{
		XMStoreFloat4x4(&m_Transform, accumulatedTransform);
		D3D11Drawable::Draw(renderer);
	}

	XMMATRIX Mesh::GetTransformXM() const noexcept
	{
		return XMLoadFloat4x4(&m_Transform);
	}



	/*********/
	/** Node */
	/*********/
	Node::Node(std::vector<Mesh*> meshPtrs, const XMMATRIX& transform) noexcept
		:
		m_MeshPtrs(std::move(meshPtrs))
	{
		XMStoreFloat4x4(&this->m_Transform, transform);
	}

	void Node::Draw(D3D11Renderer& renderer, FXMMATRIX accumulatedTransform) const noexcept
	{
		const auto built = XMLoadFloat4x4(&m_Transform) * accumulatedTransform;
		for (const auto pm : m_MeshPtrs)
		{
			pm->Draw(renderer, built);
		}
		for (const auto& pc : m_ChildPtrs)
		{
			pc->Draw(renderer, built);
		}
	}

	void Node::AddChild(std::unique_ptr<Node> pChild) noexcept
	{
		assert(pChild);
		m_ChildPtrs.push_back(std::move(pChild));
	}



	/**********/
	/** Model */
	/**********/
	Model::Model(D3D11Renderer& renderer, const std::string fileName,
		/** Transform */
		XMFLOAT3& position, XMFLOAT3& rotation, XMFLOAT3& scale)
		: DObject(position, rotation, scale)
	{
		// store directory string
		m_ModelDirectory = fileName.substr(0, fileName.find_last_of('/') + 1);

		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(fileName.c_str(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			m_MeshPtrs.push_back(ParseMesh(renderer, *pScene->mMeshes[i], pScene->mMaterials));
		}

		m_Root = ParseNode(*pScene->mRootNode);
	}

	std::unique_ptr<Mesh> Model::ParseMesh(D3D11Renderer& renderer, const aiMesh& mesh, const aiMaterial* const* pMaterials)
	{
		std::vector<VertexPosNormalTex> vertices;
		vertices.reserve(mesh.mNumVertices);
		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vertices.push_back(
				{
					*reinterpret_cast<XMFLOAT3*>(&mesh.mVertices[i]),			// position
					*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i]),			// normal
					*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i])	// texcoord
				}
			);
		}

		std::vector<unsigned int> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		/** Bindable */
		std::vector<std::unique_ptr<D3D11Bindable>> bindablePtrs;

		// If there is no texture then the index is negative.
		// *******
		// Texture
		// *******
		bool hasSpecularMap{ false };
		float shininess{ 35.0f };  // specular power
		if (mesh.mMaterialIndex >= 0)
		{
			// Each grid has its own material index.
			const aiMaterial& material = *pMaterials[mesh.mMaterialIndex];
			aiString texFileName;
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				bindablePtrs.push_back(std::make_unique<D3D11Texture>(renderer, 
					m_ModelDirectory + std::string(texFileName.C_Str()), 0u));
			}
			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				bindablePtrs.push_back(std::make_unique<D3D11Texture>(renderer, 
					m_ModelDirectory + std::string(texFileName.C_Str()), 1u));
				hasSpecularMap = true;
			}
			else
			{
				material.Get(AI_MATKEY_SHININESS, shininess);
			}

			bindablePtrs.push_back(std::make_unique<D3D11Sampler>(renderer));
		}


		bindablePtrs.push_back(std::make_unique<D3D11VertexBuffer>(renderer, vertices));
		bindablePtrs.push_back(std::make_unique<D3D11IndexBuffer>(renderer, indices));

		auto pvs = std::make_unique<D3D11VertexShader>(renderer, L"./shaders/hlsl/BlinnPhongPosNorTex_VS.cso");
		auto pvsbc = pvs->GetBytecodeBlob();
		bindablePtrs.push_back(std::move(pvs));

		// Check has Specular Map
		if (hasSpecularMap)
		{
			bindablePtrs.push_back(std::make_unique<D3D11PixelShader>(renderer, L"./shaders/hlsl/BlinnPhongPosNorTexSpec_PS.cso"));
		}
		else
		{
			bindablePtrs.push_back(std::make_unique<D3D11PixelShader>(renderer, L"./shaders/hlsl/BlinnPhongPosNorTex_PS.cso"));

			// PS const buffer
			m_MaterialConst =
			{
				{ 1.0f, 1.0f, 1.0f },
				0.8f,
				shininess
			};
			bindablePtrs.push_back(std::make_unique<D3D11PSConstantBuffer<MaterialConst>>(renderer, m_MaterialConst, 1u));
		}

		bindablePtrs.push_back(std::make_unique<D3D11InputLayout>(renderer, VertexPosNormalTex::inputLayout,
			(unsigned int)ARRAYSIZE(VertexPosNormalTex::inputLayout), pvsbc));


		return std::make_unique<Mesh>(renderer, std::move(bindablePtrs));
	}

	std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
	{
		const auto transform = XMMatrixTranspose(XMLoadFloat4x4(
			reinterpret_cast<const XMFLOAT4X4*>(&node.mTransformation)
		));

		std::vector<Mesh*> curMeshPtrs;
		curMeshPtrs.reserve(node.mNumMeshes);
		for (size_t i = 0; i < node.mNumMeshes; i++)
		{
			const auto meshIdx = node.mMeshes[i];
			curMeshPtrs.push_back(m_MeshPtrs.at(meshIdx).get());
		}

		auto pNode = std::make_unique<Node>(std::move(curMeshPtrs), transform);
		for (size_t i = 0; i < node.mNumChildren; i++)
		{
			pNode->AddChild(ParseNode(*node.mChildren[i]));
		}

		return pNode;
	}

	void Model::Draw(D3D11Renderer& renderer) const
	{
		m_Root->Draw(renderer, GetTransformXM());
	}

	/** World Transform */
	XMMATRIX Model::GetTransformXM() const noexcept
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation)) *
			XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));
	}
}