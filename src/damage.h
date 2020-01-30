#pragma once
#ifndef DAMAGE_H
#define DAMAGE_H

#include <array>
#include <map>
#include <vector>
#include <string>

#include "damage_type.h"
#include "type_id.h"

class item;
class monster;
class JsonObject;
class JsonArray;
class JsonIn;

enum body_part : int;

struct damage_unit {
    damage_type_id type;
    float amount;
    float res_pen;
    float res_mult;
    float damage_multiplier;

    damage_unit( damage_type_id dt, float a, float rp = 0.0f, float rm = 1.0f, float mul = 1.0f ) :
        type( dt ), amount( a ), res_pen( rp ), res_mult( rm ), damage_multiplier( mul ) { }

    bool operator==( const damage_unit &other ) const;
};

// a single atomic unit of damage from an attack. Can include multiple types
// of damage at different armor mitigation/penetration values
struct damage_instance {
    std::vector<damage_unit> damage_units;
    damage_instance();
    static damage_instance physical( float bash, float cut, float stab, float arpen = 0.0f );
    damage_instance( damage_type_id dt, float a, float rp = 0.0f, float rm = 1.0f, float mul = 1.0f );
    void mult_damage( double multiplier, bool pre_armor = false );
    float type_damage( damage_type_id dt ) const;
    float total_damage() const;
    void clear();
    bool empty() const;

    std::vector<damage_unit>::iterator begin();
    std::vector<damage_unit>::const_iterator begin() const;
    std::vector<damage_unit>::iterator end();
    std::vector<damage_unit>::const_iterator end() const;

    bool operator==( const damage_instance &other ) const;

    /**
     * Adds damage to the instance.
     * If the damage type already exists in the instance, the old and new instance are normalized.
     * The normalization means that the effective damage can actually decrease (depending on target's armor).
     */
    /*@{*/
    void add_damage( damage_type_id dt, float a, float rp = 0.0f, float rm = 1.0f, float mul = 1.0f );
    void add( const damage_instance &added_di );
    void add( const damage_unit &added_du );
    /*@}*/

    void deserialize( JsonIn & );
};

struct dealt_damage_instance {
    std::map<damage_type_id, int> dealt_dams;
    body_part bp_hit;

    dealt_damage_instance();
    void set_damage( damage_type_id dt, int amount );
    int type_damage( damage_type_id dt ) const;
    int total_damage() const;
};

struct resistances {
    std::map<damage_type_id, float> resist_vals;

    resistances();

    // If to_self is true, we want armor's own resistance, not one it provides to wearer
    resistances( const item &armor, bool to_self = false );
    resistances( monster &monster );
    void set_resist( damage_type_id dt, float amount );
    float type_resist( damage_type_id dt ) const;

    float get_effective_resist( const damage_unit &du ) const;

    resistances &operator+=( const resistances &other );
};

damage_instance load_damage_instance( const JsonObject &jo );
damage_instance load_damage_instance( const JsonArray &jarr );

resistances load_resistances_instance( const JsonObject &jo );

// Returns damage or resistance data
// Handles some shorthands
std::map<damage_type_id, float> load_damage_array( const JsonObject &jo );

#endif
