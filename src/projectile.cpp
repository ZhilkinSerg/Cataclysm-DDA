#include "projectile.h"

#include <memory>
#include <utility>

#include "explosion.h"
#include "field.h"
#include "game.h"
#include "item.h"
#include "map.h"
#include "map_iterator.h"
#include "rng.h"

projectile::projectile() :
    speed( 0 ), range( 0 ), drop( nullptr ), custom_explosion( nullptr )
{ }

projectile::~projectile() = default;

projectile::projectile( projectile && ) = default;

projectile::projectile( const projectile &other )
{
    *this = other;
}

projectile &projectile::operator=( const projectile &other )
{
    impact = other.impact;
    speed = other.speed;
    range = other.range;
    proj_effects = other.proj_effects;
    set_drop( other.get_drop() );
    set_custom_explosion( other.get_custom_explosion() );

    return *this;
}

const item &projectile::get_drop() const
{
    if( drop != nullptr ) {
        return *drop;
    }

    static const item null_drop;
    return null_drop;
}

void projectile::set_drop( const item &it )
{
    if( it.is_null() ) {
        unset_drop();
    } else {
        drop = std::make_unique<item>( it );
    }
}

void projectile::set_drop( item &&it )
{
    if( it.is_null() ) {
        unset_drop();
    } else {
        drop = std::make_unique<item>( std::move( it ) );
    }
}

void projectile::unset_drop()
{
    drop.reset( nullptr );
}

const explosion_data &projectile::get_custom_explosion() const
{
    if( custom_explosion != nullptr ) {
        return *custom_explosion;
    }

    static const explosion_data null_explosion{};
    return null_explosion;
}

void projectile::set_custom_explosion( const explosion_data &ex )
{
    custom_explosion = std::make_unique<explosion_data>( ex );
}

void projectile::unset_custom_explosion()
{
    custom_explosion.reset();
}

struct ammo_effect_data {

    std::string effect_tag;

    field_type_id aoe_field_type = fd_null;
    int aoe_intensity_min = 0;
    int aoe_intensity_max = 0;
    int aoe_radius = 0;
    int aoe_radius_z = 0;
    int aoe_chance = 0;
    int aoe_size = 0;
    explosion_data aoe_explosion_data;
    bool aoe_check_passable = false;
    bool aoe_check_sees = false;
    int aoe_check_sees_radius = 0;
    bool do_flashbang = false;
    bool do_emp_blast = false;

    ammo_effect_data( const std::string &tag, const field_type_id &aoe_field,
                      int aoe_intensity_min, int aoe_intensity_max, int aoe_radius, int aoe_radius_z,
                      int aoe_chance, int aoe_size, explosion_data aoe_explosion_data = {},
                      bool aoe_check_passable = false, bool aoe_check_sees = false, int aoe_check_sees_radius = 0,
                      bool do_flashbang = false, bool do_emp_blast = false )
        : effect_tag( tag )
        , aoe_field_type( aoe_field )
        , aoe_intensity_min( aoe_intensity_min )
        , aoe_intensity_max( aoe_intensity_max )
        , aoe_radius( aoe_radius )
        , aoe_radius_z( aoe_radius_z )
        , aoe_chance( aoe_chance )
        , aoe_size( aoe_size )
        , aoe_explosion_data( aoe_explosion_data )
        , aoe_check_passable( aoe_check_passable )
        , aoe_check_sees( aoe_check_sees )
        , aoe_check_sees_radius( aoe_check_sees_radius )
        , do_flashbang( do_flashbang )
        , do_emp_blast( do_emp_blast ) {
    }
};

static const std::vector<ammo_effect_data> ammo_effects = {
    { ammo_effect_data( "NAPALM", fd_fire, 1, 1, 1, 0, 1, 3, explosion_data( 60, 0.7, true ) ) },
    { ammo_effect_data( "NAPALM_BIG", fd_fire, 1, 1, 3, 0, 1, 4, explosion_data( 360, 0.8, true ) ) },
    { ammo_effect_data( "PYROPHORIC", fd_fire, 2, 2, 3, 0, 1, 0, explosion_data( 360, 0.8, true ) ) },
    { ammo_effect_data( "ACIDBOMB", fd_acid, 3, 3, 1, 0, 1, 0 ) },
    { ammo_effect_data( "TOXICGAS", fd_toxic_gas, 3, 3, 1, 0, 1, 0 ) },
    { ammo_effect_data( "GAS_FUNGICIDAL", fd_fungicidal_gas, 3, 3, 1, 0, 1, 0 ) },
    { ammo_effect_data( "GAS_INSECTICIDAL", fd_insecticidal_gas, 3, 3, 1, 0, 1, 0 ) },
    { ammo_effect_data( "SMOKE", fd_smoke, 3, 3, 1, 0, 1, 0 ) },
    { ammo_effect_data( "SMOKE_BIG", fd_smoke, 3, 3, 6, 0, 1, 0 ) },
    { ammo_effect_data( "FLARE", fd_fire, 1, 1, 1, 0, 1, 0 ) },
    { ammo_effect_data( "LIGHTNING", fd_electricity, 3, 3, 1, 0, 1, 0 ) },
    { ammo_effect_data( "PLASMA", fd_plasma,  2, 3, 1, 0, 2, 0 ) },
    { ammo_effect_data( "EXPLOSIVE_HUGE", fd_null, 0, 0, 1, 0, 1, 4, explosion_data( 1200 ) ) },
    { ammo_effect_data( "EXPLOSIVE_BIG", fd_null, 0, 0, 1, 0, 1, 3, explosion_data( 600 ) ) },
    { ammo_effect_data( "EXPLOSIVE", fd_null, 0, 0, 1, 0, 1, 2, explosion_data( 360 ) ) },
    { ammo_effect_data( "EXPLOSIVE_SMALL", fd_null, 0, 0, 1, 0, 1, 2, explosion_data( 360, 0.4 ) ) },
    { ammo_effect_data( "FRAG", fd_null, 0, 0, 0, 0, 0, 15, explosion_data( 185, 0.8, false, shrapnel_data( 212, 0.05 ) ) ) },
    { ammo_effect_data( "FLAME", fd_null, 0, 0, 0, 0, 0, 0 ) },
    //TODO: sees & passable check
    { ammo_effect_data( "MININUKE_MOD", fd_nuke_gas, 3, 3, 18, 0, 1, 1, explosion_data( 72000000 ), true, true, 3 ) },
    { ammo_effect_data( "FLASHBANG", fd_null, 0, 0, 0, 0, 0, 0, explosion_data(), false, false, 0, true ) },
    { ammo_effect_data( "EMP", fd_null, 0, 0, 0, 0, 0, 0, explosion_data( ), false, false, 0, false, true ) },
};

void apply_ammo_effects( const tripoint &p, const std::set<std::string> &effects )
{
    for( const ammo_effect_data &aed : ammo_effects ) {
        if( effects.count( aed.effect_tag ) > 0 ) {
            for( auto &pt : g->m.points_in_radius( p, aed.aoe_radius, aed.aoe_radius_z ) ) {
                if( one_in( aed.aoe_chance ) ) {
                    const bool check_sees = !aed.aoe_check_sees ||
                                            ( aed.aoe_check_sees && g->m.sees( p, pt, aed.aoe_check_sees_radius ) );
                    const bool check_passable = !aed.aoe_check_passable ||
                                                ( aed.aoe_check_passable && g->m.passable( pt ) );
                    if( check_sees && check_passable ) {
                        g->m.add_field( pt, aed.aoe_field_type, rng( aed.aoe_intensity_min, aed.aoe_intensity_max ) );
                    }
                }
            }
        }
        if( aed.aoe_explosion_data.power > 0 ) {
            explosion_handler::explosion( p, aed.aoe_explosion_data );
        }
        if( aed.do_flashbang ) {
            explosion_handler::flashbang( p );
        }
        if( aed.do_emp_blast ) {
            explosion_handler::emp_blast( p );
        }
    }

    if( effects.count( "NO_BOOM" ) == 0 && effects.count( "FLAME" ) > 0 ) {
        for( auto &pt : g->m.points_in_radius( p, 1, 0 ) ) {
            g->m.add_field( pt, fd_fire, 1 );
        }
    }
}

int max_aoe_size( const std::set<std::string> &tags )
{
    int aoe_size = 0;
    for( const ammo_effect_data &aed : ammo_effects ) {
        if( tags.count( aed.effect_tag ) > 0 ) {
            aoe_size = std::max( aoe_size,  aed.aoe_size ) ;
        }
    }
    return aoe_size;
}
