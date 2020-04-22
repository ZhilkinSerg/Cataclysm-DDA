#include "weather_type.h"

#include <cstdlib>

#include "bodypart.h"
#include "color.h"
#include "debug.h"
#include "enum_conversions.h"
#include "enums.h"
#include "generic_factory.h"
#include "int_id.h"
#include "json.h"
#include "string_id.h"

namespace
{

generic_factory<weather_type> all_weather_types( "weather types" );

} // namespace

/** @relates int_id */
template<>
bool int_id<weather_type>::is_valid() const
{
    return all_weather_types.is_valid( *this );
}

/** @relates int_id */
template<>
const weather_type &int_id<weather_type>::obj() const
{
    return all_weather_types.obj( *this );
}

/** @relates int_id */
template<>
const string_id<weather_type> &int_id<weather_type>::id() const
{
    return all_weather_types.convert( *this );
}

/** @relates string_id */
template<>
bool string_id<weather_type>::is_valid() const
{
    return all_weather_types.is_valid( *this );
}

/** @relates string_id */
template<>
const weather_type &string_id<weather_type>::obj() const
{
    return all_weather_types.obj( *this );
}

/** @relates string_id */
template<>
int_id<weather_type> string_id<weather_type>::id() const
{
    return all_weather_types.convert( *this, weather_null );
}

/** @relates int_id */
template<>
int_id<weather_type>::int_id( const string_id<weather_type> &id ) : _id( id.id() )
{
}

void weather_type::load( const JsonObject &jo, const std::string & )
{
    optional( jo, was_loaded, "legacy_enum_id", legacy_enum_id, -1 );

    /*
    mandatory( jo, was_loaded, "name", name );
    mandatory( jo, was_loaded, "sym", symbol, unicode_codepoint_from_symbol_reader );
    color = jo.has_member( "color" ) ? color_from_string( jo.get_string( "color" ) ) : color;

    if( was_loaded && jo.has_member( "copy-from" ) && looks_like.empty() ) {
        looks_like = jo.get_string( "copy-from" );
    }
    jo.read( "looks_like", looks_like );
    */
}

void weather_type::finalize()
{
}

void weather_type::check() const
{
}

size_t weather_type::count()
{
    return all_weather_types.size();
}

void weather_types::load( const JsonObject &jo, const std::string &src )
{
    all_weather_types.load( jo, src );
}

void weather_types::finalize_all()
{
    set_weather_type_ids();
    all_weather_types.finalize();
    for( const weather_type &wt : all_weather_types.get_all() ) {
        const_cast<weather_type &>( wt ).finalize();
    }
}

void weather_types::check_consistency()
{
    all_weather_types.check();
}

void weather_types::reset()
{
    all_weather_types.reset();
}

const std::vector<weather_type> &weather_types::get_all()
{
    return all_weather_types.get_all();
}

weather_type_id weather_null,
                weather_clear,
                weather_sunny,
                weather_cloudy,
                weather_light_drizzle,
                weather_drizzle,
                weather_rainy,
                weather_thunder,
                weather_lightning,
                weather_acid_drizzle,
                weather_acid_rain,
                weather_flurries,
                weather_snow,
                weather_snowstorm
                ;

void weather_types::set_weather_type_ids()
{
    weather_null = weather_type_id( "weather_null" );
    weather_clear = weather_type_id( "weather_clear" );
    weather_sunny = weather_type_id( "weather_sunny" );
    weather_cloudy = weather_type_id( "weather_cloudy" );
    weather_light_drizzle = weather_type_id( "weather_light_drizzle" );
    weather_drizzle = weather_type_id( "weather_drizzle" );
    weather_rainy = weather_type_id( "weather_rainy" );
    weather_thunder = weather_type_id( "weather_thunder" );
    weather_lightning = weather_type_id( "weather_lightning" );
    weather_acid_drizzle = weather_type_id( "weather_acid_drizzle" );
    weather_acid_rain = weather_type_id( "weather_acid_rain" );
    weather_flurries = weather_type_id( "weather_flurries" );
    weather_snow = weather_type_id( "weather_snow" );
    weather_snowstorm = weather_type_id( "weather_snowstorm" );
}

weather_type weather_types::get_weather_type_by_legacy_enum( int legacy_enum_id )
{
    for( const auto &wt : all_weather_types.get_all() ) {
        if( legacy_enum_id == wt.legacy_enum_id ) {
            return wt;
        }
    }
    debugmsg( "Cannot find weather_type for legacy enum: %d.", legacy_enum_id );
    return weather_type();
}
