#include <iostream>
#include <set>

#include <windows.h>

#include "data.hpp"

std::int32_t kColumns = 0;
std::int32_t kRows = 0;

std::string get_new_event_id(const std::set<std::string>& used_events, const std::map<std::string, std::int32_t>& chains, const std::map<std::string, scu::Trait>& traits) {
	std::string event_id = "";

	bool checks_passed = false;
	while (!checks_passed) {
		int random = 56;// Get random number in range [0, scu::Data::stages.size())
		event_id = "E" + std::to_string(random);


		// Check if event_id exists in used_events

		// Check if event is passsed traits checks

		// Check if event is passsed chains checks

		checks_passed = true; // Results 
	}
	
	return event_id;
}

void show_text(const std::string& text) {
	std::cout << "+" << std::string(kColumns - 2, '-') << "+" << std::endl;
	for (int i = 0; i < kRows - 2; ++i) {
		std::cout << "|" << std::string(kColumns - 2, ' ') << "|" << std::endl;
	}
	std::cout << "+" << std::string(kColumns - 2, '-') << "+" << std::endl;
}

std::string process_event(const scu::Event& event) {
	// Show event
	// Show options to choose
	// Request input
	// Check input correctnes
	// Map input to choice id
	// Retrun choice id
	return "";
}

int main() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	kColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	kRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	show_text("aedghcaerhfaoeihrvohjaervoijer");

	// Show welcome message

	// Request and save character name

	// OPTIONAL: Request and save gender

	// Show initial info

	std::string name;
	std::uint32_t age = 0;
	std::map<std::string, scu::Trait> traits = scu::Data::traits;
	std::map<std::string, std::int32_t> chains;

	bool is_died = false;
	while (!is_died) {
		// Check stage (according to age) 
		
		// Choose the event

		// Process event

		// Apply effects and chains

		is_died = true; // Check that character still alive
	}

}