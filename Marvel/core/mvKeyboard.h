#pragma once
#include <queue>
#include <bitset>
#include <optional>

namespace Marvel {

	class mvKeyboard
	{

		friend class mvWindow;

	public:

		class Event
		{

		public:

			enum class Type
			{
				Press,
				Release,
			};

		public:

			Event(Type type, unsigned char code)
				: m_type(type), m_code(code)
			{}

			bool          isPress  () const { return m_type == Type::Press;}
			bool          isRelease() const { return m_type == Type::Release;}
			unsigned char getCode  () const { return m_code;}

		private:

			Type          m_type;
			unsigned char m_code;
		};

	public:

		mvKeyboard() = default;

		mvKeyboard           (const mvKeyboard&) = delete;
		mvKeyboard& operator=(const mvKeyboard&) = delete;

		// key event stuff
		bool keyIsPressed              (unsigned char keycode) const;
		std::optional<Event> readKey();
		bool keyIsEmpty                () const;
		void flushKey                  ();
		
		// char event stuff
		std::optional<char> readChar   ();
		bool                charIsEmpty() const;
		void                flushChar  ();
		void                flush      ();
		
		// autorepeat control
		void enableAutorepeat   ();
		void disableAutorepeat  ();
		bool autorepeatIsEnabled() const;
	
	private:

		void onKeyPressed (unsigned char keycode);
		void onKeyReleased(unsigned char keycode);
		void onChar       (char character);
		void clearState   ();

		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);
	
	private:
		
		static constexpr unsigned int m_nKeys      = 256u;
		static constexpr unsigned int m_bufferSize =  16u;

		bool                 m_autorepeatEnabled = false;
		std::bitset<m_nKeys> m_keystates;
		std::queue<Event>    m_keybuffer;
		std::queue<char>     m_charbuffer;
	};
}