#pragma once
#include <string>
#include "mvMath.h"

namespace Marvel {

	std::wstring ToWide(const std::string& narrow);
	std::string  ToNarrow(const std::wstring& wide);

	glm::vec3 ExtractEulerAngles(const glm::mat4& mat);
	glm::vec3 ExtractTranslation(const glm::mat4& matrix);
}