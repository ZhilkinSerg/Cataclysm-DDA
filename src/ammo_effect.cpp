#include "ammo_effect.h"

#include "debug.h"
#include "generic_factory.h"
#include "json.h"

namespace
{

generic_factory<ammo_effect> all_ammo_effects( "ammo effects" );

} // namespace

/** @relates int_id */
template<>
bool int_id<ammo_effect>::is_valid() const
{
    return all_ammo_effects.is_valid( *this );
}

/** @relates int_id */
template<>
const ammo_effect &int_id<ammo_effect>::obj() const
{
    return all_ammo_effects.obj( *this );
}

/** @relates int_id */
template<>
const string_id<ammo_effect> &int_id<ammo_effect>::id() const
{
    return all_ammo_effects.convert( *this );
}

/** @relates string_id */
template<>
bool string_id<ammo_effect>::is_valid() const
{
    return all_ammo_effects.is_valid( *this );
}

/** @relates string_id */
template<>
const ammo_effect &string_id<ammo_effect>::obj() const
{
    return all_ammo_effects.obj( *this );
}

/** @relates string_id */
template<>
int_id<ammo_effect> string_id<ammo_effect>::id() const
{
    return all_ammo_effects.convert( *this, ae_null );
}

/** @relates int_id */
template<>
int_id<ammo_effect>::int_id( const string_id<ammo_effect> &id ) : _id( id.id() )
{
}

void ammo_effect::load( JsonObject &jo, const std::string & )
{
    /*
    optional( jo, was_loaded, "legacy_enum_id", legacy_enum_id, -1 );
    JsonArray ja = jo.get_array( "intensity_levels" );
    if( !jo.has_array( "intensity_levels" ) || ja.empty() ) {
        jo.throw_error( "No intensity levels defined for field type", "id" );
    }
    for( size_t i = 0; i < ja.size(); ++i ) {
        field_intensity_level intensity_level;
        field_intensity_level fallback_intensity_level = i > 0 ? intensity_levels[i - 1] : intensity_level;
        JsonObject jao = ja.get_object( i );
        optional( jao, was_loaded, "name", intensity_level.name, fallback_intensity_level.name );
        optional( jao, was_loaded, "sym", intensity_level.symbol, unicode_codepoint_from_symbol_reader,
                  fallback_intensity_level.symbol );
        intensity_level.color = jao.has_member( "color" ) ? color_from_string( jao.get_string( "color" ) ) :
                                fallback_intensity_level.color;
        optional( jao, was_loaded, "transparent", intensity_level.transparent,
                  fallback_intensity_level.transparent );
        optional( jao, was_loaded, "dangerous", intensity_level.dangerous,
                  fallback_intensity_level.dangerous );
        optional( jao, was_loaded, "move_cost", intensity_level.move_cost,
                  fallback_intensity_level.move_cost );
        optional( jao, was_loaded, "extra_radiation_min", intensity_level.extra_radiation_min,
                  fallback_intensity_level.extra_radiation_min );
        optional( jao, was_loaded, "extra_radiation_max", intensity_level.extra_radiation_max,
                  fallback_intensity_level.extra_radiation_max );
        optional( jao, was_loaded, "radiation_hurt_damage_min", intensity_level.radiation_hurt_damage_min,
                  fallback_intensity_level.radiation_hurt_damage_min );
        optional( jao, was_loaded, "radiation_hurt_damage_max", intensity_level.radiation_hurt_damage_max,
                  fallback_intensity_level.radiation_hurt_damage_max );
        optional( jao, was_loaded, "radiation_hurt_message", intensity_level.radiation_hurt_message,
                  fallback_intensity_level.radiation_hurt_message );
        optional( jao, was_loaded, "intensity_upgrade_chance", intensity_level.intensity_upgrade_chance,
                  fallback_intensity_level.intensity_upgrade_chance );
        optional( jao, was_loaded, "intensity_upgrade_duration", intensity_level.intensity_upgrade_duration,
                  fallback_intensity_level.intensity_upgrade_duration );
        optional( jao, was_loaded, "monster_spawn_chance", intensity_level.monster_spawn_chance,
                  fallback_intensity_level.monster_spawn_chance );
        optional( jao, was_loaded, "monster_spawn_count", intensity_level.monster_spawn_count,
                  fallback_intensity_level.monster_spawn_count );
        optional( jao, was_loaded, "monster_spawn_radius", intensity_level.monster_spawn_radius,
                  fallback_intensity_level.monster_spawn_radius );
        optional( jao, was_loaded, "monster_spawn_group", intensity_level.monster_spawn_group,
                  fallback_intensity_level.monster_spawn_group );
        optional( jao, was_loaded, "light_emitted", intensity_level.light_emitted,
                  fallback_intensity_level.light_emitted );
        optional( jao, was_loaded, "translucency", intensity_level.translucency,
                  fallback_intensity_level.translucency );
        optional( jao, was_loaded, "convection_temperature_mod", intensity_level.convection_temperature_mod,
                  fallback_intensity_level.convection_temperature_mod );
        intensity_levels.emplace_back( intensity_level );
    }

    if( jo.has_object( "npc_complain" ) ) {
        JsonObject joc = jo.get_object( "npc_complain" );
        int chance;
        std::string issue;
        time_duration duration;
        std::string speech;
        optional( joc, was_loaded, "chance", chance, 0 );
        optional( joc, was_loaded, "issue", issue );
        optional( joc, was_loaded, "duration", duration, 0_turns );
        optional( joc, was_loaded, "speech", speech );
        npc_complain_data = std::make_tuple( chance, issue, duration, speech );
    }

    JsonObject jid = jo.get_object( "immunity_data" );
    JsonArray jidt = jid.get_array( "traits" );
    while( jidt.has_more() ) {
        immunity_data_traits.emplace_back( trait_id( jidt.next_string() ) );
    }
    JsonArray jidr = jid.get_array( "body_part_env_resistance" );
    while( jidr.has_more() ) {
        JsonArray jao = jidr.next_array();
        immunity_data_body_part_env_resistance.emplace_back( std::make_pair( get_body_part_token(
                    jao.get_string( 0 ) ), jao.get_int( 1 ) ) );
    }
    optional( jo, was_loaded, "underwater_age_speedup", underwater_age_speedup, 0_turns );
    optional( jo, was_loaded, "outdoor_age_speedup", outdoor_age_speedup, 0_turns );
    optional( jo, was_loaded, "decay_amount_factor", decay_amount_factor, 0 );
    optional( jo, was_loaded, "percent_spread", percent_spread, 0 );
    optional( jo, was_loaded, "apply_slime_factor", apply_slime_factor, 0 );
    optional( jo, was_loaded, "gas_absorption_factor", gas_absorption_factor, 0 );
    optional( jo, was_loaded, "is_splattering", is_splattering, false );
    optional( jo, was_loaded, "dirty_transparency_cache", dirty_transparency_cache, false );
    optional( jo, was_loaded, "has_fire", has_fire, false );
    optional( jo, was_loaded, "has_acid", has_acid, false );
    optional( jo, was_loaded, "has_elec", has_elec, false );
    optional( jo, was_loaded, "has_fume", has_fume, false );
    optional( jo, was_loaded, "priority", priority, 0 );
    optional( jo, was_loaded, "half_life", half_life, 0_turns );
    if( jo.has_member( "phase" ) ) {
        phase = jo.get_enum_value<phase_id>( "phase", PNULL );
    }
    optional( jo, was_loaded, "accelerated_decay", accelerated_decay, false );
    optional( jo, was_loaded, "display_items", display_items, true );
    optional( jo, was_loaded, "display_field", display_field, false );
    optional( jo, was_loaded, "wandering_field", wandering_field_id, "fd_null" );
    */
}

void ammo_effect::finalize()
{
}

void ammo_effect::check() const
{
}

size_t ammo_effect::count()
{
    return all_ammo_effects.size();
}

void ammo_effects::load( JsonObject &jo, const std::string &src )
{
    all_ammo_effects.load( jo, src );
}

void ammo_effects::finalize_all()
{
    all_ammo_effects.finalize();
    for( const ammo_effect &ae : all_ammo_effects.get_all() ) {
        const_cast<ammo_effect &>( ae ).finalize();
    }
}

void ammo_effects::check_consistency()
{
    all_ammo_effects.check();
}

void ammo_effects::reset()
{
    all_ammo_effects.reset();
}

const std::vector<ammo_effect> &ammo_effects::get_all()
{
    return all_ammo_effects.get_all();
}

ammo_effect_id ae_null;
