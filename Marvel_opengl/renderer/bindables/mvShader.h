#include "mvBindable.h"

namespace Marvel {

	class mvShader : public mvBindable
	{

	public:

		mvShader(mvGraphics& graphics, const char* vs_path, const char* ps_path);
		~mvShader();

		void bind(mvGraphics& graphics) override;

	private:

		unsigned int m_program;

	};

}