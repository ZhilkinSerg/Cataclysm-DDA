#include "move_mode.h"

#include "debug.h"
#include "generic_factory.h"
#include "json.h"
#include "int_id.h"

namespace
{

generic_factory<move_mode> all_move_modes( "move modes" );

} // namespace

/** @relates int_id */
template<>
bool int_id<move_mode>::is_valid() const
{
    return all_move_modes.is_valid( *this );
}

/** @relates int_id */
template<>
const move_mode &int_id<move_mode>::obj() const
{
    return all_move_modes.obj( *this );
}

/** @relates int_id */
template<>
const string_id<move_mode> &int_id<move_mode>::id() const
{
    return all_move_modes.convert( *this );
}

/** @relates string_id */
template<>
bool string_id<move_mode>::is_valid() const
{
    return all_move_modes.is_valid( *this );
}

/** @relates string_id */
template<>
const move_mode &string_id<move_mode>::obj() const
{
    return all_move_modes.obj( *this );
}

/** @relates string_id */
template<>
int_id<move_mode> string_id<move_mode>::id() const
{
    return all_move_modes.convert( *this, MM_NULL );
}

/** @relates int_id */
template<>
int_id<move_mode>::int_id( const string_id<move_mode> &id ) : _id( id.id() )
{
}

void move_mode::load( const JsonObject &jo, const std::string & )
{
    mandatory( jo, was_loaded, "name", name );
    mandatory( jo, was_loaded, "description", description );
    optional( jo, was_loaded, "speed_modifier", speed_modifier, 1.0f );
    optional( jo, was_loaded, "stamina_spent_per_second", stamina_spent_per_second, 0 );
}

void move_mode::finalize()
{
}

void move_mode::check() const
{
}

size_t move_mode::count()
{
    return all_move_modes.size();
}

void move_modes::load( const JsonObject &jo, const std::string &src )
{
    all_move_modes.load( jo, src );
}

void move_modes::finalize_all()
{
    set_move_mode_ids();
    all_move_modes.finalize();
    for( const move_mode &mm : all_move_modes.get_all() ) {
        const_cast<move_mode &>( mm ).finalize();
    }
}

void move_modes::check_consistency()
{
    all_move_modes.check();
}

void move_modes::reset()
{
    all_move_modes.reset();
}

const std::vector<move_mode> &move_modes::get_all()
{
    return all_move_modes.get_all();
}

move_mode_id MM_NULL,
             MM_WALK,
             MM_RUN,
             MM_CROUCH
             ;

void move_modes::set_move_mode_ids()
{
    MM_NULL = move_mode_id( "MM_NULL" );
    MM_WALK = move_mode_id( "MM_WALK" );
    MM_RUN = move_mode_id( "MM_RUN" );
    MM_CROUCH = move_mode_id( "MM_CROUCH" );
}
