#pragma once
#ifndef CATA_SRC_RAILROAD_STATION_H
#define CATA_SRC_RAILROAD_STATION_H

#include <string>         // for string, basic_string
#include <vector>         // for vector

#include "coordinates.h"  // for operator==, point_om_omt, point_abs_om, coo...
#include "omdata.h"
#include "overmap_types.h"
#include "point.h"        // for point
#include "type_id.h"      // for railroad_station_id, string_id

static const overmap_special_id overmap_special_Railroad_Station( "Railroad Station" );

class JsonObject;
template <typename T> class generic_factory;

struct railroad_station {
    void load( const JsonObject &, const std::string & );
    void check() const;
    static void load_railroad_station( const JsonObject &, const std::string & );
    static void finalize();
    static void check_consistency();
    static const std::vector<railroad_station> &get_all();
    static void reset();

    railroad_station_id id;
    bool was_loaded = false;

    int database_id = 0;
    // location of the railroad station (in overmap coordinates)
    point_abs_om pos_om;
    // location of the railroad station (in overmap terrain coordinates)
    point_om_omt pos;
    std::string name;
    overmap_special_id special_id = overmap_special_Railroad_Station;
    om_direction::type dir;

    explicit railroad_station( const point_om_omt &P = point_om_omt() );

    bool operator==( const railroad_station &rhs ) const {
        return id == rhs.id ||
               database_id == rhs.database_id ||
               ( pos_om == rhs.pos_om && pos == rhs.pos ) ;
    }

};

generic_factory<railroad_station> &get_railroad_station_factory();

#endif // CATA_SRC_RAILROAD_STATION_H
