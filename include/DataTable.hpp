#ifndef H_DATATABLE
#define H_DATATABLE
// #pragma once not officially part of standard, has can have consquences if using two files with the same base name!
// (obviously not an issue here)

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <map>

// Molecule     Solubility Molecular Weight
// Paracetamol  4.97       151
// Caffeine     5.05       194
// Indomethacin 0.4        358
// Trimethoprim 3.14       290

// Simple data table class that can represent collections of molecules with associated properties. 
// The properties can be numeric or text types.

// Write C++ code that can combine two data tables. 
// You may wish to provide the ability to take set unions, differences and intersections. 
// You may also wish to think about how to combine tables that have different property columns.

typedef std::variant<double, int, std::string> PropertyType;
enum PropertyTypeEnum{
    DOUBLE,
    INT,
    STRING
};

class DataTable {
public:
    /// Constructor
    DataTable();

    /// Data setters

    /* 
        Adds the given molecule to the database, 
        if any properties arent provided they will be auto filled with default values
        Returns true if addition was successful, false if not 
    */
    bool add_molecule(const std::string& new_molecule, const std::unordered_map<std::string, PropertyType>& new_properties);

    /*
        Adds the given property to the table, uses whatever value is given as the default value for that property.
        If the provided property already exists, the value type and default value will be updated
        Returns true if the addition was successful, false if not 
    */
    bool add_property(const std::string& new_property_name, const PropertyType& default_value);

    bool set_property(const std::string& molecule_name, const std::string& property_name, const PropertyType& new_value);

    void clear();

    // Data getters

    std::vector<std::string> get_all_molecules() const;
    std::vector<std::string> get_all_properties() const;

    bool get_properties(const std::string& molecule, std::unordered_map<std::string, PropertyType>& properties) const;  
    bool get_property(const std::string& molecule_name, const std::string& property_name, PropertyType& property) const; 
    bool get_property_default_value(const std::string& property_name, PropertyType& default_value) const;

private:

    std::unordered_map<std::string, std::unordered_map<std::string, PropertyType>> m_property_map;

    // A map of all property types and default values in the table
    std::unordered_map<std::string, PropertyType> m_property_cache;
};

#endif
