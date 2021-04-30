#include "mvMarvelUtils.h"
#include <sstream>
#include <iomanip>

namespace Marvel {

	std::wstring ToWide(const std::string& narrow)
	{
		wchar_t wide[512];
		mbstowcs_s(nullptr, wide, narrow.c_str(), _TRUNCATE);
		return wide;
	}

	std::string ToNarrow(const std::wstring& wide)
	{
		char narrow[512];
		wcstombs_s(nullptr, narrow, wide.c_str(), _TRUNCATE);
		return narrow;
	}

	glm::vec3 ExtractEulerAngles(const glm::mat4& mat)
	{
		glm::vec3 euler;
		glm::extractEulerAngleXYZ(glm::transpose(mat), euler.x, euler.y, euler.z);
		return euler;
	}

	glm::vec3 ExtractTranslation(const glm::mat4& matrix)
	{
		return { matrix[3][0], matrix[3][1], matrix[3][2] };
	}

}