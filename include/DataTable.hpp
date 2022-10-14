#ifndef H_DATATABLE
#define H_DATATABLE
// #pragma once not officially part of standard, has can have consquences if using two files with the same base name!
// (obviously not an issue here)

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

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


struct Property {
    typedef std::variant<double, int, std::string> Type;
    enum {
        DOUBLE,
        INT,
        STRING
    };

    // Basic constructor for ease of creation
    Property(std::string name, Type value) : m_property_name(name), m_value(value) {};

    // Member variables 
    std::string m_property_name;
    Type m_value;

    // Comparison operator
    // We can implicitly construct using non member function
    friend bool operator==(const Property& lhs, const Property& rhs);
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
    bool add_molecule(const std::string& new_molecule);
    bool add_molecule(const std::string& new_molecule, std::vector<Property> new_properties);

    /*
        Adds the given property to the molecule,
        If the provided property already exists on the molecule, the value will be updated
        Returns true if the addition was successful, false if not 
        (e.g. if the provided property already exists, but with a different type)
    */
    bool add_property(const std::string& molecule, Property new_property);

    // Data getters

    std::vector<std::string> get_molecules() const;
    std::vector<Property> get_properties(const std::string& molecule) const;
    Property::Type get_property_value(const std::string& molecule, const std::string& property) const; 

private:
    void update_cache(const std::vector<Property>& properties);
    bool is_property_valid(const Property& p);

    std::vector<std::string> m_molecules;
    std::unordered_map<std::string, std::vector<Property>> m_property_map;

    // A list of all property types in the table, this cache is used to quickly verify any properties of new molecules added to the table (or new properties).
    // Additionally, it could be used to set a default value for each property, if unspecified (e.g. when merging two tables)
    std::vector<Property> m_cached_property_types;
};

#endif
