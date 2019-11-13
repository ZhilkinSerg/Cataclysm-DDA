#include "proficiency.h"

#include "debug.h"
#include "generic_factory.h"
#include "json.h"

namespace
{

generic_factory<proficiency> all_proficiencies( "proficiencies" );

} // namespace

template<>
bool string_id<proficiency>::is_valid() const
{
    return all_proficiencies.is_valid( *this );
}

template<>
const proficiency &string_id<proficiency>::obj() const
{
    return all_proficiencies.obj( *this );
}

void proficiency::load( JsonObject &jo, const std::string & )
{
    optional( jo, was_loaded, "name", _name );
    optional( jo, was_loaded, "description", _description );

    if( jo.has_object( "required" ) ) {
        JsonObject jor = jo.get_object( "required" );
        JsonArray jars = jor.get_array( "skills" );
        while( jars.has_more() ) {
            JsonObject jors = jars.next_object();
            skill_id _skill_id = skill_id( jors.get_string( "id" ) );
            int _skill_level_min =  jors.get_int( "level_min" );
            int _skill_level_learn =  jors.get_int( "level_learn" );
            _required_skills.emplace( std::make_pair( _skill_id,
                                      std::make_pair( _skill_level_min, _skill_level_learn ) ) );
        }
        optional( jor, was_loaded, "proficiencies", _required_proficiencies );
    }

    if( jo.has_object( "related" ) ) {
        JsonObject jor = jo.get_object( "related" );
        JsonArray jars = jor.get_array( "skills" );
        while( jars.has_more() ) {
            JsonObject jors = jars.next_object();
            skill_id _skill_id = skill_id( jors.get_string( "id" ) );
            double _xp_mult = jors.get_float( "xp_mult" );
            _related_skills.emplace( std::make_pair( _skill_id, _xp_mult ) );
        }
        JsonArray jarp = jor.get_array( "proficiencies" );
        while( jarp.has_more() ) {
            JsonObject jorp = jarp.next_object();
            proficiency_id _proficiency_id = proficiency_id( jorp.get_string( "id" ) );
            double _xp_mult = jorp.get_float( "xp_mult" );
            _related_proficiencies.emplace( std::make_pair( _proficiency_id, _xp_mult ) );
        }
    }
}

void proficiency::check() const
{
    // TODO: Check skills validity?
}

void proficiency::finalize()
{
    // TODO: Check skills validity?
}

void proficiencies::load( JsonObject &jo, const std::string &src )
{
    all_proficiencies.load( jo, src );
}

void proficiencies::check_consistency()
{
    all_proficiencies.check();
}

void proficiencies::reset()
{
    all_proficiencies.reset();
}

void proficiencies::finalize()
{
    all_proficiencies.finalize();
}
