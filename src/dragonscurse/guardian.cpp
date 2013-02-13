#include "guardian.h"

void Guardian::move(Map *map)
{
    face_reference();

    // Check ground
    if (m_action != Jump && m_action != Hit) {
        check_ground(map);
    }

    switch(m_action) {
        case Move:
            animate_move();
            break;

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
            set_move_dir(Keep);
            break;
    }

}

