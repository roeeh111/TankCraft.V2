
//------------------------------------------------------------------------------
// This code was generated by a tool.
//
//   Tool : Bond Compiler 0.12.1.0
//   Input filename:  example.bond
//   Output filename: example_reflection.h
//
// Changes to this file may cause incorrect behavior and will be lost when
// the code is regenerated.
// <auto-generated />
//------------------------------------------------------------------------------

#pragma once

#include "example_types.h"
#include <bond/core/reflection.h>

namespace example
{
    //
    // Record
    //
    struct Record::Schema
    {
        typedef ::bond::no_base base;

        static const ::bond::Metadata metadata;
        
        private: static const ::bond::Metadata s_name_metadata;
        private: static const ::bond::Metadata s_items_metadata;

        public: struct var
        {
            // name
            typedef struct name_type : ::bond::reflection::FieldTemplate<
                0,
                ::bond::reflection::optional_field_modifier,
                Record,
                std::string,
                &Record::name,
                &s_name_metadata
            > {} name;
        
            // items
            typedef struct items_type : ::bond::reflection::FieldTemplate<
                1,
                ::bond::reflection::optional_field_modifier,
                Record,
                std::vector<double>,
                &Record::items,
                &s_items_metadata
            > {} items;
        };

        private: typedef boost::mpl::list<> fields0;
        private: typedef boost::mpl::push_front<fields0, var::items>::type fields1;
        private: typedef boost::mpl::push_front<fields1, var::name>::type fields2;

        public: typedef fields2::type fields;
        
        
        static ::bond::Metadata GetMetadata()
        {
            return ::bond::reflection::MetadataInit("Record", "example.Record",
                ::bond::reflection::Attributes()
            );
        }
    };
    

    
} // namespace example
