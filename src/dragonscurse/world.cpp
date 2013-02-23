#include <iostream>
#include <vector>
#include <string>
#include "Tmx/Tmx.h"
#include "object_factory.h"
#include "player.h"
#include "world.h"

World::World(Map *map, int object_group)
    : m_map(map)
{
    const Tmx::ObjectGroup *group = map->get_object_group(object_group);
    int n = group->GetNumObjects();
    for (int i = 0; i < n; i++) {
        const Tmx::Object *obj = group->GetObject(i);
        const Tmx::PropertySet prop = obj->GetProperties();
        std::string dirname = prop.GetLiteralProperty(std::string("direction"));
        Object *object = ObjectFactory::create_object(obj->GetName().c_str(),
                                                      obj->GetType().c_str(),
                                                      obj->GetX(), obj->GetY(),
                                                      dirname.c_str());
        if (object) {
            m_objects.push_back(object);
        }
        else {
            std::cerr << "Warning - Unable to load object: " << i << std::endl;
        }
    }
}

void World::move(Player *player,
                 int clip_x, int clip_y, int clip_w, int clip_h)
{
    player->move(m_map);
    int window_width = clip_w - clip_x;
    int window_height = clip_h - clip_y;
    int map_x = player->get_x() - window_width / 2;
    int map_y = player->get_y() - window_height / 2;
    if (map_x < 0) {
        map_x = 0;
    }
    if (map_y < 0) {
        map_y = 0;
    }
    m_map->set_x(map_x);
    m_map->set_y(map_y);

    std::vector<Object*> perished;

    for (std::list<Object*>::iterator it = m_objects.begin();
         it != m_objects.end();
         ++it) {
        Object *object = *it;
        if (object->get_visible(m_map, clip_x, clip_y, clip_w, clip_h)) {

            // Move object
            object->move(m_map);

            // Handle actor object
            if (object->get_type() == Object::TypeEnemy) {
                Actor *actor = (Actor *) object;
                actor->set_reference(player->get_front(), player->get_bottom());
                if (player->check_collision(actor)) {
                    if (!actor->get_invisible()) {
                        player->set_hit(actor);
                    }
                }
                if (player->attack_actor(actor)) {
                    actor->set_hit(player);
                }
                if (actor->get_action() == Actor::Perished) {
                    perished.push_back(*it);
                }
            }
        }
    }

    // Remove all perished objects
    for (int i = 0; i < perished.size(); i++) {
        m_objects.remove(perished[i]);
    }
}

void World::draw(SDL_Surface *dest, Player *player,
                 int clip_x, int clip_y, int clip_w, int clip_h)
{
    int num_layers = m_map->get_num_layers();
    for (int i = 0; i < num_layers; i++) {
        m_map->draw_layer(dest, clip_x, clip_y, clip_w, clip_h, 0);
    }

    for (std::list<Object*>::iterator it = m_objects.begin();
         it != m_objects.end();
         ++it) {
        Object *object = *it;
        object->draw(dest, m_map, clip_x, clip_y, clip_w, clip_h);
    }

    player->draw(dest, m_map, clip_x, clip_y, clip_w, clip_h);
}

