#include "EnginePCH.h"

#include "D3D11DrawableGeomety.h"
#include "../Bindable/D3D11BindableBase.h"

using namespace DirectX;

namespace Devil
{
	/********************************************/
	/** D3D11DrawableGeometry_PhongPosNormalTex */
	/********************************************/
	D3D11DrawableGeometry_PhongPosNormalTex::D3D11DrawableGeometry_PhongPosNormalTex(D3D11Renderer& renderer, DrawableGeometryType drawableGeometryType,
		DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& rotation, DirectX::XMFLOAT3& scale, std::string textureFilename)
		: DObject(position, rotation, scale),
		m_MaterialColor{ 1.0f, 1.0f, 1.0f },
		m_DrawableGeometryType{ drawableGeometryType }
	{
		// select geometry type
		switch (drawableGeometryType)
		{
		case Devil::DrawableGeometryType::Box:
			m_MeshData = Geometry::CreateBox<VertexPosNormalTex, unsigned int>();
			break;
		case Devil::DrawableGeometryType::Sphere:
			m_MeshData = Geometry::CreateSphere<VertexPosNormalTex, unsigned int>();
			break;
		case Devil::DrawableGeometryType::Plane:
			m_MeshData = Geometry::CreatePlane<VertexPosNormalTex, unsigned int>();
			break;
		case Devil::DrawableGeometryType::MAX_Default:
			break;
		default:
			break;
		}

		AddBind(std::make_unique<D3D11VertexBuffer>(renderer, m_MeshData.vertexVec));

		auto pvs = std::make_unique<D3D11VertexShader>(renderer, L"./shaders/hlsl/PhongPosNormalTex_VS.cso");
		auto pvsbc = pvs->GetBytecodeBlob();
		AddBind(std::move(pvs));

		AddBind(std::make_unique<D3D11PixelShader>(renderer, L"./shaders/hlsl/PhongPosNormalTex_PS.cso"));

		// add bind texture
		AddBind(std::make_unique<D3D11Texture>(renderer, textureFilename.c_str()));
		AddBind(std::make_unique<D3D11Sampler>(renderer));

		AddIndexBuffer(std::make_unique<D3D11IndexBuffer>(renderer, m_MeshData.indexVec));

		AddBind(std::make_unique<D3D11InputLayout>(renderer, VertexPosNormalTex::inputLayout,
			(unsigned int)ARRAYSIZE(VertexPosNormalTex::inputLayout), pvsbc));

		AddBind(std::make_unique<D3D11Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));



		/***************************************************************************/
		/** Static binding is not needed because it conflicts with the design here */
		/***************************************************************************/
		// Check Static Bind
		/*if (!IsStaticInitialized())
		{
		}
		else
		{
			SetIndexFromStatic();
		}*/

		// object transform bind is not static.
		// add bind vs constant buffer
		AddBind(std::make_unique<D3D11TransformCbuf>(renderer, *this));


		// *************
		// (b0) is Light
		// *************
		// add bind ps constant buffer (b1)
		SetMaterialColor(renderer, m_MaterialColor);
	}

	void D3D11DrawableGeometry_PhongPosNormalTex::Update(float deltaTime) noexcept
	{
	}

	void D3D11DrawableGeometry_PhongPosNormalTex::SetMaterialColor(D3D11Renderer& renderer, DirectX::XMFLOAT3& color) noexcept
	{
		m_MaterialColor = color;

		/** Update PS Constant Buffer */
		struct ConstantBuffer
		{
			XMFLOAT3 color;
			float padding;
		};
		ConstantBuffer colorConst;
		colorConst.color = m_MaterialColor;

		AddBind(std::make_unique<D3D11PSConstantBuffer<ConstantBuffer>>(renderer, colorConst, 1u));
	}

	void D3D11DrawableGeometry_PhongPosNormalTex::SetTexture(D3D11Renderer& renderer, std::string textureFilename)
	{
		AddBind(std::make_unique<D3D11Texture>(renderer, textureFilename.c_str()));
	}

	DirectX::XMMATRIX D3D11DrawableGeometry_PhongPosNormalTex::GetTransformXM() const noexcept
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation)) *
			XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));
	}




	/***********************************/
	/** D3D11DrawableGeometry_PosColor */
	/***********************************/
	D3D11DrawableGeometry_PosColor::D3D11DrawableGeometry_PosColor(D3D11Renderer& renderer, DrawableGeometryType drawableGeometryType,
		DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& rotation, DirectX::XMFLOAT3& scale)
		: DObject(position, rotation, scale),
		m_MaterialColor{ 1.0f, 1.0f, 1.0f },
		m_DrawableGeometryType(drawableGeometryType)
	{
		// Check Static Bind
		if (!IsStaticInitialized())
		{
			// select geometry type
			switch (drawableGeometryType)
			{
			case Devil::DrawableGeometryType::Box:
				m_MeshData = Geometry::CreateBox<VertexPosNormalTex, unsigned int>();
				break;
			case Devil::DrawableGeometryType::Sphere:
				m_MeshData = Geometry::CreateSphere<VertexPosNormalTex, unsigned int>();
				break;
			case Devil::DrawableGeometryType::Plane:
				m_MeshData = Geometry::CreatePlane<VertexPosNormalTex, unsigned int>();
				break;
			case Devil::DrawableGeometryType::MAX_Default:
				break;
			default:
				break;
			}

			AddStaticBind(std::make_unique<D3D11VertexBuffer>(renderer, m_MeshData.vertexVec));

			auto pvs = std::make_unique<D3D11VertexShader>(renderer, L"./shaders/hlsl/PosColor_VS.cso");
			auto pvsbc = pvs->GetBytecodeBlob();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<D3D11PixelShader>(renderer, L"./shaders/hlsl/PosColor_PS.cso"));

			AddStaticIndexBuffer(std::make_unique<D3D11IndexBuffer>(renderer, m_MeshData.indexVec));

			AddStaticBind(std::make_unique<D3D11InputLayout>(renderer, VertexPos::inputLayout,
				(unsigned int)ARRAYSIZE(VertexPos::inputLayout), pvsbc));

			AddStaticBind(std::make_unique<D3D11Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		// object transform bind is not static.
		// add bind vs constant buffer
		AddBind(std::make_unique<D3D11TransformCbuf>(renderer, *this));

		// *************
		// (b0) is Light
		// *************
		// add bind ps constant buffer (b1)
		SetMaterialColor(renderer, m_MaterialColor);
	}

	void D3D11DrawableGeometry_PosColor::Update(float deltaTime) noexcept
	{
	}

	void D3D11DrawableGeometry_PosColor::SetMaterialColor(D3D11Renderer& renderer, DirectX::XMFLOAT3& color) noexcept
	{
		m_MaterialColor = color;

		/** Update PS Constant Buffer */
		struct ConstantBuffer
		{
			XMFLOAT3 color;
			float padding;
		};
		ConstantBuffer colorConst;
		colorConst.color = m_MaterialColor;

		AddBind(std::make_unique<D3D11PSConstantBuffer<ConstantBuffer>>(renderer, colorConst));
	}

	void D3D11DrawableGeometry_PosColor::SetTexture(D3D11Renderer& renderer, std::string textureFilename)
	{
		AddBind(std::make_unique<D3D11Texture>(renderer, textureFilename.c_str()));
	}

	DirectX::XMMATRIX D3D11DrawableGeometry_PosColor::GetTransformXM() const noexcept
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation)) *
			XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));
	}
}