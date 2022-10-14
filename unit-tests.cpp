#include "include/DataTable.hpp"
#include <iostream>
#include <string>

struct make_string_functor {
  std::string operator()(const std::string &x) const { return x; }
  std::string operator()(int x) const { return std::to_string(x); }
  std::string operator()(double x) const { return std::to_string(x); }
};

void print_table(const DataTable& tb) {
    for (std::string m : tb.get_molecules()) {
        std::cout << m << " ";
        for (const Property& p : tb.get_properties(m)) {
            std::cout << p.m_property_name << ": " << std::visit(make_string_functor(), p.m_value);
        }
        std::cout << std::endl;
    }
}

int main(int argc, char *argv[]) {
    DataTable tb1;
    tb1.add_molecule("Potato", {Property("Mass", 50)});
    print_table(tb1);
    tb1.add_property("Potato", Property("Mass", 60));
    print_table(tb1);
}