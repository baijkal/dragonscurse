#include <string.h>
#include <string>
#include "SDL.h"
#include "statusbar.h"

int Statusbar::c_height = 40;

Statusbar::Statusbar(Status *status, MediaDB *media)
    : m_status(status)
{
    m_gold_label = new Text("Wonderfull_18", media);
    m_gold_label->add_line("Gold");

    m_gold_text = new Text("Wonderfull_18", media);
    m_gold_text->add_line("000000");
}

void Statusbar::draw(SDL_Surface *surface, int screen_width, int screen_height)
{
    SDL_Rect dest_rect;

    dest_rect.x = 0;
    dest_rect.y = 0;
    dest_rect.w = screen_width;
    dest_rect.h = c_height - 2;
    SDL_FillRect(surface, &dest_rect, 0x77777777);

    dest_rect.x = 0;
    dest_rect.y = c_height - 2;
    dest_rect.w = screen_width;
    dest_rect.h = 2;
    SDL_FillRect(surface, &dest_rect, 0x55555555);

    static char str[6];
    sprintf(str, "%06d", m_status->get_gold());
    m_gold_text->replace_line(str);
    m_gold_label->draw(surface, screen_width - 120, 0,
                       0, 0, screen_width, screen_height);
    m_gold_text->draw(surface, screen_width - 80, c_height - 20,
                      0, 0, screen_width, screen_height);
}

