#include "railroad_station.h"             // IWYU pragma: associated

#include <algorithm>          // for max
#include <set>                // for set
#include <vector>             // for vector
#include "coordinates.h"      // for point_om_omt, point_abs_om, trig_dist
#include "debug.h"            // for realDebugmsg, debugmsg
#include "generic_factory.h"  // for mandatory, optional, generic_factory
#include "name.h"             // for get, nameFlags
#include "options.h"          // for get_option
#include "rng.h"              // for rng

#include "cube_direction.h"
#include "omdata.h"
#include "overmap.h"


generic_factory<railroad_station> &get_railroad_station_factory()
{
    static generic_factory<railroad_station> railroad_station_factory( "railroad_station" );
    return railroad_station_factory;
}

/** @relates string_id */
template<>
const railroad_station &string_id<railroad_station>::obj() const
{
    return get_railroad_station_factory().obj( *this );
}

/** @relates string_id */
template<>
bool string_id<railroad_station>::is_valid() const
{
    return get_railroad_station_factory().is_valid( *this );
}

void railroad_station::load_railroad_station( const JsonObject &jo, const std::string &src )
{
    get_railroad_station_factory().load( jo, src );
}

void railroad_station::finalize()
{
    for( railroad_station &r : const_cast<std::vector<railroad_station>&>
         ( railroad_station::get_all() ) ) {
        if( r.name.empty() ) {
            r.name = string_format( _( "%s RAILROAD STATION" ), Name::get( nameFlags::IsTownName ) );
        }
    }
}

void railroad_station::check_consistency()
{
    get_railroad_station_factory().check();
}

const std::vector<railroad_station> &railroad_station::get_all()
{
    return get_railroad_station_factory().get_all();
}

void railroad_station::reset()
{
    get_railroad_station_factory().reset();
}

void railroad_station::load( const JsonObject &jo, const std::string & )
{

    mandatory( jo, was_loaded, "id", id );
    mandatory( jo, was_loaded, "database_id", database_id );
    optional( jo, was_loaded, "name", name );
    optional( jo, was_loaded, "special_id", special_id );
    mandatory( jo, was_loaded, "pos_om", pos_om );
    mandatory( jo, was_loaded, "pos", pos );
    dir = om_direction::random();
    optional( jo, was_loaded, "dir", dir );
}

void railroad_station::check() const
{
}

railroad_station::railroad_station( const point_om_omt &P )
    : pos( P )
    , name( Name::get( nameFlags::IsTownName ) )
{
}
