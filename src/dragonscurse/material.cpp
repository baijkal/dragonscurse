#include <stdlib.h>
#include "material.h"

Material::Material(Type type, const char *fn, MediaDB *media, int x, int y)
    : Body(type, x, y),
      m_from_chest(false),
      m_action(Rise),
      m_ref_done(false)
{
    load(fn, media);

    m_frame = get_attribute("move_start");
}

void Material::animate_move()
{
    if (m_anim_timer.expired(get_attribute("treshold"))) {
        m_anim_timer.reset();
        if (++m_frame > get_attribute("move_end")) {
            m_frame = get_attribute("move_start");
        }
    }
}

void Material::move(Map *map)
{
    if (!m_ref_done) {
        int dx;
        if (m_from_chest) {
            dx = get_attribute("chest_speed");
        }
        else {
            dx = get_attribute("move_speed");
        }
        if (get_reference() == Right) {
            set_speed(dx, -get_attribute("rise_speed"));
        }
        else {
            set_speed(-dx, -get_attribute("rise_speed"));
        }
        set_accelration(0, get_attribute("weight"));
        m_ref_done = true;
    }

    Body::move(map);
    if (get_moving()) {
        animate_move();
    }
    else {
        m_frame = get_attribute("move_still");
    }
}

