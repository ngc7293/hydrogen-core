#ifndef DRAWER_H_
#define DRAWER_H_

#include "object.h"

#include <vector>

#include "sprite.h"
#include "segment.h"

class Drawer: public hc::Object {
public:
    static const int TYPE_ID = 10;

private:
    std::vector<hc::Segment*> segments_;
    
    bool draw_wave_;
    bool draw_lines_;
    bool draw_bg_;
    

    int line_count_;

    hc::Sprite* bg_;

public:
    Drawer();
    ~Drawer();

    virtual void update();
    virtual void render();

private:
    void clear();
};

#endif