#pragma once
#include <string>

namespace Marvel {

	std::wstring ToWide(const std::string& narrow);
	std::string  ToNarrow(const std::wstring& wide);
}