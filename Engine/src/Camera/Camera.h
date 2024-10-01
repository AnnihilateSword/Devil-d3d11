#pragma once

#include <DirectXMath.h>

#include "Platform/D3D11/Object/DObject.h"

namespace Devil
{
	class Camera : public DObject
	{
	public:
		Camera(DirectX::XMFLOAT3& position = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f),
			DirectX::XMFLOAT3& rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		virtual ~Camera() = default;

	public:
		/** Setter and Getter */
		DirectX::XMMATRIX GetViewMatrix() const noexcept;

		// imgui window
		void SpawnImGuiControlWindow();
		void Reset();
	};
}