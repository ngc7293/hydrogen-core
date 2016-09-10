#include "media.h"

#include <iostream>
#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

namespace hc {

Media::Media()
{
}

Media::~Media()
{
	// Clean up databases. This isn't Java, this is real programming
	while (bitmaps_.size()) {
		al_destroy_bitmap(bitmaps_[0].bitmap);
		bitmaps_.erase(bitmaps_.begin());
	}

	while (fonts_.size()) {
		al_destroy_font(fonts_[0].font);
		fonts_.erase(fonts_.begin());
	}
}

ALLEGRO_BITMAP* Media::image(std::string file)
{
	// Check if it's already been loaded
	for (unsigned int i(0); i < bitmaps_.size(); i++) {
		if (bitmaps_[i].file == file) {
			return bitmaps_[i].bitmap;
		}
	}

	// Load and add to the database
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP | ALLEGRO_CONVERT_BITMAP);

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

ALLEGRO_FONT* Media::font(std::string file, int size)
{
	for (unsigned int i(0); i < bitmaps_.size(); i++) {
		if (fonts_[i].file == file && fonts_[i].size == size) {
			return fonts_[i].font;
		}
	}

	al_set_new_bitmap_flags(ALLEGRO_CONVERT_BITMAP);

	Font fnt;
	fnt.file = file;
	fnt.size = size;
	fnt.font = al_load_font(file.c_str(), size, 0);

	if (fnt.font == NULL) {
		std::cout << "Failed to load " << file << "\n";
		return nullptr;
	}

	fonts_.push_back(fnt);
	return fnt.font;
}

} //namespace hc