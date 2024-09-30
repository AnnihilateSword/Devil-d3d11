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

	/********************************************/
	/** D3D11DrawableGeometry_PhongPosNormalTex */
	/********************************************/
	class D3D11DrawableGeometry_PhongPosNormalTex : public D3D11DrawableBase<D3D11DrawableGeometry_PhongPosNormalTex>, public DObject
	{
	public:
		D3D11DrawableGeometry_PhongPosNormalTex(D3D11Renderer& renderer, DrawableGeometryType drawableGeometryType = DrawableGeometryType::Box,
			DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
			std::string textureFilename = "./content/textures/default_texture.png");

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