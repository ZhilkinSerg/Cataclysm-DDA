#pragma once
#ifndef MOVE_MODE_H
#define MOVE_MODE_H

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>

#include "type_id.h"
#include "string_id.h"
#include "translations.h"

class JsonObject;

struct move_mode {
    public:
        void load( const JsonObject &jo, const std::string &src );
        void finalize();
        void check() const;

    public:
        // Used by generic_factory
        move_mode_str_id id;
        bool was_loaded = false;

    public:
        translation name;
        translation description;
        float speed_modifier = 1.0f;
        int stamina_spent_per_second = 0;

        static size_t count();
};

namespace move_modes
{

void load( const JsonObject &jo, const std::string &src );
void finalize_all();
void check_consistency();
void reset();

const std::vector<move_mode> &get_all();
void set_move_mode_ids();

} // namespace move_modes

extern move_mode_id MM_NULL,
       MM_WALK,
       MM_RUN,
       MM_CROUCH
       ;

#endif // MOVE_MODE_H
