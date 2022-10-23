#include "include/DataTable.hpp"
#include "include/DataTableUtils.hpp"
#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>

struct make_string_functor {
  std::string operator()(const std::string &x) const { return x; }
  std::string operator()(int x) const { return std::to_string(x); }
  std::string operator()(double x) const { return std::to_string(x); }
};

bool compare(std::vector<std::string>& v1, std::vector<std::string>& v2)
{
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());
    return v1 == v2;
}

void print_table(const DataTable& tb) {
    for (std::string m : tb.get_all_molecules()) {
        std::cout << m << " ";
        for (const std::string& p : tb.get_all_properties()) {
            PropertyType pt;
            if (tb.get_property(m, p, pt)) {
                std::cout << p << ": " << std::visit(make_string_functor(), pt);
            }
        }
        std::cout << std::endl;
    }
}

void print_list(std::vector<std::string> list) {
    for (std::string s : list) {
        std::cout << s << "\n";
    }
}

int main(int argc, char *argv[]) {
    // Add property tests
{   
    /// Check properties get added
    DataTable tb;
    assert(tb.add_property("Mass", 0.0));
    std::vector<std::string> expected_list = {"Mass"};
    std::vector<std::string> prop_list = tb.get_all_properties();
    assert(compare(expected_list, prop_list));

    assert(tb.add_property("Sprouts", 0));
    expected_list.push_back("Sprouts");
    prop_list = tb.get_all_properties();
    assert(compare(expected_list, prop_list));

    assert(tb.add_property("Description", "It's a potato"));
    expected_list.push_back("Description");
    prop_list = tb.get_all_properties();
    assert(compare(expected_list, prop_list));

    /// Check molecule can only be added if it has expected properties
    // No properties
    assert(!tb.add_molecule("Potato", {}));
    assert(tb.get_all_molecules().empty());

    // Some correct properties
    assert(!tb.add_molecule("Potato", {{"Mass", 51.2}, {"Sprouts", 1}, {"Colour", "Red"}}));
    assert(tb.get_all_molecules().empty());

    // All correct properties
    assert(tb.add_molecule("Potato", {{"Description", "Looks good!"}, {"Mass", 30.0}, {"Sprouts", 2}})); 
    assert(tb.get_all_molecules().size() == 1);

    /// Check that new properties get back-propegated to existing molecules with their default value
    assert(tb.add_property("Colour", "Unknown"));
    PropertyType p;
    assert(tb.get_property("Potato", "Colour", p));
    assert(p == PropertyType("Unknown"));

    std::cout << "\033[32mAll add properties tests passed\033[0m\n";
}

    // Set Union
{
    DataTable tb1;
    DataTable tb2;

    assert(tb1.add_property("Red", false));
    assert(tb1.add_property("Green", false));

    assert(tb1.add_molecule("Potato", {{"Red", false}, {"Green", true}}));
    assert(tb1.add_molecule("Onion", {{"Red", true}, {"Green", false}}));

    assert(tb2.add_property("Mass", 0.0));
    assert(tb2.add_property("Blue", false));

    assert(tb2.add_molecule("Blueberry", {{"Mass", 4.0}, {"Blue", true}}));
    assert(tb2.add_molecule("Strawberry", {{"Mass", 5.0}, {"Blue", false}}));

    DataTable result;
    assert(DataTableUtils::set_union(tb1, tb2, result));

    auto molecules = result.get_all_molecules();
    std::vector<std::string> expected_molecules = {
        "Potato",
        "Onion",
        "Blueberry",
        "Strawberry"
    };
    assert(compare(molecules, expected_molecules));

    auto properties = result.get_all_properties();
    std::vector<std::string> expected_properties = {
        "Mass", "Red", "Blue", "Green"
    };
    assert(compare(properties, expected_properties));

    std::cout << "\033[32mSet union tests passed\033[0m\n";
}

{
    DataTable tb1;
    DataTable tb2;

    assert(tb1.add_property("Mass", 0.0));
    assert(tb1.add_property("Description", ""));
    assert(tb2.add_property("Mass", 0.0));
    assert(tb2.add_property("Description", ""));

    assert(tb1.add_molecule("Potato", {{"Mass", 5.0}, {"Description", "Gross looking"}}));
    assert(tb1.add_molecule("Onion", {{"Mass", 3.0}, {"Description", "Verdant"}}));
    assert(tb1.add_molecule("Carrot", {{"Mass", 1.0}, {"Description", "Fine"}}));

    assert(tb2.add_molecule("Potato", {{"Mass", 4.1}, {"Description", "It's ok"}}));
    assert(tb2.add_molecule("Strawberry", {{"Mass", 1.0}, {"Description", ""}}));

    DataTable result;
    assert(DataTableUtils::intersection(tb1, tb2, result));

    assert(result.get_all_molecules()[0] == "Potato");
    PropertyType mass;
    assert(result.get_property("Potato", "Mass", mass));
    assert(mass == PropertyType(5.0));
    std::cout << "\033[32mSet intersection tests passed\033[0m\n";
}
}
