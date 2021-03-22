#pragma once
#include <queue>
#include <optional>

namespace Marvel {

	class mvMouse
	{

		friend class mvWindow;

	public:

		struct RawDelta { int x, y;};

		class Event
		{

		public:

			enum class Type
			{
				LPress, LRelease, RPress, RRelease,
				WheelUp, WheelDown, Move, Enter, Leave,
			};

		public:
			Event(Type type, const mvMouse& parent)
				: m_type(type), m_leftPressed(parent.m_leftPressed), m_rightPressed(parent.m_rightPressed), m_x(parent.m_x), m_y(parent.m_y)
			{}

			Type                getType       () const { return m_type;}
			std::pair<int, int> getPos        () const { return{ m_x, m_y };}
			int                 getPosX       () const { return m_x; }
			int                 getPosY       () const { return m_y;}
			bool                isLeftPressed () const { return m_leftPressed;}
			bool                isRightPressed() const { return m_rightPressed;}

		private:

			Type m_type;
			bool m_leftPressed;
			bool m_rightPressed;
			int  m_x;
			int  m_y;
		};

	public:

		mvMouse() = default;

		mvMouse(const mvMouse&) = delete;
		mvMouse& operator=(const mvMouse&) = delete;

		std::pair<int, int>           getPos        () const;
		std::optional<RawDelta>       readRawDelta  ();
		int                           getPosX       () const;
		int                           getPosY       () const;
		bool                          isInWindow    () const;
		bool                          isLeftPressed () const;
		bool                          isRightPressed() const;
		std::optional<mvMouse::Event> read          ();
		bool                          isEmpty       () const { return m_buffer.empty();}
		void                          flush         ();
		void                          enableRaw     ();
		void                          disableRaw    ();
		bool                          rawEnabled    () const;
	
	private:

		void onMouseMove       (int x, int y);
		void onMouseLeave      ();
		void onMouseEnter      ();
		void onRawDelta        (int dx, int dy);
		void onLeftPressed     (int x, int y);
		void onLeftReleased    (int x, int y);
		void onRightPressed    (int x, int y);
		void onRightReleased   (int x, int y);
		void onWheelUp         (int x, int y);
		void onWheelDown       (int x, int y);
		void trimBuffer        ();
		void trimRawInputBuffer();
		void onWheelDelta      (int x, int y, int delta);
	
	private:

		static constexpr unsigned int m_bufferSize = 16u;
		int                           m_x;
		int                           m_y;
		bool                          m_leftPressed = false;
		bool                          m_rightPressed = false;
		bool                          m_isInWindow = false;
		int                           m_wheelDeltaCarry = 0;
		bool                          m_rawEnabled = false;
		std::queue<Event>             m_buffer;
		std::queue<RawDelta>          m_rawDeltaBuffer;
	};

}