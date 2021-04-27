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
		//DirectX::XMFLOAT3 euler;
		glm::vec3 euler;

		euler.x = asinf(-mat[2][3]);                  // Pitch
		if (cosf(euler.x) > 0.0001)                // Not at poles
		{
			euler.y = atan2f(mat[1][3], mat[3][3]);      // Yaw
			euler.z = atan2f(mat[2][1], mat[2][2]);      // Roll
		}
		else
		{
			euler.y = 0.0f;                           // Yaw
			euler.z = atan2f(-mat[1][2], mat[1][1]);     // Roll
		}

		return euler;
	}

	glm::vec3 ExtractTranslation(const glm::mat4& matrix)
	{
		return { matrix[0][3], matrix[1][3], matrix[2][3] };
	}

}