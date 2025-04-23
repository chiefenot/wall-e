#include <iostream>
#include <set>
#include <algorithm>

#include <io.h>
#include <fcntl.h>
#include <windows.h>

#include "data.hpp"
#include "types.hpp"

#include <cstdlib>
#include <ctime>

using namespace scu;

std::int32_t get_random_number(std::int32_t min, std::int32_t max) {
    return min + (std::rand() % (max - min));
}

std::int32_t kColumns = 0;
std::int32_t kRows = 0;

std::string get_stage_according_to_age(std::int32_t age) {
    return "S0";
}

std::string get_new_event_id(
    const std::set<std::string>& used_events,
    const std::map<std::string, int>& chains,
    const std::map<std::string, Trait>& traits,
    const std::map<std::string, Stage>& stages,
    const std::string& stage_id
) {
    auto stage_it = stages.find(stage_id);
    if (stage_it == stages.end()) {
        return ""; // Стадия не найдена
    }

    const Stage& stage = stage_it->second;
    std::vector<std::string> valid_events;

    for (const auto& event_pair : stage.events) {
        const Event& event = event_pair.second;

        if (used_events.find(event_pair.first) != used_events.end())
            continue;

        bool traits_ok = true;
        for (const auto& cond : event.conditions_traits_min) {
            auto it = traits.find(cond.first);
            if (it == traits.end() || it->second.value < cond.second) {
                traits_ok = false;
                break;
            }
        }

        for (const auto& cond : event.conditions_traits_max) {
            auto it = traits.find(cond.first);
            if (it == traits.end() || it->second.value > cond.second) {
                traits_ok = false;
                break;
            }
        }

        bool chains_ok = true;
        for (const auto& cond : event.conditions_chains) {
            auto it = chains.find(cond.first);
            if (it == chains.end() || it->second != cond.second) {
                chains_ok = false;
                break;
            }
        }

        if (traits_ok && chains_ok) {
            valid_events.push_back(event_pair.first);
        }
    }

    if (valid_events.empty())
        return "";

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int index = std::rand() % valid_events.size();
    return valid_events[index];
}

// Done
std::wstring get_ending_description(const std::map<std::string, scu::Trait>& traits) {
    for (const auto& ending : scu::Data::endings) {
        const scu::Trait& trait = traits.at(ending.condition_trait_id);

        // Traits value should be queal or exids value
        if (ending.is_condition_max && trait.value >= ending.condition_trait_value) {
            return ending.description;
        }
        // Traits value should be queal or less then value
        else if (!ending.is_condition_max && trait.value <= ending.condition_trait_value) {
            return ending.description;
        }
    }
    return L"";
}

// Done
void clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

// Done
void show_text(const std::wstring& begining, const std::wstring& ending = L"") {
    const std::size_t kLines = kRows - 3;
    const std::size_t kCharsInLine = kColumns - 2;

    std::vector<std::wstring> lines(kRows - 3, std::wstring(kCharsInLine, L' '));
    const std::size_t kFiledSize = kLines * kCharsInLine;

    std::size_t i = 0;

    for (std::size_t j = 0; j < begining.size() && i < kFiledSize; ++j) {
        if (begining[j] == L'\n') {
            i = kCharsInLine * ((i + kCharsInLine) / kCharsInLine);
        }
        else {
            lines[i / kCharsInLine][i % kCharsInLine] = begining[j];
            ++i;
        }
    }

    std::size_t ending_n_count = std::count_if(ending.begin(), ending.end(), [](wchar_t c) {return c == L'\n'; });
    std::size_t ending_size = ending.size() - ending_n_count + (ending_n_count * kCharsInLine);

    i = (kFiledSize - ending_size) - ((kFiledSize - ending_size) % kCharsInLine);

    for (std::size_t j = 0; j < ending.size() && i < kFiledSize; ++j) {
        if (ending[j] == L'\n') {
            i = kCharsInLine * ((i + kCharsInLine) / kCharsInLine);
        }
        else {
            lines[i / kCharsInLine][i % kCharsInLine] = ending[j];
            ++i;
        }
    }

    std::wcout << L"+" << std::wstring(kColumns - 2, L'-') << L"+" << std::endl;
    for (int i = 0; i < kRows - 3; ++i) {
        std::wcout << L"|" << lines[i] << L"|" << std::endl;
    }
    std::wcout << L"+" << std::wstring(kColumns - 2, L'-') << L"+" << std::endl;
}

std::string process_event(const scu::Event& event) {
    std::wstring choices;
    std::map<std::uint32_t, std::string> choices_map;
    // Prepare choices
    for (const auto& choice : event.choices) {
        choices += std::to_wstring(choices_map.size() + 1) + L". " + choice.second.description + L"\n";
        choices_map[choices_map.size() + 1] = choice.first;
    }
    choices[choices.size() - 1] = L' ';

    // Clear console
    clear();

    // Show event and options
    show_text(event.description, choices);

    // Request input
    std::string temp;
    std::cin >> temp;

    // Check input correctness

    // Map input to choice id. Use choices_map.
    // 
    // Return choice id
    return "C0";
}

void main() {
    std::srand(std::time(nullptr)); //для выбора эвента, ограничение
    setlocale(LC_ALL, "Russian");
    _setmode(_fileno(stdout), _O_U16TEXT);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    kColumns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    kRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    // Show welcome message

    // Request and save character name

    // OPTIONAL: Request and save gender

    // Show initial info

    std::string name;
    const std::map<std::string, scu::Trait>& traits = scu::Data::traits;
    std::map<std::string, std::int32_t> chains;
    std::set<std::string> used_events;

    bool is_died = false;
    std::wstring ending_description;

    std::string previous_stage_id;

    int counter = 0;
    while (!is_died) {
        // Check stage according to age
        std::string stage_id = get_stage_according_to_age(traits.at(scu::Data::age_trait_id).value);

        // Clear used_events if stage changed and remember new stage
        if (stage_id != previous_stage_id) {
            previous_stage_id = stage_id;
            used_events.clear();
        }

        // Choose the event
        std::string new_event_id = get_new_event_id(
            used_events,
            chains,
            traits,
            scu::Data::stages,
            stage_id
        );

        if (new_event_id.empty()) {
            show_death_screen("Вы слишком долго колебались... и шанс был утерян.");
            return;
        }

        auto it = scu::Data::stages.at(stage_id).events.find(new_event_id);
        if (it != scu::Data::stages.at(stage_id).events.end()) {
            const scu::Event& event = it->second;

            // Process event
            std::string choice_id = process_event(event);

            // Apply effects and chains
            const scu::Choice& choice = event.choices.at(choice_id);
            for (const auto& effect : choice.effects) {
                // Apply effect
            }
            for (const auto& chain : choice.chains) {
                // Apply chain
            }

            // Apply age change

        }
        else {
            std::cerr << "Ошибка: event_id не найден" << std::endl;
        }

        // Check that character still alive. If have ending description game should end, otherwise game continue.
        ending_description = get_ending_description(traits); //G-01
        is_died = ending_description.empty();
    }

    // Prepare and show life results 
    clear();

    std::wstring life_description;

    for (auto desc : scu::Data::death_descriptions) {
        auto it = traits.find(desc.trait_id);
        if (it != traits.end() && it->second.value >= desc.min_value && it->second.value <= desc.max_value) {
            life_description += desc.description + L" ";
        }
    }

    // Wait for chains descriptions will be provided
    std::wstring chains_description;

    std::wstring result = ending_description + L"\n\n" + life_description + L"\n\n" + chains_description;

    show_text(result, L"Вы начинаете новую жизнь.\n");
}
