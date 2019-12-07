#include "gamemode_MA.h" // IWYU pragma: associated

//#include <set>
//#include <vector>

#include "sqlite3.h"

#include "game.h"
#include "output.h"
#include "overmap.h"
#include "game_constants.h"
#include "point.h"

enum class land_use_code : int {
    none = 0,
    cropland,
    pasture,
    forest,
    wetland,
    mining,
    open_land,
    recreation_participation,
    recreation_spectator,
    recreation_water,
    residential_multi,
    residential_high,
    residential_medium,
    residential_low,
    wetland_saltwater,
    commercial,
    industrial,
    transitional,
    transportation,
    waste_disposal,
    water,
    cranberry_bog,
    powerline_utility,
    saltwater_sandy_beach,
    golf_course,
    marina,
    urban_public_institutional,
    cemetery,
    orchard,
    nursery,
    wetland_forest,
    residential_very_low,
    junkyard,
    brushland,
    NUM_LAND_USE_CODE
};

/*
struct MA_road_data {
    int primary_road_type;
    std::set<int> all_road_types;
};

struct MA_subway_data {
    std::string subway_primary_line;
    std::string subway_all_lines;
};

struct MA_rapid_transit_data {
    std::string rapid_transit_primary_line;
    int rapid_transit_primary_line_grade;
    std::string rapid_transit_all_lines;
    std::string rapid_transit_all_lines_grades;
};

struct MA_transit_station_data {
    std::string transit_station_line;
    int transit_station_is_terminus;
};

struct MA_military_base_data {
    std::string military_base_component;
    std::string military_base_name;
};

struct MA_primary_structure_data {
    std::string primary_structure_id;
    double primary_structure_total_area_sq_m;
    int primary_structure_other_cell_cover_count;
    int cell_structure_count;
    double cell_structure_total_percentage_covered;
};
*/

struct overmap_terrain_data {
    //int om_page_number;
    //int omt_page_number;
    //point om_pos;
    //point omt_pos;
    int land_use_code;
    //MA_road_data road_data;
    //MA_subway_data subway_data;
    //MA_rapid_transit_data rapid_transit_data;
    //MA_transit_station_data transit_station_data;
    //MA_military_base_data military_base_data;
    int town_id;
    //int trail_class;
    //int train_type;
    //int station;
    int ocean;
    //MA_primary_structure_data primary_structure_data;
};

bool ma_game::init()
{
    popup( "Beware! Certain overmaps would be generated using Massachusetts state geodata!" );
    return true;
}

bool ma_game::generate_overmap( const tripoint &om_pos )
{
    const int z = om_pos.z;
    sqlite3 *MA_db;
    int rc = sqlite3_open( MA_database_path.c_str(), &MA_db );
    if( rc ) {
        debugmsg( "Can't open database: %s", sqlite3_errmsg( MA_db ) );
        return false;
    }
    const std::string MA_sql = string_format(
                                   "select omt_x, omt_y, land_use_code, town_id, ocean from omt where om_x = %1$d and om_y = %2$d;",
                                   om_pos.x, om_pos.y );
    sqlite3_stmt *MA_statement;
    rc = sqlite3_prepare( MA_db, MA_sql.c_str(), MA_sql.length(), &MA_statement, nullptr );
    if( rc ) {
        debugmsg( "Can't prepare sql statement %s", sqlite3_errmsg( MA_db ) );
        sqlite3_close( MA_db );
        return false;
    }

    overmap_terrain_data MA_omt[OMAPX][OMAPY];
    for( size_t row = 0; row <= OMAPX * OMAPY; ++row ) {
        rc = sqlite3_step( MA_statement );
        if( rc == SQLITE_DONE || rc != SQLITE_ROW ) {
            debugmsg( "Can't step next row %s", sqlite3_errmsg( MA_db ) );
            break;
        }
        int col = 0;
        overmap_terrain_data MA_otd;
        const int x = sqlite3_column_int( MA_statement, col++ );
        const int y = sqlite3_column_int( MA_statement, col++ );
        MA_otd.land_use_code = sqlite3_column_int( MA_statement, col++ );
        MA_otd.town_id = sqlite3_column_int( MA_statement, col++ );
        MA_otd.ocean = sqlite3_column_int( MA_statement, col );
        MA_omt[x][y] = MA_otd;
    }
    sqlite3_close( MA_db );

    overmap &om = overmap_buffer.get( om_pos.xy() );
    for( int x = 0; x < OMAPX; x++ ) {
        for( int y = 0; y < OMAPY; y++ ) {
            tripoint p( x, y, z );
            const overmap_terrain_data &otd = MA_omt[x][y];
            oter_id new_t = oter_id( "open_air" );
            if( otd.ocean == 1 ) {
                new_t = oter_id( "river_center" );
            } else {
                const land_use_code luc = static_cast<land_use_code>( otd.land_use_code );
                switch( luc ) {
                    case land_use_code::cropland:
                        new_t = oter_id( "field" );
                        break;
                    case land_use_code::pasture:
                        new_t = oter_id( "field" );
                        break;
                    case land_use_code::forest:
                        new_t = oter_id( "forest" );
                        break;
                    case land_use_code::wetland:
                        new_t = oter_id( "swamp" );
                        break;
                    case land_use_code::mining:
                        new_t = oter_id( "crater" );
                        break;
                    case land_use_code::open_land:
                        new_t = oter_id( "field" );
                        break;
                    case land_use_code::recreation_participation:
                        new_t = oter_id( "skate_park" );
                        break;
                    case land_use_code::recreation_spectator:
                        new_t = oter_id( "football_field_a1" );
                        break;
                    case land_use_code::recreation_water:
                        new_t = oter_id( "pool" );
                        break;
                    case land_use_code::residential_multi:
                        new_t = oter_id( "house" );
                        break;
                    case land_use_code::residential_high:
                        new_t = oter_id( "house" );
                        break;
                    case land_use_code::residential_medium:
                        new_t = oter_id( "house" );
                        break;
                    case land_use_code::residential_low:
                        new_t = oter_id( "house" );
                        break;
                    case land_use_code::wetland_saltwater:
                        new_t = oter_id( "swamp" );
                        break;
                    case land_use_code::commercial:
                        new_t = oter_id( "s_gas" );
                        break;
                    case land_use_code::industrial:
                        new_t = oter_id( "warehouse" );
                        break;
                    case land_use_code::transitional:
                        new_t = oter_id( "field" );
                        break;
                    case land_use_code::transportation:
                        new_t = oter_id( "road_nesw" );
                        break;
                    case land_use_code::waste_disposal:
                        new_t = oter_id( "toxic_dump" );
                        break;
                    case land_use_code::water:
                        new_t = oter_id( "river_center" );
                        break;
                    case land_use_code::cranberry_bog:
                        new_t = oter_id( "swamp" );
                        break;
                    case land_use_code::powerline_utility:
                        new_t = oter_id( "sai" );
                        break;
                    case land_use_code::saltwater_sandy_beach:
                        new_t = oter_id( "swamp" );
                        break;
                    case land_use_code::golf_course:
                        new_t = oter_id( "golfcourse_00" );
                        break;
                    case land_use_code::marina:
                        new_t = oter_id( "marina_1_north" );
                        break;
                    case land_use_code::urban_public_institutional:
                        new_t = oter_id( "church" );
                        break;
                    case land_use_code::cemetery:
                        new_t = oter_id( "cemetery" );
                        break;
                    case land_use_code::orchard:
                        new_t = oter_id( "orchard_tree_apple" );
                        break;
                    case land_use_code::nursery:
                        new_t = oter_id( "TreeFarm_1b" );
                        break;
                    case land_use_code::wetland_forest:
                        new_t = oter_id( "forest_water" );
                        break;
                    case land_use_code::residential_very_low:
                        new_t = oter_id( "house" );
                        break;
                    case land_use_code::junkyard:
                        new_t = oter_id( "junkyard_1a" );
                        break;
                    case land_use_code::brushland:
                        new_t = oter_id( "field" );
                        break;
                    case land_use_code::none:
                    case land_use_code::NUM_LAND_USE_CODE:
                    default:
                        break;
                }

            }
            om.ter_set( p, new_t );
        }
    }
    return false;
}
