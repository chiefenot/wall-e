#include <map>
#include <string>

#include "types.hpp"

namespace scu {

	struct Data
	{
		static const std::map<std::string, Trait> traits;
		static const std::map<std::string, Stage> stages;
	};

}