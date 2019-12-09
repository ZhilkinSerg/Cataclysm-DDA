#include "gamemode_MA.h" // IWYU pragma: associated

//#include <set>
//#include <vector>

#include "sqlite3.h"

#include "game.h"
#include "omdata.h"
#include "output.h"
#include "overmap.h"
#include "game_constants.h"
#include "point.h"
#include "coordinate_conversions.h"
#include "avatar.h"
#include "profession.h"

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
    //popup( "Beware! Certain overmaps would be generated using Massachusetts state geodata!" );
    //g->weather.temperature = 65;
    // We use a Z-factor of 10 so that we don't plop down tutorial rooms in the
    // middle of the "real" game world
    g->u.normalize();
    g->u.str_cur = g->u.str_max;
    g->u.per_cur = g->u.per_max;
    g->u.int_cur = g->u.int_max;
    g->u.dex_cur = g->u.dex_max;
    for( int i = 0; i < num_hp_parts; i++ ) {
        g->u.hp_cur[i] = g->u.hp_max[i];
    }
    //~ default name for the tutorial
    g->u.name = _( "John Smith" );
    g->u.prof = profession::generic();

    const tripoint offset( OMAPX * 45, OMAPY * 12, 0 );
    const tripoint where( g->u.global_omt_location() + offset );
    g->place_player_overmap( where );
    g->load_map( omt_to_sm_copy( where ) );

    // overmap terrain coordinates
    //const tripoint lp( 50, 50, 0 );
    //g->load_map( omt_to_sm_copy( lp ) );
    //g->u.setx( 2 );
    //g->u.sety( 4 );

    // This shifts the view to center the players pos
    g->update_map( g->u );
    return true;
}

bool ma_game::generate_overmap( const tripoint &om_pos )
{
    //return false;
    const int z = om_pos.z;
    sqlite3 *MA_db;
    int rc = sqlite3_open( MA_database_path.c_str(), &MA_db );
    if( rc ) {
        //debugmsg( "Can't open database: %s", sqlite3_errmsg( MA_db ) );
        return false;
    }
    const std::string MA_sql = string_format(
                                   "select omt_x, omt_y, land_use_code, town_id, ocean from omt where om_x = %1$d and om_y = %2$d;",
                                   om_pos.x, om_pos.y );
    sqlite3_stmt *MA_statement;
    rc = sqlite3_prepare( MA_db, MA_sql.c_str(), MA_sql.length(), &MA_statement, nullptr );
    if( rc ) {
        //debugmsg( "Can't prepare sql statement %s", sqlite3_errmsg( MA_db ) );
        sqlite3_close( MA_db );
        return false;
    }

    overmap_terrain_data MA_omt[OMAPX][OMAPY];
    for( size_t row = 0; row <= OMAPX * OMAPY; ++row ) {
        rc = sqlite3_step( MA_statement );
        if( rc == SQLITE_DONE || rc != SQLITE_ROW ) {
            //debugmsg( "Can't step next row %s", sqlite3_errmsg( MA_db ) );
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
            std::string new_ter_id = "open_air";
            std::string new_ter_rot = om_direction::name( om_direction::random() );
            if( otd.ocean == 1 ) {
                new_ter_id = "river_center";
                new_ter_rot.clear();
            } else {
                const land_use_code luc = static_cast<land_use_code>( otd.land_use_code );
                switch( luc ) {
                    case land_use_code::cropland:
                        new_ter_id = "field";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::pasture:
                        new_ter_id = "field";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::forest:
                        new_ter_id = "forest";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::wetland:
                        new_ter_id = "forest_water";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::mining:
                        new_ter_id = "crater";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::open_land:
                        new_ter_id = "field";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::recreation_participation:
                        new_ter_id = "skate_park";
                        break;
                    case land_use_code::recreation_spectator:
                        new_ter_id = "football_field_a1";
                        break;
                    case land_use_code::recreation_water:
                        new_ter_id = "pool";
                        break;
                    case land_use_code::residential_multi:
                        new_ter_id = "house";
                        break;
                    case land_use_code::residential_high:
                        new_ter_id = "house";
                        break;
                    case land_use_code::residential_medium:
                        new_ter_id = "house";
                        break;
                    case land_use_code::residential_low:
                        new_ter_id = "house";
                        break;
                    case land_use_code::wetland_saltwater:
                        new_ter_id = "forest_water";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::commercial:
                        new_ter_id = "s_gas";
                        break;
                    case land_use_code::industrial:
                        new_ter_id = "warehouse";
                        break;
                    case land_use_code::transitional:
                        new_ter_id = "field";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::transportation:
                        new_ter_id = "road_nesw";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::waste_disposal:
                        new_ter_id = "toxic_dump";
                        break;
                    case land_use_code::water:
                        new_ter_id = "river_center";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::cranberry_bog:
                        new_ter_id = "forest_water";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::powerline_utility:
                        new_ter_id = "sai";
                        break;
                    case land_use_code::saltwater_sandy_beach:
                        new_ter_id = "forest_water";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::golf_course:
                        new_ter_id = "golfcourse_00";
                        break;
                    case land_use_code::marina:
                        new_ter_id = "marina_1";
                        break;
                    case land_use_code::urban_public_institutional:
                        new_ter_id = "church";
                        break;
                    case land_use_code::cemetery:
                        new_ter_id = "cemetery";
                        break;
                    case land_use_code::orchard:
                        new_ter_id = "orchard_tree_apple";
                        break;
                    case land_use_code::nursery:
                        new_ter_id = "TreeFarm_1b";
                        break;
                    case land_use_code::wetland_forest:
                        new_ter_id = "forest_water";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::residential_very_low:
                        new_ter_id = "house";
                        break;
                    case land_use_code::junkyard:
                        new_ter_id = "junkyard_1a";
                        break;
                    case land_use_code::brushland:
                        new_ter_id = "field";
                        new_ter_rot.clear();
                        break;
                    case land_use_code::none:
                    case land_use_code::NUM_LAND_USE_CODE:
                    default:
                        new_ter_rot.clear();
                        break;
                }

            }
            oter_id new_t = oter_id( new_ter_id + ( new_ter_rot.empty() ? "" : "_" + new_ter_rot ) );
            om.ter_set( p, new_t );
        }
    }

    for( int i = 0; i < OMAPX; i++ ) {
        for( int j = 0; j < OMAPY; j++ ) {
            //for( int k = -OVERMAP_DEPTH; k <= OVERMAP_HEIGHT; k++ ) {
            om.seen( { i, j, 0 } ) = true;
            //}
        }
    }
    om.save();
    //overmap_buffer.clear();
    return true;
}

bool ma_game::place_cities( const tripoint &om_pos )
{

    const int z = om_pos.z;
    sqlite3 *MA_db;
    int rc = sqlite3_open( MA_database_path.c_str(), &MA_db );
    if( rc ) {
        //debugmsg( "Can't open database: %s", sqlite3_errmsg( MA_db ) );
        return false;
    }
    const std::string MA_sql = string_format(
                                   "select town_id, town_name, omt_x, omt_y from towns where om_x = %1$d and om_y = %2$d;",
                                   om_pos.x, om_pos.y );
    sqlite3_stmt *MA_statement;
    rc = sqlite3_prepare( MA_db, MA_sql.c_str(), MA_sql.length(), &MA_statement, nullptr );
    if( rc ) {
        //debugmsg( "Can't prepare sql statement %s", sqlite3_errmsg( MA_db ) );
        sqlite3_close( MA_db );
        return false;
    }

    overmap &om = overmap_buffer.get( om_pos.xy() );
    const string_id<overmap_connection> local_road_id( "local_road" );
    const overmap_connection &local_road( *local_road_id );

    for( size_t row = 0; row <= OMAPX * OMAPY; ++row ) {
        rc = sqlite3_step( MA_statement );
        if( rc == SQLITE_DONE || rc != SQLITE_ROW ) {
            //debugmsg( "Can't step next row %s", sqlite3_errmsg( MA_db ) );
            break;
        }
        int col = 0;
        const int cid = sqlite3_column_int( MA_statement, col++ );
        const std::string cname = ( char * )sqlite3_column_text( MA_statement, col++ );
        const int cx = sqlite3_column_int( MA_statement, col++ );
        const int cy = sqlite3_column_int( MA_statement, col++ );
        const int size = rng( 1, 16 );
        const tripoint p( cx, cy, om_pos.z );
        city tmp;
        tmp.pos = p.xy();
        tmp.name = string_format( "%d-%s", cid, cname );
        om.cities.push_back( tmp );
        om.ter_set( p, oter_id( "road_nesw" ) ); // every city starts with an intersection
        const auto start_dir = om_direction::random();
        auto cur_dir = start_dir;
        do {
            om.build_city_street( local_road, tmp.pos, size, cur_dir, tmp );
        } while( ( cur_dir = om_direction::turn_right( cur_dir ) ) != start_dir );
    }

    for( int i = 0; i < OMAPX; i++ ) {
        for( int j = 0; j < OMAPY; j++ ) {
            //for( int k = -OVERMAP_DEPTH; k <= OVERMAP_HEIGHT; k++ ) {
            om.seen( { i, j, 0 } ) = true;
            //}
        }
    }

    sqlite3_close( MA_db );
    return true;
}
