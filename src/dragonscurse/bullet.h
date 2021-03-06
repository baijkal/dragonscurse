#ifndef _Bullet_H
#define _Bullet_H

#include "phoboz/timer.h"
#include "body.h"

class Bullet : public Body {
public:
    bool get_ready() const { return m_ready; }
    bool get_active() const { return m_active; }

    virtual void set_dir(Direction dir);
    void set_hit_one(bool value) { m_hit_one = value; }
    virtual bool fire(int x, int y, int dx, int dy) = 0;

    bool hit_object(Object *object);

    virtual void move(Map *map);
    virtual void draw(SDL_Surface *dest, Map *map,
                      int clip_x, int clip_y, int clip_w, int clip_h) {
        if (m_active) Object::draw(dest, map, clip_x, clip_y, clip_w, clip_h);
    }

protected:
    Bullet(const char *fn, MediaDB *media);

    void reload(bool active);

private:
    void animate_move();

    bool m_hit_one;
    bool m_ready;
    bool m_active;
    bool m_animated;
    int m_distance;
    Timer m_reload_timer;
    Timer m_anim_timer;
};

#endif

