#include "damage_type.h"

#include "debug.h"
#include "generic_factory.h"
#include "json.h"

namespace
{

generic_factory<damage_type> all_damage_types( "damage types" );

} // namespace

/** @relates int_id */
template<>
bool int_id<damage_type>::is_valid() const
{
    return all_damage_types.is_valid( *this );
}

/** @relates int_id */
template<>
const damage_type &int_id<damage_type>::obj() const
{
    return all_damage_types.obj( *this );
}

/** @relates int_id */
template<>
const string_id<damage_type> &int_id<damage_type>::id() const
{
    return all_damage_types.convert( *this );
}

/** @relates string_id */
template<>
bool string_id<damage_type>::is_valid() const
{
    return all_damage_types.is_valid( *this );
}

/** @relates string_id */
template<>
const damage_type &string_id<damage_type>::obj() const
{
    return all_damage_types.obj( *this );
}

/** @relates string_id */
template<>
int_id<damage_type> string_id<damage_type>::id() const
{
    return all_damage_types.convert( *this, DT_NULL );
}

/** @relates int_id */
template<>
int_id<damage_type>::int_id( const string_id<damage_type> &id ) : _id( id.id() )
{
}

void damage_type::load( const JsonObject &jo, const std::string & )
{
    optional( jo, was_loaded, "name", name );
    optional( jo, was_loaded, "description", description );
    std::string color_string;
    optional( jo, was_loaded, "color", color_string, "black" );
    color = color_from_string( color_string );
    optional( jo, was_loaded, "is_typeless", is_typeless, false );
    optional( jo, was_loaded, "is_internal", is_internal, false );
    optional( jo, was_loaded, "is_physical", is_physical, false );
    optional( jo, was_loaded, "skill", skill );
}

void damage_type::finalize()
{
}

void damage_type::check() const
{
}

size_t damage_type::count()
{
    return all_damage_types.size();
}

void damage_types::load( const JsonObject &jo, const std::string &src )
{
    all_damage_types.load( jo, src );
}

void damage_types::finalize_all()
{
    set_damage_type_ids();
    all_damage_types.finalize();
    for( const damage_type &dt : all_damage_types.get_all() ) {
        const_cast<damage_type &>( dt ).finalize();
    }
}

void damage_types::check_consistency()
{
    all_damage_types.check();
}

void damage_types::reset()
{
    all_damage_types.reset();
}

const std::vector<damage_type> &damage_types::get_all()
{
    return all_damage_types.get_all();
}

damage_type_id DT_NULL,
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

void damage_types::set_damage_type_ids()
{
    DT_NULL = damage_type_id( "null" );
    DT_TRUE = damage_type_id( "true" );
    DT_BIOLOGICAL = damage_type_id( "biological" );
    DT_BASH = damage_type_id( "bash" );
    DT_CUT = damage_type_id( "cut" );
    DT_ACID = damage_type_id( "acid" );
    DT_STAB = damage_type_id( "stab" );
    DT_HEAT = damage_type_id( "heat" );
    DT_COLD = damage_type_id( "cold" );
    DT_ELECTRIC = damage_type_id( "electric" );
}
