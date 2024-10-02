#pragma once

#include "Event.h"

namespace Devil
{
	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowActivateEvent : public Event
	{
	public:
		WindowActivateEvent(WPARAM wparam)
			: m_WParam(wparam) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[WindowActivateEvent]: " << "Window Active!!!" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowActivate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	public:
		WPARAM GetWParam() const { return m_WParam; }

	private:
		WPARAM m_WParam{};
	};

	class WindowRawInputEvent : public Event
	{
	public:
		WindowRawInputEvent(LPARAM lparam, WPARAM wparam)
			: m_LParam(lparam), m_WParam(wparam) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[WindowRawInputEvent]: " << "WindowRawInputEvent" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowRawInput)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	public:
		WPARAM GetWParam() const { return m_WParam; }
		WPARAM GetLParam() const { return m_LParam; }

	private:
		WPARAM m_WParam{};
		LPARAM m_LParam{};
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			:m_Width(width), m_Height(height) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[WindowResizeEvent]: " << m_Width << ", " << m_Height << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	public:
		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

	private:
		unsigned int m_Width{}, m_Height{};
	};

	class WindowEnterSizeMoveEvent : public Event
	{
	public:
		WindowEnterSizeMoveEvent() {}

		EVENT_CLASS_TYPE(WindowEnterSizeMove)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowExitSizeMoveEvent : public Event
	{
	public:
		WindowExitSizeMoveEvent() {}

		EVENT_CLASS_TYPE(WindowExitSizeMove)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent(){ }

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}