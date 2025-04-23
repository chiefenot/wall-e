#include <iostream>
#include <map>
#include <set>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <windows.h>


#include "data.hpp" // предполагаем, что здесь описаны scu::Data, scu::Stage, Event, Choice и Trait
using namespace scu;

// Функция для вывода трейтов
void print_trait(const Trait& trait) {
	std::wcout << L"Трейт: " << trait.name << L" (" << trait.description << L") Значение: " << trait.value << std::endl;
}

// Функция для вывода всех трейтов
void print_traits(const std::map<std::string, Trait>& traits) {
	for (const auto& trait : traits) {
		print_trait(trait.second);  // Печать каждого трейта
	}
}

// Функция для вывода событий
void print_event(const Event& event) {
	std::wcout << L"Событие: " << event.description << std::endl;

	for (const auto& [choice_key, choice] : event.choices) {
		std::wcout << L"  Выбор: " << choice.description << std::endl;
	}
}

// Функция для вывода стадии
void print_stage(const Stage& stage) {
	std::wcout << L"Стадия: " << stage.name << L" (" << stage.description << L")\n";
	for (const auto& event : stage.events) {  // Теперь правильное обращение
		print_event(event.second); // Печать события
	}
}

// Функция для вывода всех данных
void print_all_data() {
	print_traits(Data::traits); // Печать всех трейтов
	//  вывод стадий, если они есть
	for (const auto& stage : Data::stages) {
		print_stage(stage.second);  // Печать каждой стадии
	}
}


std::wstring check_ending(const std::map<std::string, scu::Trait>& traits) {
	for (const auto& ending : scu::Data::endings) {
		const scu::Trait& trait = traits.at(ending.condition_trait_id);

		if (ending.is_condition_max && trait.value >= ending.condition_trait_value)
			return ending.description;
		if (!ending.is_condition_max && trait.value <= ending.condition_trait_value)
			return ending.description;
	}
	return L"";
}





	void game_loop(scu::Stage * initial_stage) {
		scu::Stage* current_stage = initial_stage;
		SelectedEvents selected_events;
		ActiveChains active_chains;

		// Цикл игры, пока не достигнут финал
		while (true) {
			// 1. Выводим текущую стадию
			std::wcout << L"Текущая стадия: " << current_stage->name << std::endl;

			bool event_found = false;  // Было ли найдено доступное событие для выбора
			bool all_events_selected = true;  // Все ли события выбраны

			// 2. Проверяем события текущей стадии
			for (auto& event_pair : current_stage->events) {
				std::string event_id = event_pair.first;
				scu::Event& event = event_pair.second;

				// Пропускаем, если событие уже выбрано
				if (selected_events.is_event_selected(event_id)) {
					continue;
				}

				// 3. Проверяем условия цепочек
				bool can_activate_event = true;

				// Проверяем условия цепочек
				for (const auto& chain : event.conditions_chains) {
					if (!active_chains.is_chain_active(chain.first)) {
						can_activate_event = false;
						break;
					}
				}

				// Если событие доступно для активации
				if (can_activate_event) {
					event_found = true;
					all_events_selected = false;

					// 4. Выводим описание события
					std::wcout << L"Событие: " << event.description << std::endl;

					// Выводим все доступные выборы
					for (auto& choice_pair : event.choices) {
						std::wcout << L"  Выбор: " << choice_pair.second.description << std::endl;
					}

					// Запрашиваем выбор игрока
					std::wstring choice_input;
					std::wcin >> choice_input;

					// 5. Запоминаем выбранное событие
					selected_events.add_event(event_id);

					// Запоминаем активные цепочки из выбранного варианта
					for (auto& chain_pair : event.choices[choice_input].chains) {
						active_chains.add_chain(chain_pair.first);
					}
				}
			}

			// 6. Переход к следующей стадии, если все события выбраны или нет доступных событий
			//if (!event_found || all_events_selected) {
			//	auto next_stage_iter = Data::stages.find(current_stage->name);
			//	if (next_stage_iter != Data::stages.end() && std::next(next_stage_iter) != Data::stages.end()) {
			//		current_stage = &std::next(next_stage_iter)->second;  // Переход к следующей стадии
			//		std::wcout << L"Вы переходите на следующий уровень: " << current_stage->name << std::endl;
			//	}
			//	else {
			//		std::wcout << L"Вы достигли финала!" << std::endl;
			//		break;  // Игра завершена
			//	}
			//}

			// 7. Ожидаем, если необходимо, чтобы игрок продолжил
			std::wcout << L"Нажмите Enter для продолжения..." << std::endl;
			std::cin.ignore();
			std::cin.get();
		}
	}

	int main() {
		SetConsoleOutputCP(CP_UTF8);
		setlocale(LC_ALL, "Russian");
		_setmode(_fileno(stdout), _O_U16TEXT);

		std::map<std::string, scu::Trait> traits = scu::Data::traits;
		std::wstring ending_description;




		//std::string stage_id = "S0";  // начальный этап
		//std::string event_id = "E0";  // первое событие (для теста)

		//	print_all_data();
		// Инициализация stage_id и event_id
		const auto& stage = scu::Data::stages.begin()->second;
		std::wcout << L"Stage name: " << stage.name << std::endl;

		//std::string stage_id = scu::Data::stages.begin()->first;
		//std::cout << "Stage ID: " << stage_id << std::endl;

		// Перебираем все стадии
		//for (const auto& [id, stage] : scu::Data::stages) {
		//	std::wcout << L"Stage ID: " << std::wstring(id.begin(), id.end()) << std::endl;
		//	std::wcout << L"Name: " << stage.name << std::endl;
		//	std::wcout << L"Description: " << stage.description << std::endl;
		//	std::wcout << L"Age range: " << stage.min_age << L"–" << stage.max_age << std::endl;
		//	std::wcout << L"-------------------------" << std::endl;
		//}

		// Выводим значения
		//std::wcout << L"stage_id: " << std::wstring(stage_id.begin(), stage_id.end()) << std::endl;

		//std::wcout << L"event_id: " << event_id.c_str() << std::endl;

		/*return 0;*/

		//const scu::Stage& stage = scu::Data::stages.at(stage_id);
		//const scu::Event& event = stage.events.at(event_id);

		/*print_stage(stage);*/

		// цикл до достижения одного из условий окончания
		//(ending_description = check_ending(traits)).empty()
		//while (false) {
		//	system("cls");

		//	std::wcout << L"=== Этап: " << stage_id.c_str() << L" | Событие: " << event_id.c_str() << L" ===\n";
		//	print_traits(traits);
		//	print_event(event);

		//	std::wcout << L"\n(Заглушка: конец цикла, так как выбор не реализован)\n";
		//	break;  // сейчас просто один шаг
		//}

		//if (!ending_description.empty()) {
		//	std::wcout << L"\nКонец игры: " << ending_description << std::endl;
		//}
		//return 0;
		// Инициализируем игру с начальной стадии
		scu::Stage* initial_stage = &scu::Data::stages["S0"];
		game_loop(initial_stage);
		return 0;
	}

