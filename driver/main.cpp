#include<iostream>
#include<fstream>
#include <string>
#include "../external/json.hpp"
#include "../parsers/eventsParser.hpp"

using json = nlohmann::json;


int main(){

    std::string filename = "../test_generation/generated_events.json";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return 1;
    }

    try {
    json j;
    file >> j;
    // EventParser parser = EventParser();

    // Example: Accessing data
    for (const auto& obj : j) {
        if (!obj.is_object()) continue;
        Event e = EventParser().parse(obj);
        std::cout<<e.type<<std::endl;

    }

    } catch (const json::parse_error& e) {
        std::cerr << "Parse error at byte " << e.byte << ": " << e.what() << "\n";
        return 1;
    } catch (const json::type_error& e) {
        std::cerr << "Type error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}