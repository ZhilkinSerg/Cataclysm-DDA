#include "damage.h"

#include <cstddef>
#include <algorithm>
#include <map>
#include <numeric>
#include <utility>

#include "debug.h"
#include "item.h"
#include "json.h"
#include "monster.h"
#include "mtype.h"
#include "translations.h"
#include "cata_utility.h"

namespace io
{
template<>
std::string enum_to_string<damage_type>( damage_type data )
{
    switch( data ) {
        // *INDENT-OFF*
        case damage_type::DT_NULL: return "null";
        case damage_type::DT_TRUE: return "true";
        case damage_type::DT_BIOLOGICAL: return "biological";
        case damage_type::DT_BASH: return "bash";
        case damage_type::DT_CUT: return "cut";
        case damage_type::DT_STAB: return "stab";
        case damage_type::DT_ACID: return "acid";
        case damage_type::DT_HEAT: return "heat";
        case damage_type::DT_COLD: return "cold";
        case damage_type::DT_ELECTRIC: return "electric";
        // *INDENT-ON*
        case damage_type::NUM_DT:
            break;
    }
    debugmsg( "Invalid damage type value '%d'.", data );
    return "invalid";
}

} // namespace io

bool damage_unit::operator==( const damage_unit &other ) const
{
    return type == other.type &&
           amount == other.amount &&
           res_pen == other.res_pen &&
           res_mult == other.res_mult &&
           damage_multiplier == other.damage_multiplier;
}

damage_instance::damage_instance() = default;
damage_instance damage_instance::physical( float bash, float cut, float stab, float arpen )
{
    damage_instance d;
    d.add_damage( DT_BASH, bash, arpen );
    d.add_damage( DT_CUT, cut, arpen );
    d.add_damage( DT_STAB, stab, arpen );
    return d;
}
damage_instance::damage_instance( damage_type dt, float a, float rp, float rm, float mul )
{
    add_damage( dt, a, rp, rm, mul );
}

void damage_instance::add_damage( damage_type dt, float a, float rp, float rm, float mul )
{
    damage_unit du( dt, a, rp, rm, mul );
    add( du );
}

void damage_instance::mult_damage( double multiplier, bool pre_armor )
{
    if( multiplier <= 0.0 ) {
        clear();
    }

    if( pre_armor ) {
        for( auto &elem : damage_units ) {
            elem.amount *= multiplier;
        }
    } else {
        for( auto &elem : damage_units ) {
            elem.damage_multiplier *= multiplier;
        }
    }
}
float damage_instance::type_damage( damage_type dt ) const
{
    float ret = 0;
    for( const auto &elem : damage_units ) {
        if( elem.type == dt ) {
            ret += elem.amount * elem.damage_multiplier;
        }
    }
    return ret;
}
//This returns the damage from this damage_instance. The damage done to the target will be reduced by their armor.
float damage_instance::total_damage() const
{
    float ret = 0;
    for( const auto &elem : damage_units ) {
        ret += elem.amount * elem.damage_multiplier;
    }
    return ret;
}
void damage_instance::clear()
{
    damage_units.clear();
}

bool damage_instance::empty() const
{
    return damage_units.empty();
}

void damage_instance::add( const damage_instance &added_di )
{
    for( auto &added_du : added_di.damage_units ) {
        add( added_du );
    }
}

void damage_instance::add( const damage_unit &added_du )
{
    auto iter = std::find_if( damage_units.begin(), damage_units.end(),
    [&added_du]( const damage_unit & du ) {
        return du.type == added_du.type;
    } );
    if( iter == damage_units.end() ) {
        damage_units.emplace_back( added_du );
    } else {
        damage_unit &du = *iter;
        float mult = added_du.damage_multiplier / du.damage_multiplier;
        du.amount += added_du.amount * mult;
        du.res_pen += added_du.res_pen * mult;
        // Linearly interpolate armor multiplier based on damage proportion contributed
        float t = added_du.damage_multiplier / ( added_du.damage_multiplier + du.damage_multiplier );
        du.res_mult = lerp( du.res_mult, added_du.damage_multiplier, t );
    }
}

std::vector<damage_unit>::iterator damage_instance::begin()
{
    return damage_units.begin();
}

std::vector<damage_unit>::const_iterator damage_instance::begin() const
{
    return damage_units.begin();
}

std::vector<damage_unit>::iterator damage_instance::end()
{
    return damage_units.end();
}

std::vector<damage_unit>::const_iterator damage_instance::end() const
{
    return damage_units.end();
}

bool damage_instance::operator==( const damage_instance &other ) const
{
    return damage_units == other.damage_units;
}

void damage_instance::deserialize( JsonIn &jsin )
{
    // TODO: Clean up
    if( jsin.test_object() ) {
        JsonObject jo = jsin.get_object();
        damage_units = load_damage_instance( jo ).damage_units;
    } else if( jsin.test_array() ) {
        damage_units = load_damage_instance( jsin.get_array() ).damage_units;
    } else {
        jsin.error( "Expected object or array for damage_instance" );
    }
}

dealt_damage_instance::dealt_damage_instance()
{
    dealt_dams.fill( 0 );
}

void dealt_damage_instance::set_damage( damage_type dt, int amount )
{
    if( dt < 0 || dt >= NUM_DT ) {
        debugmsg( "Tried to set invalid damage type %d. NUM_DT is %d", dt, NUM_DT );
        return;
    }

    dealt_dams[dt] = amount;
}
int dealt_damage_instance::type_damage( damage_type dt ) const
{
    if( static_cast<size_t>( dt ) < dealt_dams.size() ) {
        return dealt_dams[dt];
    }

    return 0;
}
int dealt_damage_instance::total_damage() const
{
    return std::accumulate( dealt_dams.begin(), dealt_dams.end(), 0 );
}

resistances::resistances()
{
    resist_vals.fill( 0 );
}

resistances::resistances( const item &armor, bool to_self )
{
    // Armors protect, but all items can resist
    if( to_self || armor.is_armor() ) {
        for( int i = 0; i < NUM_DT; i++ ) {
            damage_type dt = static_cast<damage_type>( i );
            set_resist( dt, armor.damage_resist( dt, to_self ) );
        }
    }
}
resistances::resistances( monster &monster )
{
    set_resist( DT_BASH, monster.type->armor_bash );
    set_resist( DT_CUT,  monster.type->armor_cut );
    set_resist( DT_STAB, monster.type->armor_stab );
    set_resist( DT_ACID, monster.type->armor_acid );
    set_resist( DT_HEAT, monster.type->armor_fire );
}
void resistances::set_resist( damage_type dt, float amount )
{
    resist_vals[dt] = amount;
}
float resistances::type_resist( damage_type dt ) const
{
    return resist_vals[dt];
}
float resistances::get_effective_resist( const damage_unit &du ) const
{
    return std::max( type_resist( du.type ) - du.res_pen, 0.0f ) * du.res_mult;
}

resistances &resistances::operator+=( const resistances &other )
{
    for( size_t i = 0; i < NUM_DT; i++ ) {
        resist_vals[ i ] += other.resist_vals[ i ];
    }

    return *this;
}

static const std::map<damage_type, std::string> dt_map = {
    { DT_NULL, translate_marker_context( "damage type", "null" ) },
    { DT_TRUE, translate_marker_context( "damage type", "true" ) },
    { DT_BIOLOGICAL, translate_marker_context( "damage type", "biological" ) },
    { DT_BASH, translate_marker_context( "damage type", "bash" )  },
    { DT_CUT, translate_marker_context( "damage type", "cut" ) },
    { DT_STAB, translate_marker_context( "damage type", "stab" ) },
    { DT_ACID, translate_marker_context( "damage type", "acid" ) },
    { DT_HEAT, translate_marker_context( "damage type", "heat" ) },
    { DT_COLD, translate_marker_context( "damage type", "cold" ) },
    { DT_ELECTRIC, translate_marker_context( "damage type", "electric" ), }
};

std::string name_by_dt( damage_type dt )
{
    return pgettext( "damage type", dt_map.at( dt ).c_str() ) ;
}

const skill_id &skill_by_dt( damage_type dt )
{
    static skill_id skill_bashing( "bashing" );
    static skill_id skill_cutting( "cutting" );
    static skill_id skill_stabbing( "stabbing" );

    switch( dt ) {
        case DT_BASH:
            return skill_bashing;

        case DT_CUT:
            return skill_cutting;

        case DT_STAB:
            return skill_stabbing;

        default:
            return skill_id::NULL_ID();
    }
}

static damage_unit load_damage_unit( const JsonObject &curr )
{
    damage_type dt = io::string_to_enum<damage_type>( curr.get_string( "damage_type" ) );
    float amount = curr.get_float( "amount" );
    int arpen = curr.get_int( "armor_penetration", 0 );
    float armor_mul = curr.get_float( "armor_multiplier", 1.0f );
    float damage_mul = curr.get_float( "damage_multiplier", 1.0f );
    return damage_unit( dt, amount, arpen, armor_mul, damage_mul );
}

damage_instance load_damage_instance( const JsonObject &jo )
{
    damage_instance di;
    if( jo.has_array( "values" ) ) {
        for( const JsonObject &curr : jo.get_array( "values" ) ) {
            di.damage_units.push_back( load_damage_unit( curr ) );
        }
    } else if( jo.has_string( "damage_type" ) ) {
        di.damage_units.push_back( load_damage_unit( jo ) );
    }

    return di;
}

damage_instance load_damage_instance( const JsonArray &jarr )
{
    damage_instance di;
    for( const JsonObject &curr : jarr ) {
        di.damage_units.push_back( load_damage_unit( curr ) );
    }

    return di;
}

std::map<body_part, resistances> load_resistances_map( const JsonArray &jarr )
{
    std::map<body_part, resistances> ret;

    for( const JsonObject &jo : jarr ) {
        std::set<std::string> parts = jo.get_tags( "body_parts" );
        std::set<body_part> bps;
        for( const std::string &part_string : parts ) {
            if( part_string == "ALL" ) {
                // Shorthand for all body parts
                bps.insert( all_body_parts.begin(), all_body_parts.end() );
            } else {
                bps.insert( io::string_to_enum<body_part>( part_string ) );
            }
        }

        resistances res;
        for( const JsonObject &job : jo.get_array( "bonus" ) ) {
            std::string damage_type_string = job.get_string( "damage_type" );
            if( damage_type_string == "ALL" ) {

            } else {
                res.resist_vals[io::string_to_enum<damage_type>( damage_type_string )] =
                    job.get_float( "amount" );
            }
        }

        /*
        resistances res;
        res.resist_vals = load_resist_values( ao.get_array( "bonus" ) );

        for( body_part bp : bps ) {
            ret[bp] = res;
        }

        resistances ret;
        for( const JsonObject &jo : jarr ) {
            ret.resist_vals = load_resist_values( jo );
        }
        */
    }

    return ret;
}

std::array<float, NUM_DT> load_damage_array( const JsonObject &jo )
{
    std::array<float, NUM_DT> ret;
    float init_val = jo.get_float( "all", 0.0f );

    float phys = jo.get_float( "physical", init_val );
    ret[ DT_BASH ] = jo.get_float( "bash", phys );
    ret[ DT_CUT ] = jo.get_float( "cut", phys );
    ret[ DT_STAB ] = jo.get_float( "stab", phys );

    float non_phys = jo.get_float( "non_physical", init_val );
    ret[ DT_BIOLOGICAL ] = jo.get_float( "biological", non_phys );
    ret[ DT_ACID ] = jo.get_float( "acid", non_phys );
    ret[ DT_HEAT ] = jo.get_float( "heat", non_phys );
    ret[ DT_COLD ] = jo.get_float( "cold", non_phys );
    ret[ DT_ELECTRIC ] = jo.get_float( "electric", non_phys );

    // DT_TRUE should never be resisted
    ret[ DT_TRUE ] = 0.0f;
    return ret;
}
