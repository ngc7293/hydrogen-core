#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#ifndef MEDIA_H_
#define MEDIA_H_

class Media {
public:
    struct Bitmap {
        ALLEGRO_BITMAP* bitmap;
        std::string file;
    };

private:
    std::vector<Bitmap> bitmaps_;

public:
    Media();
    ~Media();

	/* Load image from file */
    ALLEGRO_BITMAP* image(std::string file);
};

#endif