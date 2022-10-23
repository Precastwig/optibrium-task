#include "DataTableUtils.hpp"
#include "DataTable.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_map>

bool DataTableUtils::set_union(const DataTable& dt1, const DataTable& dt2, DataTable& result) {
    result.clear();

    return copy_property_list(dt1, result) &&
            copy_property_list(dt2, result) &&
            copy_molecule_list(dt1, result) &&
            copy_molecule_list(dt2, result);
}

bool DataTableUtils::intersection(const DataTable& dt1, const DataTable& dt2, DataTable& result) {
    result.clear();

    std::vector<std::string> table_1_props = dt1.get_all_properties();
    std::vector<std::string> table_2_props = dt2.get_all_properties();
    std::sort(table_1_props.begin(), table_1_props.end());
    std::sort(table_2_props.begin(), table_2_props.end());

    if (table_1_props != table_2_props) {
        std::cout << "Properties between tables are incompatible\n";
        return false;
    }
    
    copy_property_list(dt1, result);

    std::vector<std::string> intersected_molecules;
    std::vector<std::string> table_1_mols = dt1.get_all_molecules();
    std::vector<std::string> table_2_mols = dt2.get_all_molecules();
    
    std::sort(table_1_mols.begin(), table_1_mols.end());
    std::sort(table_2_mols.begin(), table_2_mols.end());

    std::set_intersection(
        table_1_mols.begin(), table_1_mols.end(),
        table_2_mols.begin(), table_2_mols.end(),
        std::back_inserter(intersected_molecules)
    );

    for (const std::string& mol : intersected_molecules) {
        std::unordered_map<std::string, PropertyType> mol_props;
        if (!dt1.get_properties(mol, mol_props) && !dt2.get_properties(mol, mol_props)) {
            // For some reason the molecule cannot be found in either table
            return false;
        }
        result.add_molecule(mol, mol_props);
    }
    return true;
}

bool DataTableUtils::copy_property_list(const DataTable& from, DataTable& to) {
    std::vector<std::string> prop_names = from.get_all_properties();
    for (const std::string& prop_name : prop_names) {
        PropertyType prop_value;
        if (!from.get_property_default_value(prop_name, prop_value)) {
            return false;
        }

        if (!to.add_property(prop_name, prop_value)) {
            // The property already exists
            PropertyType default_val;
            if (!to.get_property_default_value(prop_name, default_val) || 
                prop_value.index() != default_val.index()) {
                // The property has already been added to the result
                // But with a different type!
                std::cout << "The two tables contain incompatible property lists\n";
                // Could output what type was the problem here
                return false;
            }
        }
    }
    return true;
}

bool DataTableUtils::copy_molecule_list(const DataTable& from, DataTable& to) {
    std::vector<std::string> mol_names = from.get_all_molecules();
    for (const std::string& mol_name : mol_names) {
        std::unordered_map<std::string, PropertyType> combined_map;
        if (!from.get_properties(mol_name, combined_map)) {
            return false;
        }
        
        std::vector<std::string> expected_properties = to.get_all_properties();
        for (const std::string& prop : expected_properties) {
            if (combined_map.contains(prop)) {
                continue;
            }
            PropertyType default_val;
            if (!to.get_property_default_value(prop, default_val)) {
                return false;
            }
            combined_map[prop] = default_val;
        }
        
        to.add_molecule(mol_name, combined_map);
    }
    return true;
}

