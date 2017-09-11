#include "media.h"

#include <string>
#include <vector>

#include <cstdlib>
#include <cstdio>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "log.h"

namespace hc {

Media::Media()
{
}

Media::~Media()
{
	// Clean up databases. This isn't Java, this is real programming
	while (bitmaps_.size()) {
		if (bitmaps_[0].references != 0)
			Log::log(Log::WARNING, "Media", "\'" + bitmaps_[0].file + "\' had " + std::to_string(bitmaps_[0].references) + " references left.");

		al_destroy_bitmap(bitmaps_[0].bitmap);
		bitmaps_.erase(bitmaps_.begin());
	}

	while (fonts_.size()) {
		if (fonts_[0].references != 0)
			Log::log(Log::WARNING, "Media", "\'" + fonts_[0].file + "\' had " + std::to_string(fonts_[0].references) + " references left.");

		al_destroy_font(fonts_[0].font);
		fonts_.erase(fonts_.begin());
	}
}

ALLEGRO_BITMAP* Media::image(std::string file)
{
	// Check if it's already been loaded
	for (unsigned int i(0); i < bitmaps_.size(); i++) {
		if (bitmaps_[i].file == file) {
			bitmaps_[i].references++;
			return bitmaps_[i].bitmap;
		}
	}

	// Load and add to the database
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);

	Bitmap img;
	img.file = file;
	img.bitmap = al_load_bitmap(file.c_str());
	img.references = 1;

	if (img.bitmap == NULL) {
		Log::log(Log::ERROR, "Media", "Failed to load " + file);
		return nullptr;
	}

	bitmaps_.push_back(img);
	return img.bitmap;
}

ALLEGRO_FONT* Media::font(std::string file, int size)
{
	for (unsigned int i(0); i < fonts_.size(); i++) {
		if (fonts_[i].file == file && fonts_[i].size == size) {
			fonts_[i].references++;
			return fonts_[i].font;
		}
	}

	Font fnt;
	fnt.file = file;
	fnt.size = size;
	fnt.font = al_load_font(file.c_str(), size, 0);
	fnt.references = 1;

	if (fnt.font == NULL) {
		Log::log(Log::ERROR, "Media", "Failed to load " + file);
		return nullptr;
	}

	fonts_.push_back(fnt);
	return fnt.font;
}

void Media::unref(ALLEGRO_BITMAP* bitmap)
{
	for (unsigned int i(0); i < bitmaps_.size(); i++) {
		if (bitmaps_[i].bitmap == bitmap) {
			bitmaps_[i].references--;
			return;
		}
	}

}

void Media::unref(ALLEGRO_FONT* font)
{
	for (unsigned int i(0); i < fonts_.size(); i++) {
		if (fonts_[i].font == font) {
			fonts_[i].references--;
			return;
		}
	}
}

void Media::clean()
{
	for (unsigned int i(0); i < bitmaps_.size(); i++) {
		if (bitmaps_[i].references == 0) {
			al_destroy_bitmap(bitmaps_[i].bitmap);
			bitmaps_.erase(bitmaps_.begin() + i--);
		}
	}

	for (unsigned int i(0); i < fonts_.size(); i++) {
		if (fonts_[i].references == 0) {
			al_destroy_font(fonts_[i].font);
			fonts_.erase(fonts_.begin() + i--);
		}
	}
}

} //namespace hc