#pragma once

#include "D3D11Drawable.h"
#include "../Bindable/D3D11IndexBuffer.h"

namespace Devil
{
	template<class T>
	class D3D11DrawableBase : public D3D11Drawable
	{
	protected:
		static bool IsStaticInitialized() noexcept
		{
			return !s_StaticBinds.empty();
		}

		static void AddStaticBind(std::unique_ptr<D3D11Bindable> bind) noexcept
		{
			assert("*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(D3D11IndexBuffer));
			s_StaticBinds.push_back(std::move(bind));
		}

		void AddStaticIndexBuffer(std::unique_ptr<D3D11IndexBuffer> indexBuffer) noexcept
		{
			assert("Attempting to add index buffer a second time" && m_IndexBuffer == nullptr);
			m_IndexBuffer = indexBuffer.get();
			s_StaticBinds.push_back(std::move(indexBuffer));
		}


		/** For all objects that do not have static initialization, we get one IndexBuffer from the static binding */
		void SetIndexFromStatic() noexcept
		{
			assert("Attempting to add index buffer a second time" && m_IndexBuffer == nullptr);
			for (const auto& b : s_StaticBinds)
			{
				if (const auto p = dynamic_cast<D3D11IndexBuffer*>(b.get()))
				{
					m_IndexBuffer = p;
					return;
				}
			}
			assert("Failed to find index buffer in static binds" && m_IndexBuffer != nullptr);
		}

	private:
		virtual const std::vector<std::unique_ptr<D3D11Bindable>>& GetStaticBinds() const noexcept
		{
			return s_StaticBinds;
		}

	private:
		static std::vector<std::unique_ptr<D3D11Bindable>> s_StaticBinds;
	};

	template<class T>
	std::vector<std::unique_ptr<D3D11Bindable>> D3D11DrawableBase<T>::s_StaticBinds{};
}