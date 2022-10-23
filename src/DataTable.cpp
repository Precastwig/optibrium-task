#include "../include/DataTable.hpp"
#include <iostream>
#include <variant>

DataTable::DataTable() {

}

std::vector<std::string> DataTable::get_all_properties() const {
    std::vector<std::string> property_strings;
    for (const auto& prop : m_property_cache) {
        property_strings.push_back(prop.first);
    }
    return property_strings;
}

std::vector<std::string> DataTable::get_all_molecules() const {
    std::vector<std::string> molecule_strings;
    for (const auto& prop : m_property_map) {
        molecule_strings.push_back(prop.first);
    }
    return molecule_strings;
}

bool DataTable::get_properties(const std::string& molecule, std::unordered_map<std::string, PropertyType>& properties) const {
    if (!m_property_map.contains(molecule)) {
        return false;
    }
    properties = m_property_map.at(molecule);
    return true;
}  

bool DataTable::get_property(const std::string& molecule_name, const std::string& property_name, PropertyType& property) const {
    if (!m_property_map.contains(molecule_name) ||
        !m_property_cache.contains(property_name)) {
        return false;
    }
    property = m_property_map.at(molecule_name).at(property_name);
    return true;
}

bool DataTable::get_property_default_value(const std::string& property_name, PropertyType& default_value) const {    
    if (!m_property_cache.contains(property_name)) {
        return false;
    }
    default_value = m_property_cache.at(property_name);
    return true;
}

bool DataTable::add_molecule(const std::string& new_molecule, const std::unordered_map<std::string, PropertyType>& new_properties) {
    if (m_property_map.find(new_molecule) != m_property_map.end()) {
        // We dont want to double-add a molecule
        std::cout << "The molecule already exists in the table\n";
        return false;
    }
    // We need to sanity check the properties list
    if (new_properties.size() != m_property_cache.size()) {
        std::cout << "The property list is the incorrect size\n";
        return false;
    }
    for (const auto& prop : new_properties) {
        if (!m_property_cache.contains(prop.first)) {
            std::cout << "The property " << prop.first << " doesn't exist in the table\n";
            return false;
        }

        // Check that the type of the incoming properties is correct
        if (m_property_cache[prop.first].index() != prop.second.index()) {
            // The molecule being added contains a property that already
            // exists in the table, but with a different type
            std::cout << "The property " << prop.first << " is invalid\n";
            return false;
        }
    }

    // Insert the molecule
    m_property_map[new_molecule] = new_properties;

    return true;
}

bool DataTable::add_property(const std::string& new_property_name, const PropertyType& default_value) {
    if (m_property_cache.contains(new_property_name)) {
        std::cout << "Property already exists in the table\n";
        return false;
    }
    m_property_cache[new_property_name] = default_value;
    // The property is new to the table
    // Add the new property to the molecules in the table
    for (auto& molecule : m_property_map) {
        molecule.second[new_property_name] = default_value;
    }
    return true;
}

bool DataTable::set_property(const std::string& molecule_name, const std::string& property_name, const PropertyType& new_value) {
    if (!m_property_map.contains(molecule_name) ||
        !m_property_cache.contains(property_name)) {
        std::cout << "Molecule or Property doesn't exist in the table\n";
        return false;
    }
    if (m_property_cache[property_name].index() != new_value.index()) {
        std::cout << "Given property has differing type in table\n";
        return false;
    }

    m_property_map[molecule_name][property_name] = new_value;
    return true;
}

void DataTable::clear() {
    m_property_map.clear();
    m_property_cache.clear();
}
