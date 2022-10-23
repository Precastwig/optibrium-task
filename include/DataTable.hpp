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
        The provided map should contain all properties in the table,
        Returns true if addition was successful, false if not 
    */
    bool add_molecule(const std::string& new_molecule, const std::unordered_map<std::string, PropertyType>& new_properties);

    /*
        Removes the given molecule from the table,
        If the provided molecule doesn't exist, returns false.
        Else true.
    */
    bool remove_molecule(const std::string& molecule_name);

    /*
        Adds the given property to the table, uses whatever value is given as the default value for that property.
        If the provided property already exists, the value type and default value will be updated
        Returns true if the addition was successful, false if not 
    */
    bool add_property(const std::string& new_property_name, const PropertyType& default_value);

    /*
        Sets the given property on the given molecule to the given value,
        The type of the value must match that of the property set
        Returns false if the property could not be set
    */
    bool set_property(const std::string& molecule_name, const std::string& property_name, const PropertyType& new_value);

    /*
        Removes the given property from the table
        returns true if the property has been removed,
        false if it could not be removed (e.g. it doesn't exist in the table)
    */
    bool remove_property(const std::string& property_name);

    // Empties the table completely
    void clear();

    // Data getters

    // Retrieves all molecule names from the table
    std::vector<std::string> get_all_molecules() const;
    // Retrieves all property names from the table
    std::vector<std::string> get_all_properties() const;

    // Retrieves all properties for a specific molecule, returns false if the property doesn't exist
    bool get_properties(const std::string& molecule, std::unordered_map<std::string, PropertyType>& properties) const;  
    // Retrieves a specific property from a specific molecule in the table, returns false if the property or molecule dont exist in the table
    bool get_property(const std::string& molecule_name, const std::string& property_name, PropertyType& property) const; 
    // Retrieves the default value for the given property in the table, returns false if the given property doesnt exist
    bool get_property_default_value(const std::string& property_name, PropertyType& default_value) const;

private:
    // A map of all molecules to property maps
    // This is somewhat space inefficient, as it stores all the property strings for each
    // molecule it contains
    // Could be improved by making the property map a vector and saving the index of each property
    // in the property cache.
    std::unordered_map<std::string, std::unordered_map<std::string, PropertyType>> m_property_map;

    // A map of all property types and default values in the table
    std::unordered_map<std::string, PropertyType> m_property_cache;
};

#endif
