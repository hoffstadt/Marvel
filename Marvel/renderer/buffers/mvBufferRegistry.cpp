#include "mvBufferRegistry.h"
#include "mvCommonBuffers.h"
#include <assert.h>
#include "mvGraphics.h"

namespace Marvel {

	std::unordered_map<std::string, std::shared_ptr<mvBuffer>> mvBufferRegistry::s_buffers;

	void mvBufferRegistry::Initialize(mvGraphics& graphics)
	{
		s_buffers.try_emplace("transCBuf", std::make_shared<mvTransformConstantBuffer>(graphics));
	}

	std::shared_ptr<mvBuffer> mvBufferRegistry::GetBuffer(const std::string& ID)
	{
		if (s_buffers.find(ID) == s_buffers.end())
		{
			//assert(false);
			return nullptr;
		}

		return s_buffers.at(ID);
	}

	std::shared_ptr<mvBuffer> mvBufferRegistry::AddBuffer(const std::string& ID, std::shared_ptr<mvBuffer> buffer)
	{
		assert(s_buffers.find(ID) == s_buffers.end());

		s_buffers.try_emplace(ID, buffer);

		return buffer;
	}

}