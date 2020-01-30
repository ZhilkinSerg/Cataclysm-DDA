#pragma once
#ifndef DAMAGE_TYPE_H
#define DAMAGE_TYPE_H

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>

#include "color.h"
#include "skill.h"
#include "type_id.h"
#include "translations.h"

struct damage_type {
    public:
        void load( const JsonObject &jo, const std::string &src );
        void finalize();
        void check() const;

    public:
        // Used by generic_factory
        damage_type_str_id id;
        bool was_loaded = false;

    public:

        translation name;
        translation description;
        nc_color color;
        bool is_typeless = false;
        bool is_internal = false;
        bool is_physical = false;
        skill_id skill;

        static size_t count();
};

namespace damage_types
{

void load( const JsonObject &jo, const std::string &src );
void finalize_all();
void check_consistency();
void reset();

const std::vector<damage_type> &get_all();
void set_damage_type_ids();

} // namespace damage_types

extern damage_type_id DT_NULL,
       DT_TRUE,
       DT_BIOLOGICAL,
       DT_BASH,
       DT_CUT,
       DT_ACID,
       DT_STAB,
       DT_HEAT,
       DT_COLD,
       DT_ELECTRIC
       ;

#endif // DAMAGE_TYPE_H
