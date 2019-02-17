#ifndef MEDIA_H_
#define MEDIA_H_

#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

namespace hc {

class Media {
public:
	struct Bitmap {
		ALLEGRO_BITMAP* bitmap;
		std::string file;
		unsigned int references;
	};

	struct Font {
		ALLEGRO_FONT* font;
		std::string file;
		int size;
		unsigned int references;
	};

private:
	std::vector<Bitmap> bitmaps_;
	std::vector<Font> fonts_;

public:
	Media();
	~Media();

	// Load resource from file
	ALLEGRO_BITMAP* image(std::string file);
	ALLEGRO_FONT* font(std::string file, int size);

	// Dereference loaded resource
	void unref(ALLEGRO_BITMAP* bitmap);
	void unref(ALLEGRO_FONT* font);

	// Remove unused resources
	void clean();
};

} //namespace hc

#endif