#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "mvBuffer.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvBufferRegistry
	//-----------------------------------------------------------------------------
	class mvBufferRegistry
	{

	public:

		static void                      Initialize(mvGraphics& graphics);
		static std::shared_ptr<mvBuffer> GetBuffer(const std::string& ID);
		static std::shared_ptr<mvBuffer> AddBuffer(const std::string& ID, std::shared_ptr<mvBuffer> buffer);

		template<class T, typename...Params>
		static std::shared_ptr<T> Request(mvGraphics& graphics, Params&&...p)
		{
			static_assert(std::is_base_of<mvBuffer, T>::value, "Can only resolve classes derived from mvBuffer");
			return T::Request(graphics, std::forward<Params>(p)...);
		}

	private:

		static std::unordered_map<std::string, std::shared_ptr<mvBuffer>> s_buffers;

	};

}