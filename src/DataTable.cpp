#include "../include/DataTable.hpp"
#include <iostream>

bool operator==(const Property& lhs, const Property& rhs) {
    return lhs.m_property_name == rhs.m_property_name &&
            lhs.m_value == rhs.m_value;
}

DataTable::DataTable() {

}

std::vector<Property> DataTable::get_properties(const std::string& molecule) const {
    auto it = m_property_map.find(molecule);
    if (it != m_property_map.end())
        return it->second;
    
    return {};
}

std::vector<std::string> DataTable::get_molecules() const {
    return m_molecules;
}

Property::Type DataTable::get_property_value(const std::string& molecule, const std::string& property) const {

    for (const Property& p : get_properties(molecule)) {
        if (p.m_property_name == property) {
            return p.m_value;
        }
    }

    // Return not found?
    return Property::Type();
}

bool DataTable::add_molecule(const std::string& new_molecule) {
    // Slightly less time efficient, but readability seems more important here
    return add_molecule(new_molecule, m_cached_property_types);
}

bool DataTable::add_molecule(const std::string& new_molecule, std::vector<Property> new_properties) {
    if (std::find(m_molecules.begin(), m_molecules.end(), new_molecule) != m_molecules.end()) {
        // We dont want to double-add a molecule
        return false;
    }

    // We need to sanity check the properties list
    for (const Property& p : new_properties) {
        // Find any invalid properties (according to the cache)
        if (!is_property_valid(p)) {
            // The molecule being added contains a property that already
            // exists in the table, but with a different type
            std::cout << "The property " << p.m_property_name << " is invalid\n";
            return false;
        }
    }

    // Insert the molecule
    m_molecules.push_back(new_molecule);
    m_property_map[new_molecule] = new_properties;

    update_cache(new_properties);
    return true;
}

bool DataTable::add_property(const std::string& molecule, Property new_property) {
    if (!is_property_valid(new_property)) {
        return false;
    } 

    auto find_it = std::find_if(
        m_property_map[molecule].begin(), 
        m_property_map[molecule].end(), 
        [](const Property& p) {
            return true;
        });
    if (find_it == m_property_map[molecule].end()) {
        // The property is unique to the table
        m_property_map[molecule].push_back(new_property);
        update_cache({new_property});
    } else {
        // The property already exists, update its value
        find_it->m_value = new_property.m_value;
    }
    return true;
}

bool DataTable::is_property_valid(const Property& p) {
    // Check if the given property has the same name as an existing property, but a different type
    auto find_it = 
        std::find_if(
            m_cached_property_types.begin(), 
            m_cached_property_types.end(), 
            [&p](const Property& cached_p ){
                return p.m_property_name == cached_p.m_property_name &&
                        p.m_value.index() != cached_p.m_value.index();
            });
    return find_it == m_cached_property_types.end();
}

void DataTable::update_cache(const std::vector<Property>& properties) {
    // Find any unique properties of this molecule and add it to the cache
    for (const Property& p : properties) {
        auto find_it =
            std::find_if(
                m_cached_property_types.begin(), 
                m_cached_property_types.end(), 
                [&p](const Property& cached_p ){
                    return p.m_property_name == cached_p.m_property_name;
                });
        if (find_it == m_cached_property_types.end()) {
            // Unique property type
            m_cached_property_types.push_back(p);
        }
    }
}