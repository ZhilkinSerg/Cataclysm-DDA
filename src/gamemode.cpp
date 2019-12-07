#include "gamemode.h"
#include "gamemode_defense.h"
#include "gamemode_tutorial.h"
#include "gamemode_MA.h"

#include "debug.h"
#include "translations.h"

std::string special_game_name( special_game_id id )
{
    switch( id ) {
        case SGAME_NULL:
        case NUM_SPECIAL_GAMES:
            return "Bug (gamemode.cpp:special_game_name)";
        case SGAME_TUTORIAL:
            return _( "Tutorial" );
        case SGAME_DEFENSE:
            return _( "Defense" );
        case SGAME_MA:
            return _( "Massachusetts" );
        default:
            return "Uncoded (gamemode.cpp:special_game_name)";
    }
}

std::unique_ptr<special_game> get_special_game( special_game_id id )
{
    std::unique_ptr<special_game> ret;
    switch( id ) {
        case SGAME_NULL:
            ret = std::make_unique<special_game>();
            break;
        case SGAME_TUTORIAL:
            ret = std::make_unique<tutorial_game>();
            break;
        case SGAME_DEFENSE:
            ret = std::make_unique<defense_game>();
            break;
        case SGAME_MA:
            ret = std::make_unique<ma_game>();
            break;
        default:
            debugmsg( "Missing something in gamemode.cpp:get_special_game()?" );
            ret = std::make_unique<special_game>();
            break;
    }

    return ret;
}
