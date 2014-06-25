#include <string.h>
#include "phoboz/ctrl.h"
#include "actor.h"
#include "world.h"
#include "area.h"

Area::Area(const char *name, const char *type, int x, int y, int w, int h)
    : Object(Object::TypeArea, x, y),
      m_name(name),
      m_h(h), m_w(w),
      m_state(StateIdle)
{
    if (strcmp(type, "warp") == 0) {
        m_type = TypeWarp;
    }
    else if (strcmp(type, "travel") == 0) {
        m_type = TypeTravel;
    }
    else {
        m_type = TypeUser;
        load(type);
    }

    m_frame = get_attribute("open_start");
}

void Area::world_initialize(World *world)
{
    int lock_id = get_attribute("lock_id");

    if (lock_id) {
        WorldDB *db = world->get_db();
        int key;
        const char *type = db->get_lock_type(&key,
                                             lock_id, world->get_filename());
        if (type) {
            m_state = StateLocked;
            if (strcmp(type, "small_key.xml") == 0) { 
                m_frame = get_attribute("small_key");
            }
            else if (strcmp(type, "large_key.xml") == 0) {
                m_frame = get_attribute("large_key");
            }
            m_world_key = key;
            m_lock_type = std::string(type);
        }
    }
}

bool Area::is_locked() const
{
    bool result = false;

    if (m_state == StateLocked) {
        result = true;
    }

    return result;
}

bool Area::inside(Actor *actor)
{
    bool result = false;
    int x = actor->get_front();
    int y = actor->get_bottom();

    // Check if inside
    if (x >= m_x && x <= m_x + m_w &&
        y >= m_y && y <= m_y + m_h) {
        if (m_type == TypeUser) {
            if (m_state == StateOpen) {
                result = true;
                m_state = StateIdle;
            }
        }
        else {
            result = true;
        }
    }

    return result;
}

void Area::move(Map *map)
{
    if (m_type == TypeUser) {
        if (m_state == StateIdle) {
            int input = get_input();
            if (input & PRESS_UP) {
                if (m_open_timer.expired(get_attribute("open_time"))) {
                    m_open_timer.reset();
                    m_state = StateOpening;
                }
            }
            else {
                m_open_timer.reset();
            }
        }
        else if (m_state == StateOpening) {
            if (m_anim_timer.expired(get_attribute("treshold"))) {
                if (++m_frame > get_attribute("open_end")) {
                    m_anim_timer.reset();
                    m_frame = get_attribute("open_start");
                    m_state = StateOpen;
                }
            }
        }
    }
}

bool Area::move_unlock(World *world)
{
    bool result = false;

    if (m_type == TypeUser) {
        if (m_state == StateLocked) {
            int input = get_input();
            if (input & PRESS_UP) {
                if (m_open_timer.expired(get_attribute("open_time"))) {
                    m_open_timer.reset();

                    // Unlock in database
                    WorldDB *db = world->get_db();
                    db->remove(m_world_key);

                    // Mark as unlocked
                    m_state = StateIdle;
                    m_frame = get_attribute("open_start");
                    result = true;
                }
            }
            else {
                m_open_timer.reset();
            }
        }
    }

    return result;
}

void Area::draw(SDL_Surface *dest, Map *map,
                int clip_x, int clip_y, int clip_w, int clip_h)
{
    if (m_type == TypeUser) {
        Object::draw(dest, map, clip_x, clip_y, clip_w, clip_h);
    }
}
