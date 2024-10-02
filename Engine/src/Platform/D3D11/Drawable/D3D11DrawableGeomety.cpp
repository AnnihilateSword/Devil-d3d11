#include "EnginePCH.h"

#include "D3D11DrawableGeomety.h"
#include "../Bindable/D3D11BindableBase.h"

using namespace DirectX;

namespace Devil
{
	/**********************************************/
	/** D3D11DrawableGeometry_BlinnPhongPosNorTex */
	/**********************************************/
	D3D11DrawableGeometry_BlinnPhongPosNorTex::D3D11DrawableGeometry_BlinnPhongPosNorTex(D3D11Renderer& renderer, DrawableGeometryType drawableGeometryType,
		DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& rotation, DirectX::XMFLOAT3& scale, std::string textureFilename)
		: DObject(position, rotation, scale),
		m_MaterialConst{ { 1.0f, 1.0f, 1.0f }, 0.8f, 64.0f },
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

		auto pvs = std::make_unique<D3D11VertexShader>(renderer, L"./shaders/hlsl/BlinnPhongPosNorTex_VS.cso");
		auto pvsbc = pvs->GetBytecodeBlob();
		AddBind(std::move(pvs));

		AddBind(std::make_unique<D3D11PixelShader>(renderer, L"./shaders/hlsl/BlinnPhongPosNorTex_PS.cso"));

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
		// add bind (vs) constant buffer
		AddBind(std::make_unique<D3D11TransformCbuf>(renderer, *this));


		// ******************
		// (b0) is Light Data
		// ******************
		// add bind (ps) constant buffer (b1)
		SetMaterialConst(renderer, m_MaterialConst);
	}

	void D3D11DrawableGeometry_BlinnPhongPosNorTex::Update(float deltaTime) noexcept
	{
	}

	void D3D11DrawableGeometry_BlinnPhongPosNorTex::SetMaterialConst(D3D11Renderer& renderer, MaterialConst& materialConst) noexcept
	{
		/******************************/
		/** Update PS Constant Buffer */
		/******************************/
		m_MaterialConst = materialConst;

		AddBind(std::make_unique<D3D11PSConstantBuffer<MaterialConst>>(renderer, m_MaterialConst, 1u));
	}

	void D3D11DrawableGeometry_BlinnPhongPosNorTex::SetTexture(D3D11Renderer& renderer, const std::string& textureFilename)
	{
		AddBind(std::make_unique<D3D11Texture>(renderer, textureFilename.c_str()));
	}

	DirectX::XMMATRIX D3D11DrawableGeometry_BlinnPhongPosNorTex::GetTransformXM() const noexcept
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
		m_MaterialColor{ { 1.0f, 1.0f, 1.0f } },
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
		SetMaterialColor(renderer, m_MaterialColor.color);
	}

	void D3D11DrawableGeometry_PosColor::Update(float deltaTime) noexcept
	{
	}

	void D3D11DrawableGeometry_PosColor::SetMaterialColor(D3D11Renderer& renderer, DirectX::XMFLOAT3& color) noexcept
	{
		m_MaterialColor.color = color;

		/** Update PS Constant Buffer */
		// Prevent memory leaks!!!
		AddBindAndCleanupFrame(std::make_unique<D3D11PSConstantBuffer<ConstantBuffer>>(renderer, m_MaterialColor));
		// Prevent memory leaks!!!
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