#include "gamemode_tutorial.h" // IWYU pragma: associated

#include <array>
#include <vector>

#include "sqlite3.h"

#include "action.h"
#include "avatar.h"
#include "cata_utility.h"
#include "coordinate_conversions.h"
#include "game.h"
#include "gamemode_tutorial.h"
#include "json.h"
#include "map.h"
#include "map_iterator.h"
#include "mapdata.h"
#include "output.h"
#include "omdata.h"
#include "overmap.h"
#include "overmapbuffer.h"
#include "player.h"
#include "profession.h"
#include "scent_map.h"
#include "translations.h"
#include "trap.h"
#include "calendar.h"
#include "game_constants.h"
#include "int_id.h"
#include "inventory.h"
#include "item.h"
#include "pldata.h"
#include "units.h"
#include "translations.h"
#include "type_id.h"
#include "point.h"
#include "weather.h"

const mtype_id mon_zombie( "mon_zombie" );

struct overmap_terrain_data {
    point omt_pos;
    overmap_land_use_code_id luc;
    bool is_road = false;
    bool is_water = false;
    std::string suffix;
    std::string specific_terrain;
    void clear() {
        omt_pos = point_zero;
        suffix.clear();
        specific_terrain.clear();
    }
};

struct overmap_data {
    std::string id;
    tripoint om_pos;
    std::vector<overmap_terrain_data> omt_data;
    void clear() {
        om_pos = tripoint_zero;
        omt_data.clear();
    }
};

static std::vector<translation> tut_text;
static std::vector<overmap_data> MA_overmap_data;

bool tutorial_game::init()
{
    // TODO: clean up old tutorial

    // Start at noon
    calendar::turn = calendar::turn_zero + 12_hours;
    for( auto &elem : tutorials_seen ) {
        elem = false;
    }
    g->scent.reset();
    g->weather.temperature = 65;
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

    init_map();

    g->u.toggle_trait( trait_id( "QUICK" ) );
    item lighter( "lighter", 0 );
    lighter.invlet = 'e';
    g->u.inv.add_item( lighter, true, false );
    g->u.set_skill_level( skill_id( "gun" ), 5 );
    g->u.set_skill_level( skill_id( "melee" ), 5 );

    return true;
}

void tutorial_game::init_map()
{
    const std::string MA_database_path = R"(C:\Projects\Cataclysm-DDA\database\MA.sqlite3)";
    //const tripoint MA_om_pos = tripoint( 41, 64, 0 );
    tripoint_range MA_om_tripoint_range( tripoint( 49, 0, 0 ), tripoint( 50, 1, 0 ) );
    //tripoint_range MA_om_tripoint_range( tripoint( 40, 20, 0 ), tripoint( 45, 25, 0 ) );
    tripoint MA_start = MA_om_tripoint_range.min();
    //size_t i = 0;
    size_t i_max = MA_om_tripoint_range.size();
    for( const tripoint &MA_om_pos : MA_om_tripoint_range ) {
        /*
        i++;
        if( i >= i_max / 20 ) {
            debugmsg( "Loaded 20%% of MA. Stopping loading!" );
            break;
        }
        */
        load_tutorial_overmap_from_database( MA_database_path, MA_om_pos );
    }

    //debugmsg( "Loaded successfully using database %s!", MA_database_path );

    /*
    auto &starting_om = overmap_buffer.get( point_zero );
    for( int i = 0; i < OMAPX; i++ ) {
    for( int j = 0; j < OMAPY; j++ ) {
        overmap_land_use_code_id luc = overmap_land_use_codes::get_random();
        oter_id new_t = overmap_terrains::get_random_terrain_by_land_use_code( luc, rock );
        tripoint p( i, j, 0 );
        starting_om.ter_set( p, new_t );
        // Start with the overmap revealed
        starting_om.seen( p ) = true;
    }
    }
    */

    //i = 0;
    for( const overmap_data &omd : MA_overmap_data ) {
        /*
        i++;
        if( i >= i_max / 20 ) {
            debugmsg( "Generated 20%% of MA. Stopping generating!" );
            break;
        }
        debugmsg( "Generating: %s using coords %s with size of omd=%d", omd.id, omd.om_pos.to_string(),
                  omd.omt_data.size() );
        */
        //overmap_buffer.clear();
        overmap &om = overmap_buffer.get( omd.om_pos.xy() );
        //const oter_id fallback_ter = om.get_default_terrain( omd.om_pos.z );
        //const oter_id fallback_ter( "river_center" );
        const oter_id fallback_ter( "rock" );
        for( const overmap_terrain_data &otd : omd.omt_data ) {
            tripoint p( otd.omt_pos, 0 );
            oter_id new_t;
            /*
            if( !otd.specific_terrain.empty() ) {
                new_t = oter_id( otd.specific_terrain );
            }
            */
            if( otd.is_road ) {
                new_t = oter_id( "road_isolated" );
            } else if( otd.is_water ) {
                new_t = oter_id( "river_center" );
            } else {
                overmap_land_use_code_id luc = otd.luc;
                if( luc == overmap_land_use_code_id::NULL_ID() ) {
                    new_t = fallback_ter;
                } else {
                    new_t = overmap_terrains::get_random_terrain_by_land_use_code( luc, fallback_ter );
                }
            }
            om.ter_set( p, new_t );
        }

        ///*
        for( int i = 0; i < OMAPX; i++ ) {
            for( int j = 0; j < OMAPY; j++ ) {
                tripoint p( i, j, 0 );
                om.seen( p ) = true;
            }
        }
        //*/

        om.connect_roads_and_trails();
        om.save();
        DebugLog( D_ERROR, D_GAME ) << "Saved generated overmap: " << omd.om_pos.xy().to_string();
    }

    //debugmsg( "Generated successfully using MA data from database %s!", MA_database_path );

    // overmap terrain coordinates
    const tripoint lp( 50, 50, 0 );
    auto &starting_om = overmap_buffer.get( MA_start.xy() );
    starting_om.ter_set( lp, oter_id( "tutorial" ) );
    starting_om.ter_set( lp + tripoint_below, oter_id( "tutorial" ) );
    starting_om.clear_mon_groups();
    /*
    for( int i = 0; i < OMAPX; i++ ) {
        for( int j = 0; j < OMAPY; j++ ) {
            tripoint p( i, j, 0 );
            starting_om.seen( p ) = true;
        }
    }
    */
    g->load_map( omt_to_sm_copy( lp ) );

    const tripoint offset( OMAPX * MA_start.x, OMAPY * MA_start.y, MA_start.z );
    const tripoint where( g->u.global_omt_location() + offset );
    g->place_player_overmap( where );

    //g->u.setx( 2 );
    //g->u.sety( 4 );

    // This shifts the view to center the players pos
    g->update_map( g-> u );
}

void tutorial_game::per_turn()
{
    return;
    // note that add_message does nothing if the message was already shown
    add_message( LESSON_INTRO );
    add_message( LESSON_MOVE );
    add_message( LESSON_LOOK );

    if( g->light_level( g->u.posz() ) == 1 ) {
        if( g->u.has_amount( "flashlight", 1 ) ) {
            add_message( LESSON_DARK );
        } else {
            add_message( LESSON_DARK_NO_FLASH );
        }
    }

    if( g->u.get_pain() > 0 ) {
        add_message( LESSON_PAIN );
    }

    if( g->u.recoil >= MAX_RECOIL ) {
        add_message( LESSON_RECOIL );
    }

    if( !tutorials_seen[LESSON_BUTCHER] ) {
        for( const item &it : g->m.i_at( point( g->u.posx(), g->u.posy() ) ) ) {
            if( it.is_corpse() ) {
                add_message( LESSON_BUTCHER );
                break;
            }
        }
    }

    for( const tripoint &p : g->m.points_in_radius( g->u.pos(), 1 ) ) {
        if( g->m.ter( p ) == t_door_o ) {
            add_message( LESSON_OPEN );
            break;
        } else if( g->m.ter( p ) == t_door_c ) {
            add_message( LESSON_CLOSE );
            break;
        } else if( g->m.ter( p ) == t_window ) {
            add_message( LESSON_SMASH );
            break;
        } else if( g->m.furn( p ) == f_rack && !g->m.i_at( p ).empty() ) {
            add_message( LESSON_EXAMINE );
            break;
        } else if( g->m.ter( p ) == t_stairs_down ) {
            add_message( LESSON_STAIRS );
            break;
        } else if( g->m.ter( p ) == t_water_sh ) {
            add_message( LESSON_PICKUP_WATER );
            break;
        }
    }

    if( !g->m.i_at( point( g->u.posx(), g->u.posy() ) ).empty() ) {
        add_message( LESSON_PICKUP );
    }
}

void tutorial_game::pre_action( action_id &act )
{
    switch( act ) {
        case ACTION_SAVE:
        case ACTION_QUICKSAVE:
            //popup( _( "You're saving a tutorial - the tutorial world lacks certain features of normal worlds.  "
            //          "Weird things might happen when you load this save.  You have been warned." ) );
            break;
        default:
            // Other actions are fine.
            break;
    }
}

void tutorial_game::post_action( action_id act )
{
    switch( act ) {
        case ACTION_RELOAD_WEAPON:
            if( g->u.weapon.is_gun() && !tutorials_seen[LESSON_GUN_FIRE] ) {
                g->place_critter_at( mon_zombie, tripoint( g->u.posx(), g->u.posy() - 6, g->u.posz() ) );
                g->place_critter_at( mon_zombie, tripoint( g->u.posx() + 2, g->u.posy() - 5, g->u.posz() ) );
                g->place_critter_at( mon_zombie, tripoint( g->u.posx() - 2, g->u.posy() - 5, g->u.posz() ) );
                add_message( LESSON_GUN_FIRE );
            }
            break;

        case ACTION_OPEN:
            add_message( LESSON_CLOSE );
            break;

        case ACTION_CLOSE:
            add_message( LESSON_SMASH );
            break;

        case ACTION_USE:
            if( g->u.has_amount( "grenade_act", 1 ) ) {
                add_message( LESSON_ACT_GRENADE );
            }
            for( const tripoint &dest : g->m.points_in_radius( g->u.pos(), 1 ) ) {
                if( g->m.tr_at( dest ).id == trap_str_id( "tr_bubblewrap" ) ) {
                    add_message( LESSON_ACT_BUBBLEWRAP );
                }
            }
            break;

        case ACTION_EAT:
            if( g->u.last_item == "codeine" ) {
                add_message( LESSON_TOOK_PAINKILLER );
            } else if( g->u.last_item == "cig" ) {
                add_message( LESSON_TOOK_CIG );
            } else if( g->u.last_item == "water" ) {
                add_message( LESSON_DRANK_WATER );
            }
            break;

        case ACTION_WEAR: {
            item it( g->u.last_item, 0 );
            if( it.is_armor() ) {
                if( it.get_coverage() >= 2 || it.get_thickness() >= 2 ) {
                    add_message( LESSON_WORE_ARMOR );
                }
                if( it.get_storage() >= units::from_liter( 5 ) ) {
                    add_message( LESSON_WORE_STORAGE );
                }
                if( it.get_env_resist() >= 2 ) {
                    add_message( LESSON_WORE_MASK );
                }
            }
        }
        break;

        case ACTION_WIELD:
            if( g->u.weapon.is_gun() ) {
                add_message( LESSON_GUN_LOAD );
            }
            break;

        case ACTION_EXAMINE:
            add_message( LESSON_INTERACT );
        /* fallthrough */
        case ACTION_PICKUP: {
            item it( g->u.last_item, 0 );
            if( it.is_armor() ) {
                add_message( LESSON_GOT_ARMOR );
            } else if( it.is_gun() ) {
                add_message( LESSON_GOT_GUN );
            } else if( it.is_ammo() ) {
                add_message( LESSON_GOT_AMMO );
            } else if( it.is_tool() ) {
                add_message( LESSON_GOT_TOOL );
            } else if( it.is_food() ) {
                add_message( LESSON_GOT_FOOD );
            } else if( it.is_melee() ) {
                add_message( LESSON_GOT_WEAPON );
            }

        }
        break;

        default:
            // TODO: add more actions here
            break;

    }
}

void tutorial_game::add_message( tut_lesson lesson )
{
    if( tutorials_seen[lesson] ) {
        return;
    }
    tutorials_seen[lesson] = true;
    popup( tut_text[lesson].translated(), PF_ON_TOP );
    g->refresh_all();
}

void load_tutorial_overmap( JsonObject &jo )
{
    ( void ) jo;
    /*
    MA_overmap_data.clear();
    MA_overmap_data.id = jo.get_string( "id" );
    MA_overmap_data.om_pos = tripoint( jo.get_int( "om_x" ),
                                        jo.get_int( "om_y" ),
                                        jo.get_int( "om_z" ) );
    JsonArray omtd = jo.get_array( "omt_data" );
    while( omtd.has_more() ) {
        JsonObject jotd = omtd.next_object();
        overmap_terrain_data otd;
        otd.omt_pos = point( jotd.get_int( "x", 0 ), jotd.get_int( "y", 0 ) );
        const int luc = jotd.get_int( "luc", 0 );
        if( luc == 0 ) {
            //DebugLog( D_ERROR, D_GAME ) << "!!!luc at coords=" << otd.omt_pos.to_string() << " equals to " << luc;
            continue;
        }
        //DebugLog( D_ERROR, D_GAME ) << "luc at coords=" << otd.omt_pos.to_string() << " equals to " << luc;
        otd.luc = overmap_land_use_codes::get_by_land_use_code( luc );
        otd.is_road = jotd.get_bool( "is_road", false );
        otd.is_water = jotd.get_bool( "is_water", false );
        otd.suffix = jotd.get_string( "suffix", "" );
        otd.specific_terrain = jotd.get_string( "specific_terrain", "" );
        MA_overmap_data.omt_data.emplace_back( otd );
    }
    */
}

void load_tutorial_overmap_from_database( const std::string &path_to_database,
        const tripoint &om_pos )
{
    sqlite3 *db;
    char *zErrMsg = nullptr;
    int rc = sqlite3_open( path_to_database.c_str(), &db );
    //const char *data = "Callback function called";

    if( rc ) {
        debugmsg( "Can't open database: %s", sqlite3_errmsg( db ) );
        return;
    }

    overmap_data temp_od;
    temp_od.clear();
    temp_od.id = string_format( "MA_%s", om_pos.to_string() );
    temp_od.om_pos = om_pos;
    DebugLog( D_ERROR, D_GAME ) << "Loading from database: " << temp_od.om_pos.to_string();
    //debugmsg( "trying to load: %s, %s", temp_od.id, temp_od.om_pos.to_string() );
    const std::string my_select_sql = string_format(
                                          "select omt_x, omt_y, land_use_code from omt where om_x = %1$d and om_y = %2$d order by omt_x, omt_y;",
                                          om_pos.x, om_pos.y );
    sqlite3_stmt *statement;
    int x = tripoint_min.x;
    int y = tripoint_min.y;
    int land_use_code = 0;
    //debugmsg( "%s", my_select_sql );
    rc = sqlite3_prepare( db, my_select_sql.c_str(), my_select_sql.length(), &statement, nullptr );
    /*
    if( rc != SQLITE_OK && rc != SQLITE_DONE ) {
        debugmsg( "sqlite3_prepare error: %d", rc );
    }
    */
    overmap_terrain_data otd;
    for( size_t i = 0; i < OMAPX * OMAPY; ++i ) {
        //DebugLog( D_ERROR, D_GAME ) << "i:" << i;
        rc = sqlite3_step( statement );
        if( rc == SQLITE_DONE || rc != SQLITE_ROW ) {
            break;
        }
        otd.clear();
        x = sqlite3_column_int( statement, 0 );
        y = sqlite3_column_int( statement, 1 );
        land_use_code = sqlite3_column_int( statement, 2 );
        // bool is_road = sqlite3_column_int( statement, 2 );
        otd.omt_pos = point( x, y );
        //debugmsg( "LUC on MA_%s at %s=%d", om_pos.to_string(), otd.omt_pos.to_string(), land_use_code );
        /*
        if( luc == 0 ) {
            //DebugLog( D_ERROR, D_GAME ) << "!!!luc at coords=" << otd.omt_pos.to_string() << " equals to " <<
            //                            land_use_code;
            continue;
        }
        */
        otd.luc = overmap_land_use_codes::get_by_land_use_code( land_use_code );
        temp_od.omt_data.emplace_back( otd );
    }
    /*
    rc = sqlite3_finalize( statement );
    if( rc != SQLITE_OK ) {
        debugmsg( "SQL error: %s", zErrMsg );
        sqlite3_free( zErrMsg );
    }
    */
    sqlite3_close( db );
    MA_overmap_data.emplace_back( temp_od );
}

void load_tutorial_messages( JsonObject &jo )
{
    // loading them all at once, as they have to be in exact order
    tut_text.clear();
    for( auto messages : jo.get_array( "messages" ) ) {
        translation next;
        messages.read( next );
        tut_text.emplace_back( next );
    }
}

void clear_tutorial_messages()
{
    tut_text.clear();
}
