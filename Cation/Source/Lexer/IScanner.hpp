#pragma once

#include <string>

namespace Cation {
	class IScanner {
	public:
		virtual std::wstring GetToken() = 0;
	};
}