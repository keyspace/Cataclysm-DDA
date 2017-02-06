#include "player.h" // TODO: character.h?

#include "messages.h"
#include "rng.h"
#include "skill.h"

#include "catalua.h"

// TODO: could move experience-related stuff here, such as player::practice(), etc

// TODO: should be Character, not player, no? How do NPCs train their skills?
void player::dwell() {
    // TODO: early exit if conditions unfavorable

    // copy-paste from player_display.cpp, player::disp_info()
    const auto candwellon = Skill::get_skills_sorted_by( [&]( Skill const & a, Skill const & b ) {
            // TODO: save SkillLevel instead for the below TODO
            int const ep_a = get_skill_level( a.ident() ).exercise_pending();
            int const ep_b = get_skill_level( b.ident() ).exercise_pending();
            // TODO: check for more recently practiced
            return ep_a > ep_b || ( ep_a == ep_b && a.name() < b.name() );
    } );

    if( candwellon.empty() ) {
        debugmsg( "unexpected empty skill list" );
        return;
    }

    // @magicnum 3.0f - choose what to dwell upon out of the top three
    const int index = std::floor( std::abs( rng_normal( -3.0f, 3.0f ) ) );
    const skill_id id = candwellon.at( index )->ident();

    if( get_skill_level( id ).exercise_pending() == 0 ) {
        return;
    }

    // TODO: customise based on morale (brood/dwell/ponder)
    add_msg_if_player( "You dwell upon your recent experiences." );

    // TODO: too many invocations of the same, can do better
    int oldLevel = get_skill_level( id );
    ///\EFFECT_INT speeds up internalising experience from both practice and book reading
    get_skill_level( id ).dwell( get_int() * 4 );
    int newLevel = get_skill_level( id );
    if ( is_player() && newLevel > oldLevel ) {
        add_msg_if_player( m_good, _( "Your skill in %s has increased to %d!" ),
                           id.obj().name().c_str(), newLevel );
        lua_callback( "on_skill_increased" );
    }

    return;
}
