#include "mvMouse.h"
#include "mvMarvelWin.h"

namespace Marvel {


	std::pair<int, int> mvMouse::getPos() const
	{
		return { m_x, m_y };
	}

	std::optional<mvMouse::RawDelta> mvMouse::readRawDelta()
	{
		if (m_rawDeltaBuffer.empty())
		{
			return std::nullopt;
		}
		const RawDelta d = m_rawDeltaBuffer.front();
		m_rawDeltaBuffer.pop();
		return d;
	}

	int mvMouse::getPosX() const
	{
		return m_x;
	}

	int mvMouse::getPosY() const
	{
		return m_y;
	}

	bool mvMouse::isInWindow() const
	{
		return m_isInWindow;
	}

	bool mvMouse::isLeftPressed() const
	{
		return m_leftPressed;
	}


	std::optional<mvMouse::Event> mvMouse::read()
	{
		if (m_buffer.size() > 0u)
		{
			mvMouse::Event e = m_buffer.front();
			m_buffer.pop();
			return e;
		}
		return {};
	}

	void mvMouse::flush()
	{
		m_buffer = std::queue<Event>();
	}

	void mvMouse::enableRaw()
	{
		m_rawEnabled = true;
	}

	void mvMouse::disableRaw()
	{
		m_rawEnabled = false;
	}

	bool mvMouse::rawEnabled() const
	{
		return m_rawEnabled;
	}

	void mvMouse::onMouseMove(int newx, int newy)
	{
		m_x = newx;
		m_y = newy;

		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::Move, *this));
		trimBuffer();
	}

	void mvMouse::onMouseLeave()
	{
		m_isInWindow = false;
		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::Leave, *this));
		trimBuffer();
	}

	void mvMouse::onMouseEnter()
	{
		m_isInWindow = true;
		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::Enter, *this));
		trimBuffer();
	}

	void mvMouse::onRawDelta(int dx, int dy)
	{
		m_rawDeltaBuffer.push({ dx,dy });
		trimBuffer();
	}

	void mvMouse::onLeftPressed(int x, int y)
	{
		m_leftPressed = true;

		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::LPress, *this));
		trimBuffer();
	}

	void mvMouse::onLeftReleased(int x, int y)
	{
		m_leftPressed = false;

		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::LRelease, *this));
		trimBuffer();
	}

	void mvMouse::onRightPressed(int x, int y)
	{
		m_rightPressed = true;

		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::RPress, *this));
		trimBuffer();
	}

	void mvMouse::onRightReleased(int x, int y)
	{
		m_rightPressed = false;

		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::RRelease, *this));
		trimBuffer();
	}

	void mvMouse::onWheelUp(int x, int y)
	{
		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::WheelUp, *this));
		trimBuffer();
	}

	void mvMouse::onWheelDown(int x, int y)
	{
		m_buffer.push(mvMouse::Event(mvMouse::Event::Type::WheelDown, *this));
		trimBuffer();
	}

	void mvMouse::trimBuffer()
	{
		while (m_buffer.size() > m_bufferSize)
			m_buffer.pop();
	}

	void mvMouse::trimRawInputBuffer()
	{
		while (m_rawDeltaBuffer.size() > m_bufferSize)
			m_rawDeltaBuffer.pop();
	}

	void mvMouse::onWheelDelta(int x, int y, int delta)
	{
		m_wheelDeltaCarry += delta;

		// generate events for every 120 
		while (m_wheelDeltaCarry >= WHEEL_DELTA)
		{
			m_wheelDeltaCarry -= WHEEL_DELTA;
			onWheelUp(x, y);
		}
		while (m_wheelDeltaCarry <= -WHEEL_DELTA)
		{
			m_wheelDeltaCarry += WHEEL_DELTA;
			onWheelDown(x, y);
		}
	}

}