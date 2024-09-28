#pragma once

#include "Event.h"

namespace Devil
{
	class KeyEvent : public Event
	{
	public:
		KeyEvent(unsigned int keycode)
			: m_KeyCode(keycode) {}

		inline unsigned int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		unsigned int m_KeyCode{};
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(unsigned int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[KeyPressedEvent]: " << m_KeyCode << " (" << m_RepeatCount << " repeats)" << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		int m_RepeatCount{};
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(unsigned int keycode)
			: KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[KeyReleasedEvent]: " << m_KeyCode << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(unsigned int keycode)
			: KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[KeyTypedEvent]: " << m_KeyCode << std::endl;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}