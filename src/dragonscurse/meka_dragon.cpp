#include <stdlib.h>
#include "meka_dragon.h"

MekaDragon::MekaDragon(const char *fn, int x, int y, Direction dir)
    : Monster(fn, x, y, dir)
{
    m_bullet = new Bullet(get_string("bullet"));
    if (!m_bullet->get_loaded()) {
        exit(1);
    }
}

void MekaDragon::move(Map *map)
{
    Monster::move(map);

    // Check ground
    check_ground(map);

    if (m_hit == HitOne) {
        if (m_hit_timer.expired(get_attribute("hit_time"))) {
            m_dx = 0;
            reset_hit();
        }
        else {
            // Move backwards
            m_dx = get_attribute("move_speed");

            // Check for collision with map
            check_behind(map);

            // Move
            if (m_dir == Right) {
                m_x -= m_dx;
            }
            else if (m_dir == Left) {
                m_x += m_dx;
            }
        }
    }

    switch(m_action)
    {
        case Fall:
            check_ahead(map);

            // Move
            if (m_dir == Right) {
                m_x += m_dx;
            }
            else if (m_dir == Left) {
                m_x -= m_dx;
            }

            check_below(map);
            m_y += m_dy;
            break;

        default:
            break;
    }
}

