#include "EnginePCH.h"

#include "D3D11Drawable.h"

namespace Devil
{
	void D3D11Drawable::Draw(D3D11Renderer& renderer) const noexcept
	{
		for (auto& b : m_Binds)
		{
			b->Bind(renderer);
		}
		for (auto& b : GetStaticBinds())
		{
			b->Bind(renderer);
		}
		for (auto& b : m_BindsCleanupFrame)
		{
			b->Bind(renderer);
		}
		renderer.DrawIndexed(m_IndexBuffer->GetCount());
	}

	void D3D11Drawable::AddBind(std::unique_ptr<D3D11Bindable> bind) noexcept
	{
		assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(D3D11IndexBuffer));
		m_Binds.push_back(std::move(bind));
	}

	void D3D11Drawable::AddIndexBuffer(std::unique_ptr<D3D11IndexBuffer> indexBuffer) noexcept
	{
		assert("Attempting to add index buffer a second time" && m_IndexBuffer == nullptr);
		m_IndexBuffer = indexBuffer.get();
		m_Binds.push_back(std::move(indexBuffer));
	}

	void D3D11Drawable::AddBindAndCleanupFrame(std::unique_ptr<D3D11Bindable> bind) noexcept
	{
		m_BindsCleanupFrame.push_back(std::move(bind));
	}

	void D3D11Drawable::CleanupFrameBinds() noexcept
	{
		for (auto& bind : m_BindsCleanupFrame)
		{
			auto rawPtr = bind.release();
			delete rawPtr;
		}
		m_BindsCleanupFrame.clear();
	}
}