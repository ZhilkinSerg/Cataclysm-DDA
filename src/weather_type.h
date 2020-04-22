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
 * Weather type enum.
 */
enum legacy_weather_type : int {
    WEATHER_NULL,         //!< For data and stuff
    WEATHER_CLEAR,        //!< No effects
    WEATHER_SUNNY,        //!< Glare if no eye protection
    WEATHER_CLOUDY,       //!< No effects
    WEATHER_LIGHT_DRIZZLE,//!< very Light rain
    WEATHER_DRIZZLE,      //!< Light rain
    WEATHER_RAINY,        //!< Lots of rain, sight penalties
    WEATHER_THUNDER,      //!< Warns of lightning to come
    WEATHER_LIGHTNING,    //!< Rare lightning strikes!
    WEATHER_ACID_DRIZZLE, //!< No real effects; warning of acid rain
    WEATHER_ACID_RAIN,    //!< Minor acid damage
    WEATHER_FLURRIES,     //!< Light snow
    WEATHER_SNOW,         //!< snow glare effects
    WEATHER_SNOWSTORM,    //!< sight penalties
    NUM_WEATHER_TYPES     //!< Sentinel value
};

/**
 * Weather animation class.
 */
struct weather_animation_t {
    float    factor;
    nc_color color;
    char     glyph;
};

weather_animation_t get_weather_animation( legacy_weather_type const type )
{
    static const std::map<legacy_weather_type, weather_animation_t> map {
        {WEATHER_ACID_DRIZZLE, weather_animation_t {0.01f, c_light_green, '.'}},
        {WEATHER_ACID_RAIN,    weather_animation_t {0.02f, c_light_green, ','}},
        {WEATHER_LIGHT_DRIZZLE, weather_animation_t{0.01f, c_light_blue, ','}},
        {WEATHER_DRIZZLE,      weather_animation_t {0.01f, c_light_blue,  '.'}},
        {WEATHER_RAINY,        weather_animation_t {0.02f, c_light_blue,  ','}},
        {WEATHER_THUNDER,      weather_animation_t {0.02f, c_light_blue,  '.'}},
        {WEATHER_LIGHTNING,    weather_animation_t {0.04f, c_light_blue,  ','}},
        {WEATHER_FLURRIES,     weather_animation_t {0.01f, c_white,   '.'}},
        {WEATHER_SNOW,         weather_animation_t {0.02f, c_white,   ','}},
        {WEATHER_SNOWSTORM,    weather_animation_t {0.04f, c_white,   '*'}}
    };

    const auto it = map.find( type );
    if( it != std::end( map ) ) {
        return it->second;
    }

    return {0.0f, c_white, '?'};
}

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

        /*
        translation name;
        uint32_t symbol;
        nc_color color;
        std::string looks_like;
        */

    public:
        int legacy_enum_id = -1;

    public:
        /*
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
        */

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
