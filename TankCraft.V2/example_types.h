
//------------------------------------------------------------------------------
// This code was generated by a tool.
//
//   Tool : Bond Compiler 0.12.1.0
//   Input filename:  example.bond
//   Output filename: example_types.h
//
// Changes to this file may cause incorrect behavior and will be lost when
// the code is regenerated.
// <auto-generated />
//------------------------------------------------------------------------------

#pragma once

#include <bond/core/bond_version.h>

#if BOND_VERSION < 0x0902
#error This file was generated by a newer version of the Bond compiler and is incompatible with your version of the Bond library.
#endif

#if BOND_MIN_CODEGEN_VERSION > 0x0c10
#error This file was generated by an older version of the Bond compiler and is incompatible with your version of the Bond library.
#endif

#include <bond/core/config.h>
#include <bond/core/containers.h>



namespace example
{
    
    struct Record
    {
        std::string name;
        std::vector<double> items;
        
        Record()
        {
        }

        
        // Compiler generated copy ctor OK
        Record(const Record&) = default;
        
        Record(Record&&) = default;
        
        
        // Compiler generated operator= OK
        Record& operator=(const Record&) = default;
        Record& operator=(Record&&) = default;

        bool operator==(const Record& other) const
        {
            return true
                && (name == other.name)
                && (items == other.items);
        }

        bool operator!=(const Record& other) const
        {
            return !(*this == other);
        }

        void swap(Record& other)
        {
            using std::swap;
            swap(name, other.name);
            swap(items, other.items);
        }

        struct Schema;

    protected:
        void InitMetadata(const char*, const char*)
        {
        }
    };

    inline void swap(::example::Record& left, ::example::Record& right)
    {
        left.swap(right);
    }
} // namespace example
