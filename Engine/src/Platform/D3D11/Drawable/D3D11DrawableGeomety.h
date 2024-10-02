#pragma once

#include "D3D11DrawableBase.h"
#include "Platform/D3D11/Object/DObject.h"
#include "../Geometry/D3D11Geometry.h"

namespace Devil
{
	enum class DrawableGeometryType
	{
		Box,
		Sphere,
		Plane,
		MAX_Default
	};

	/**********************************************/
	/** D3D11DrawableGeometry_BlinnPhongPosNorTex */
	/**********************************************/
	class D3D11DrawableGeometry_BlinnPhongPosNorTex : public D3D11DrawableBase<D3D11DrawableGeometry_BlinnPhongPosNorTex>, public DObject
	{
	public:
		/** PS Const Buffer */
		struct MaterialConst
		{
			alignas(16) DirectX::XMFLOAT3 color{ 1.0f, 1.0f, 1.0f };
			float specularIntensity{ 0.6f };
			float specularPower{ 32 };
			float padding[2];
		};
	public:
		D3D11DrawableGeometry_BlinnPhongPosNorTex(D3D11Renderer& renderer, DrawableGeometryType drawableGeometryType = DrawableGeometryType::Box,
			DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
			std::string textureFilename = "./content/textures/default_texture.png");

	public:
		void Update(float deltaTime) noexcept override;

		/** Getter and Setter */
		void SetMaterialConst(D3D11Renderer& renderer, MaterialConst& materialConst) noexcept;
		void SetTexture(D3D11Renderer& renderer, const std::string& textureFilename);

		MaterialConst& GetMaterialConst() noexcept { return m_MaterialConst; };

		/** World Transform */
		DirectX::XMMATRIX GetTransformXM() const noexcept override;

	private:
		DrawableGeometryType m_DrawableGeometryType{};
		Geometry::MeshData<VertexPosNormalTex, unsigned int> m_MeshData{};

		// properties
		MaterialConst m_MaterialConst{};
	};


	/***********************************/
	/** D3D11DrawableGeometry_PosColor */
	/***********************************/
	class D3D11DrawableGeometry_PosColor : public D3D11DrawableBase<D3D11DrawableGeometry_PosColor>, public DObject
	{
	public:
		D3D11DrawableGeometry_PosColor(D3D11Renderer& renderer, DrawableGeometryType drawableGeometryType = DrawableGeometryType::Box,
			DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

	public:
		void Update(float deltaTime) noexcept override;

		/** Getter and Setter */
		void SetMaterialColor(D3D11Renderer& renderer, DirectX::XMFLOAT3& color) noexcept;
		void SetTexture(D3D11Renderer& renderer, std::string textureFilename);

		DirectX::XMFLOAT3& GetMaterialColor() noexcept { return m_MaterialColor; };

		/** World Transform */
		DirectX::XMMATRIX GetTransformXM() const noexcept override;

	private:
		DrawableGeometryType m_DrawableGeometryType{};
		Geometry::MeshData<VertexPosNormalTex, unsigned int> m_MeshData{};

		// properties
		DirectX::XMFLOAT3 m_MaterialColor{};
	};
}