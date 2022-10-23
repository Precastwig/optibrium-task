#ifndef H_DATATABLEUTILS
#define H_DATATABLEUTILS

#include "DataTable.hpp"

class DataTableUtils {
public:
    // Merge all properties and molecules in both given tables
    // Any properties that do not exist on a molecule will be given it's default value
    // Returns false if properties in both data tables are incompatible,
    // If any molecules are duplicated between both data tables then property values are taken
    // from the first data table
    static bool set_union(const DataTable& dt1, const DataTable& dt2, DataTable& result);

    // Create a table of all molecules that belong to both tables (and have identical property lists!)
    // Returns true if successful, false if property lists differ
    // This could be easily changed to not accept molecules with the same name, but different properties. 
    static bool intersection(const DataTable& dt1, const DataTable& dt2, DataTable& result);
private:
    static bool copy_property_list(const DataTable& from, DataTable& to);
    static bool copy_molecule_list(const DataTable& from, DataTable& to);
};

#endif
