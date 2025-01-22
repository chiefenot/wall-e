#include "data.hpp"

namespace scu {

	const std::map<std::string, Trait> Data::traits = {
		{"T0", {"T0 - NAME", "T0 - DESCRIPTION", 5}},
	};

	const static Choice choiceC0 = { "C0 - DESCRIPTION", {{"T0", -6}}, {{"CH0", -6}} };
	const static Event eventE0 = { "E0 - DESCRIPTION", {}, {}, {}, {{"C0", choiceC0}}, 12 };
	const static Stage stageS0 = { "S0 - NAME", "S0 - DESCRIPTION", 0, 36, {{"E0", eventE0}} };

	const std::map<std::string, Stage> Data::stages = { {"S0", stageS0} };
}