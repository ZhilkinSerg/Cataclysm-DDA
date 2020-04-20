#pragma once
#ifndef CATA_SRC_WEATHER_TYPE_H
#define CATA_SRC_WEATHER_TYPE_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "bodypart.h"
#include "calendar.h"
#include "catacharset.h"
#include "color.h"
#include "effect.h"
#include "enums.h"
#include "mapdata.h"
#include "translations.h"
#include "type_id.h"

class JsonObject;

/**
 * Weather animation class.
 */
struct weather_animation_t {
    float    factor;
    nc_color color;
    char     glyph;
};

struct weather_type {
    public:
        void load( const JsonObject &jo, const std::string &src );
        void finalize();
        void check() const;

    public:
        // Used by generic_factory
        weather_type_str_id id;
        bool was_loaded = false;

        weather_animation_t animation;

        translation name;
        uint32_t symbol;
        nc_color color;
        std::string looks_like;

    public:
        int legacy_enum_id = -1;

    public:
        std::string get_name( int level = 0 ) const {
            return name.translated();
        }
        uint32_t get_codepoint( int level = 0 ) const {
            return symbol;
        }
        std::string get_symbol( int level = 0 ) const {
            return utf32_to_utf8( symbol );
        }
        nc_color get_color( int level = 0 ) const {
            return color;
        }

        static size_t count();
};

namespace weather_types
{

void load( const JsonObject &jo, const std::string &src );
void finalize_all();
void check_consistency();
void reset();

const std::vector<weather_type> &get_all();
void set_weather_type_ids();
weather_type get_weather_type_by_legacy_enum( int legacy_enum_id );

} // namespace weather_types

extern weather_type_id weather_null,
       weather_clear,
       weather_sunny,
       weather_cloudy,
       weather_light_drizzle,
       weather_drizzle,
       weather_rainy,
       weather_thunder,
       weather_lightning,
       weather_acid_drizzle,
       weather_acid_rain,
       weather_flurries,
       weather_snow,
       weather_snowstorm
       ;

#endif // CATA_SRC_WEATHER_TYPE_H
