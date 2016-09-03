#include "media.h"

#include <iostream>
#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

Media::Media() 
{
}

Media::~Media()
{
	/* Clean up database. This isn't Java, this is real programming */
    while (bitmaps_.size()) {
        al_destroy_bitmap(bitmaps_[0].bitmap);
        bitmaps_.erase(bitmaps_.begin());
    }
}

ALLEGRO_BITMAP* Media::image(std::string file)
{
	/* Check if it's already been loaded */
    for (unsigned int i(0); i < bitmaps_.size(); i++)
        if (bitmaps_[i].file == file)
            return bitmaps_[i].bitmap;

	/* Load and add to the database */
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);

	Bitmap img;
	img.file = file;
	img.bitmap = al_load_bitmap(file.c_str());

	if (img.bitmap == NULL) {
		std::cout << "Failed to load " << file << "\n";
		return nullptr;
	}

	bitmaps_.push_back(img);
	return img.bitmap;
}