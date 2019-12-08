#pragma once
#ifndef GAMEMODE_MA_H
#define GAMEMODE_MA_H

#include "gamemode.h"

struct tripoint;

enum special_game_id : int;
enum action_id : int;

struct ma_game : public special_game {
        special_game_id id() override {
            return SGAME_MA;
        }
        bool init() override;
        void per_turn() override { }
        void pre_action( action_id &act ) override {
            ( void )act;
        }
        void post_action( action_id act ) override {
            ( void )act;
        }
        void game_over() override { }
        bool generate_overmap( const tripoint &om_pos ) override;
        bool place_cities( const tripoint &om_pos ) override;
    private:
        const std::string MA_database_path = R"(C:\Projects\Cataclysm-DDA\mods\MA\MA.sqlite3)";
};

#endif // GAMEMODE_MA_H
