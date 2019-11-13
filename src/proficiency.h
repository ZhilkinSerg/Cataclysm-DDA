#pragma once
#ifndef PROFICIENCY_H
#define PROFICIENCY_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "skill.h"
#include "translations.h"
#include "type_id.h"

class JsonObject;

class proficiency
{
    public:
        proficiency_id id;
        bool was_loaded = false;

        void load( JsonObject &jo, const std::string &src );
        void check() const;
        void finalize();

    private:
        translation _name;
        translation _description;
        /*
            This much time needs to be spent practicing related activity to achieve proficiency.
        */
        time_duration _time_to_achieve;
        /*
            First parameter in a pair is minimum level of a corresponding skill when you can start practicing proficiency activity.
            Second parameter in a pair is minimum level of a corresponding skill that allows to achieve proficiency.

            You cannot even attempt to practice proficiency activity until minimum level in each of the required skills is reached.
            I.e., you just don't know where to start.

            You cannot achieve proficiency until minimum level in each of the required skills is reached.
        */
        std::unordered_map<skill_id, std::pair<int, int>> _required_skills;
        /*
            You cannot even attempt to start practicing proficiency activity until each of the required proficiencies are learned.
        */
        std::unordered_set<proficiency_id>  _required_proficiencies;
        /*
            Experience towards a related skill will be multiplied by this factor if you know this proficiency.
        */
        std::unordered_map<skill_id, double> _related_skills;
        /*
            Experience towards a related proficiency will be multiplied by this factor if you know this proficiency.
            E.g. knowing `lockpicking` makes you learn `hotwiring` 5% faster.
            While `lockpicking` and `hotwiring` share very little, some proficiencies (such as `canning` and `pickling`)
            might share a lot, so if you know how to can food you're much better abled to work your way through pickling on your own.
        */
        std::unordered_map<proficiency_id, double> _related_proficiencies;

    public:
        std::string name() const {
            return _name.translated();
        }
        std::string description() const {
            return _description.translated();
        }
        std::unordered_map<skill_id, std::pair<int, int>> required_skills() const {
            return _required_skills;
        }
        std::unordered_set<proficiency_id> required_proficiencies() const {
            return _required_proficiencies;
        }
        std::unordered_map<skill_id, double> related_skills() const {
            return _related_skills;
        }
        std::unordered_map<proficiency_id, double> related_proficiencies() const {
            return _related_proficiencies;
        }
};

namespace proficiencies
{

void load( JsonObject &jo, const std::string &src );
void check_consistency();
void reset();
void finalize();

} // namespace proficiencies

#endif // PROFICIENCY_H
