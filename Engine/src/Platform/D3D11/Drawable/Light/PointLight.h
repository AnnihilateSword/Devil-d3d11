#pragma once

#include "Platform/D3D11/D3D11Renderer.h"
#include "Platform/D3D11/Drawable/D3D11DrawableGeomety.h"
#include "Platform/D3D11/Bindable/D3D11ConstantBuffers.h"
#include "Platform/D3D11/Object/DObject.h"

namespace Devil
{
	/** 
	 * A point light source stores only 
	 * the light source information and a grid that can be displayed.
	 */
	class PointLight : DObject
	{
	public:
		PointLight(D3D11Renderer& renderer, DirectX::XMFLOAT3& viewPos, DrawableGeometryType drawableType = DrawableGeometryType::Sphere,
			DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3& scale = DirectX::XMFLOAT3(0.12f, 0.12f, 0.12f));

	public:
		void Draw(D3D11Renderer& renderer) noexcept;
		void Update(D3D11Renderer& renderer, float deltaTime) noexcept;

		void Reset(D3D11Renderer& renderer, DirectX::XMFLOAT3& viewPos) noexcept;
		void SpawnImGuiControlWindow(D3D11Renderer& renderer, DirectX::XMFLOAT3& viewPos) noexcept;

	public:
		/** Bind light source information to other objects that can be drawn */
		void AddBindToDrawable(D3D11Renderer& renderer, D3D11Drawable* drawable, unsigned int slot = 0u) noexcept;

	public:
		/** Setter and Getter */
		D3D11DrawableGeometry_PosColor& GetMesh() const { return m_Mesh; }

		virtual void SetPosition(DirectX::XMFLOAT3& position) noexcept override;
		virtual void SetRotation(DirectX::XMFLOAT3& rotation) noexcept override;
		virtual void SetScale(DirectX::XMFLOAT3& scale) noexcept override;

	private:
		struct PointLightCBuf
		{
			alignas(16) DirectX::XMFLOAT3 pos;
			alignas(16) DirectX::XMFLOAT3 ambient;
			alignas(16) DirectX::XMFLOAT3 diffuseColor;
			float diffuseIntensity;
			float attConst;
			float attLin;
			float attQuad;
			alignas(16) DirectX::XMFLOAT3 viewPos;
		};

	private:
		// Constant buffer for point light source
		PointLightCBuf m_CbData{};
		// This grid behind can be considered hidden, currently for debugging purposes
		// default shape is Sphere.
		mutable D3D11DrawableGeometry_PosColor m_Mesh;
	};
}