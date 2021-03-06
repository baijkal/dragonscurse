#ifndef _Material_H
#define _Material_H

#include "phoboz/timer.h"
#include "body.h"

class Material : public Body {
public:
    virtual void move(Map *map);

    void set_from_chest(bool value) { m_from_chest = value; }

protected:
    Material(Type type, const char *fn, MediaDB *media, int x, int y);

private:
    enum Action { Rise, Fall, Still };

    void animate_move();

    bool m_from_chest;
    Action m_action;
    Timer m_rise_timer;
    Timer m_anim_timer;
    bool m_ref_done;
};

#endif

