#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#ifndef MEDIA_H_
#define MEDIA_H_

namespace hc {

class Media {
public:
	struct Bitmap {
		ALLEGRO_BITMAP* bitmap;
		std::string file;
	};

	struct Font {
		ALLEGRO_FONT* font;
		std::string file;
		int size;
	};

private:
	std::vector<Bitmap> bitmaps_;
	std::vector<Font> fonts_;

public:
	Media();
	~Media();

	// Load image from file
	ALLEGRO_BITMAP* image(std::string file);
	ALLEGRO_FONT* font(std::string file, int size);
};

} //namespace hc

#endif