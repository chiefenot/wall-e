#include <map>
#include <string>

namespace scu {

	struct Trait {
		std::string name;
		std::string description;
		std::int32_t value;
	};

	struct Choice {
		std::string description;

		std::map<std::string, std::int32_t> effects;
		std::map<std::string, std::int32_t> chains;
	};

	struct Event
	{
		std::string description;

		std::map<std::string, std::int32_t> conditions_traits_min;
		std::map<std::string, std::int32_t> conditions_traits_max;
		std::map<std::string, std::int32_t> conditions_chains;

		std::map<std::string, Choice> choices;

		// In month
		std::uint32_t age_increase;
	};

	struct Stage
	{
		std::string name;
		std::string description;

		// In month
		std::uint32_t min_age;
		std::uint32_t max_age;

		std::map<std::string, Event> events;
	};
}