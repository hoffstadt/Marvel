#pragma once
#include <vector>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 

namespace Marvel {

	// forward declarations
	class mvIndexBuffer;
	class mvVertexBuffer;
	class mvInputLayout;
	class mvBindable;
	class mvGraphics;

	class mvDrawable
	{

	public:

		mvDrawable() = default;
		virtual ~mvDrawable();

		virtual void draw(mvGraphics& graphics) const;

		void addBindable(mvGraphics& graphics, mvBindable* bindable);

		void bind(mvGraphics& graphics) const;

		size_t getIndexCount() const;

		virtual glm::mat4 getTransform() const = 0;

	protected:

		mvIndexBuffer*           m_indexBuffer;
		mvVertexBuffer*          m_vertexBuffer;
		mvInputLayout*           m_layout; // for vao
		std::vector<mvBindable*> m_bindables;

	};

}