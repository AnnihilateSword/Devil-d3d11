#include "EnginePCH.h"

#include "D3D11Box.h"
#include "../Bindable/D3D11BindableBase.h"
#include "../Geometry/D3D11Geometry.h"

using namespace DirectX;

namespace Devil
{
	D3D11Box::D3D11Box(D3D11Renderer& renderer, DirectX::XMFLOAT3& position, DirectX::XMFLOAT3& rotation, DirectX::XMFLOAT3& scale)
		: m_Position{ position },
		m_Rotation{ rotation },
		m_Scale{ scale }
	{
		// Check Static Bind
		if (!IsStaticInitialized())
		{
			const auto model = Geometry::CreateBox<VertexPosTex, unsigned int>();
			
			AddStaticBind(std::make_unique<D3D11VertexBuffer>(renderer, model.vertexVec));

			auto pvs = std::make_unique<D3D11VertexShader>(renderer, L"./shaders/hlsl/PosTex_VS.cso");
			auto pvsbc = pvs->GetBytecodeBlob();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<D3D11PixelShader>(renderer, L"./shaders/hlsl/PosTex_PS.cso"));

			AddStaticBind(std::make_unique<D3D11Texture>(renderer, "./content/textures/container2.png"));
			AddStaticBind(std::make_unique<D3D11Sampler>(renderer));

			AddStaticIndexBuffer(std::make_unique<D3D11IndexBuffer>(renderer, model.indexVec));

			struct ConstantBuffer
			{
				struct
				{
					float r;
					float g;
					float b;
					float a;
				} face_colors;
			};
			const ConstantBuffer cb =
			{
				{1.0f, 0.0f, 1.0f, 1.0f},
			};

			AddStaticBind(std::make_unique<D3D11PSConstantBuffer<ConstantBuffer>>(renderer, cb));
		
			AddStaticBind(std::make_unique<D3D11InputLayout>(renderer, VertexPosTex::inputLayout,
				(unsigned int)ARRAYSIZE(VertexPosTex::inputLayout), pvsbc));

			AddStaticBind(std::make_unique<D3D11Topology>(renderer, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		// object transform bind is not static.
		AddBind(std::make_unique<D3D11TransformCbuf>(renderer, *this));
	}

	void D3D11Box::Update(float deltaTime) noexcept
	{
	}

	DirectX::XMMATRIX D3D11Box::GetTransformXM() const noexcept
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_Rotation)) *
			XMMatrixScalingFromVector(XMLoadFloat3(&m_Scale)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&m_Position));
	}
}