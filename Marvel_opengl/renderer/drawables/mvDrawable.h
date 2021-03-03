#pragma once
#include <vector>
#include <memory>
#include "mvMath.h"
#include "mvIndexBuffer.h"
#include "mvVertexBuffer.h"
#include "mvInputLayout.h"

namespace Marvel {

	//-----------------------------------------------------------------------------
	// forward declarations
	//-----------------------------------------------------------------------------
	class mvBindable;
	class mvGraphics;

	//-----------------------------------------------------------------------------
	// mvDrawable
	//-----------------------------------------------------------------------------
	class mvDrawable
	{

	public:

		virtual void      draw         (mvGraphics& graphics) const;
		virtual glm::mat4 getTransform () const = 0;

		void              addBindable  (mvGraphics& graphics, std::shared_ptr<mvBindable> bindable);
		void              bind         (mvGraphics& graphics) const;
		size_t            getIndexCount() const;
		

	protected:

		std::unique_ptr<mvIndexBuffer>           m_indexBuffer;
		std::unique_ptr<mvVertexBuffer>          m_vertexBuffer;
		std::unique_ptr<mvInputLayout>           m_layout; // for vao
		std::vector<std::shared_ptr<mvBindable>> m_bindables;

	};

}