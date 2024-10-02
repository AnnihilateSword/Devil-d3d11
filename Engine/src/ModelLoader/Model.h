#pragma once

#include "Platform/D3D11/Object/DObject.h"
#include "Platform/D3D11/Bindable/D3D11BindableBase.h"
#include "Platform/D3D11/Drawable/D3D11DrawableBase.h"
#include "Platform/D3D11/Geometry/D3D11Geometry.h"
#include "Platform/D3D11/Geometry/Vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace DirectX;

namespace Devil
{
	/*********/
	/** Mesh */
	/*********/
	class Mesh : public D3D11DrawableBase<Mesh>
	{
	public:
		Mesh(D3D11Renderer& renderer, std::vector<std::unique_ptr<D3D11Bindable>> bindPtrs);

		void Draw(D3D11Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform) const noexcept;

		DirectX::XMMATRIX GetTransformXM() const noexcept override;

	private:
		mutable DirectX::XMFLOAT4X4 m_Transform{};
	};



	/*********/
	/** Node */
	/*********/
	class Node
	{
		friend class Model;
	public:
		Node(std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept;

		void Draw(D3D11Renderer& renderer, DirectX::FXMMATRIX accumulatedTransform) const noexcept;

	private:
		/** only friend model class can use. */
		void AddChild(std::unique_ptr<Node> pChild) noexcept;

	private:
		std::vector<std::unique_ptr<Node>> m_ChildPtrs;
		std::vector<Mesh*> m_MeshPtrs;
		DirectX::XMFLOAT4X4 m_Transform{};
	};



	/**********/
	/** Model */
	/**********/
	class Model : public DObject
	{
	public:
		/** PS Const Buffer */
		struct MaterialConst
		{
			alignas(16) DirectX::XMFLOAT3 color{ 1.0f, 1.0f, 1.0f };
			float specularIntensity{ 1.6f };
			float specularPower{ 32.0f };
			float padding[2];
		};

	public:
		Model(D3D11Renderer& renderer, const std::string fileName,
			/** Transform */
			DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

		std::unique_ptr<Mesh> ParseMesh(D3D11Renderer& renderer, const aiMesh& mesh, const aiMaterial* const* pMaterials);

		std::unique_ptr<Node> ParseNode(const aiNode& node);

		void Draw(D3D11Renderer& renderer) const;

	public:
		/** Getter and Setter */
		//void SetMaterialConst(D3D11Renderer& renderer, MaterialConst& materialConst) noexcept
		//{
		//	/******************************/
		//	/** Update PS Constant Buffer */
		//	/******************************/
		//	m_MaterialConst = materialConst;

		//	AddBind(std::make_unique<D3D11PSConstantBuffer<MaterialConst>>(renderer, m_MaterialConst, 1u));
		//}

		//void SetTexture(D3D11Renderer& renderer, const std::string& textureFilename)
		//{
		//	AddBind(std::make_unique<D3D11Texture>(renderer, textureFilename.c_str()));
		//}

		MaterialConst& GetMaterialConst() noexcept { return m_MaterialConst; }

		/** World Transform */
		DirectX::XMMATRIX GetTransformXM() const noexcept;

	public:
		std::unique_ptr<Node> m_Root;
		std::vector<std::unique_ptr<Mesh>> m_MeshPtrs;

		// properties
		MaterialConst m_MaterialConst{};

	private:
		std::string m_ModelDirectory{};
	};
}