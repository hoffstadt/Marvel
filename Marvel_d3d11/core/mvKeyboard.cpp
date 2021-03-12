#include "mvKeyboard.h"

namespace Marvel {

	bool mvKeyboard::keyIsPressed(unsigned char keycode) const
	{
		return m_keystates[keycode];
	}

	std::optional<mvKeyboard::Event> mvKeyboard::readKey()
	{
		if (m_keybuffer.size() > 0u)
		{
			mvKeyboard::Event e = m_keybuffer.front();
			m_keybuffer.pop();
			return e;
		}
		return {};
	}

	bool mvKeyboard::keyIsEmpty() const
	{
		return m_keybuffer.empty();
	}

	std::optional<char> mvKeyboard::readChar()
	{
		if (m_charbuffer.size() > 0u)
		{
			unsigned char charcode = m_charbuffer.front();
			m_charbuffer.pop();
			return charcode;
		}
		return {};
	}

	bool mvKeyboard::charIsEmpty() const
	{
		return m_charbuffer.empty();
	}

	void mvKeyboard::flushKey()
	{
		m_keybuffer = std::queue<Event>();
	}

	void mvKeyboard::flushChar()
	{
		m_charbuffer = std::queue<char>();
	}

	void mvKeyboard::flush()
	{
		flushKey();
		flushChar();
	}

	void mvKeyboard::enableAutorepeat()
	{
		m_autorepeatEnabled = true;
	}

	void mvKeyboard::disableAutorepeat()
	{
		m_autorepeatEnabled = false;
	}

	bool mvKeyboard::autorepeatIsEnabled() const
	{
		return m_autorepeatEnabled;
	}

	void mvKeyboard::onKeyPressed(unsigned char keycode)
	{
		m_keystates[keycode] = true;
		m_keybuffer.push(mvKeyboard::Event(mvKeyboard::Event::Type::Press, keycode));
		TrimBuffer(m_keybuffer);
	}

	void mvKeyboard::onKeyReleased(unsigned char keycode)
	{
		m_keystates[keycode] = false;
		m_keybuffer.push(mvKeyboard::Event(mvKeyboard::Event::Type::Release, keycode));
		TrimBuffer(m_keybuffer);
	}

	void mvKeyboard::onChar(char character)
	{
		m_charbuffer.push(character);
		TrimBuffer(m_charbuffer);
	}

	void mvKeyboard::clearState()
	{
		m_keystates.reset();
	}

	template<typename T>
	void mvKeyboard::TrimBuffer(std::queue<T>& buffer)
	{
		while(buffer.size() > m_bufferSize)
			buffer.pop();
	}


}