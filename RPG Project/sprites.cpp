#include "sprites.h"
#include "game.h"
#include "vars.h"

Sprites::~Sprites()
{
	unloadSprite();
}

void Sprites::setup(Json::Value sprite_script, unordered_map<string, string> sprites, Game* game, string graphics_db_name)
{
	this->sprite_script = sprite_script;
	this->sprite_names = sprites;
	this->game = game;
	this->graphics_db_name = graphics_db_name;
	recordData();
}

void Sprites::recordData()
{
	for (auto const& name : sprite_script.getMemberNames())
	{
		/*scale_list[name] = sprite_script[name]["scale"].asInt();*/
		scale_list[name] = 1;
		width_list[name] = sprite_script[name]["width"].asFloat();
		height_list[name] = sprite_script[name]["height"].asFloat();
		max_colors[name] = sprite_script[name]["max_colors"].asInt();
	}
}




Texture2D Sprites::getFrame(string sheet_name, int frame, Rectangle& source_rect)
{
	this->sheet_name = sheet_name;
	float width = width_list[sheet_name];
	float height = height_list[sheet_name];
	CreateSprites();
	sprite = game->GFX.sprites[sheet_name + graphics_db_name];
	vector<float> position = getPosition(frame, width, height, sprite);

	palette_rect = {position[0] * width, position[1] * height, width, height};
	source_rect.x = position[0] * width;
	source_rect.y = position[1] * height;
	if (palette_on == true)
	{
		changeTexturePalette();
	}
	return sprite;
}

Texture2D Sprites::getFrame(string sheet_name, Rectangle source_rect)
{
	this->sheet_name = sheet_name;
	palette_rect = { source_rect.x, source_rect.y, source_rect.width, source_rect.height };
	CreateSprites();
	changeTexturePalette();
	return sprite;
}

void Sprites::setPalette(string palette_name)
{
	this->palette_name = palette_name;
	palette_on = true;
}

void Sprites::turnOffPalette()
{
	palette_on = false;
}

void Sprites::unloadSprite()
{
	if (!unloadedSprites)
	{
		if (game != nullptr)
		{
			if (!game->closingGame)
			{
				for (auto& sheet_name : sprites)
				{
					/*cout << "UNLOADED TEXTURE ID: " << sprite.id << endl;*/
					UnloadTexture(game->GFX.sprites[sheet_name]);
					game->GFX.sprites.erase(sheet_name);
				}
				sprites.clear();
				unloadedSprites = true;
			}
		}
		if ((int)colors.size() > 0)
		{
			for (auto& [key, color_palette] : colors)
			{
				UnloadImagePalette(color_palette);
			}
			colors.clear();
		}
	}
}



void Sprites::CreateSprites()
{
	if (!game->GFX.SpriteExists(sheet_name + graphics_db_name))
	{
		Image temp_img = LoadImage(sprite_names[sheet_name].c_str());
		/*cout << "SPRITE NAME: " << sheet_name << endl;*/
		colors[sheet_name] = LoadImagePalette(temp_img, max_colors[sheet_name], &temp_img.mipmaps);
		UnloadImage(temp_img);

		game->GFX.sprites[sheet_name + graphics_db_name] = LoadTexture(sprite_names[sheet_name].c_str());
		sprites.push_back(sheet_name + graphics_db_name);
	}
}

void Sprites::changeTexturePalette()
{
	if (game->GFX.sprites.find(palette_name + graphics_db_name) == game->GFX.sprites.end())
	{
		palette_image = LoadImageFromTexture(sprite);
		ImageCrop(&palette_image, palette_rect);
		Color* colors = LoadImagePalette(palette_image, max_colors[sheet_name], &max_colors[sheet_name]);
		
		palette_script = game->palette_database.palettes[palette_name];

		for (string name : palette_script.getMemberNames())
		{
			int num = stoi(name);
			Color pixel = colors[num];
			Json::Value palette = palette_script[name];
			Color palette_colors = game->color_database.colors[palette.asString()];
			if (pixel.a == 255)
			{
				ImageColorReplace(&palette_image, pixel, palette_colors);
			}
		}
		/*for (int i = 0; i < 19; i++)
		{
			Color pixel = colors[i];
			cout << "R: " << (int)pixel.r << " G: " << (int)pixel.g << " B: " << (int)pixel.b << " A: " << (int)pixel.a << endl;
		}*/
		if (IsImageReady(palette_image))
		{
			Texture2D temp_text = LoadTextureFromImage(palette_image);
			game->GFX.sprites[palette_name + graphics_db_name] = temp_text;
			sprites.push_back(palette_name + graphics_db_name);
		}
		UnloadImagePalette(colors);
		UnloadImage(palette_image);
	}
	sprite = game->GFX.sprites[palette_name + graphics_db_name];
}


//Helpers
vector<float> Sprites::getPosition(int frame, float width, float height, Texture2D sprite)
{
	float max_x = sprite.width / width;
	float max_y = sprite.height / height;
	int x = 0;
	int y = 0;

	int on_frame = 1;
	for (int temp_y = 0; temp_y < max_y; temp_y++)
	{
		for (int temp_x = 0; temp_x < max_x; temp_x++)
		{
			x = temp_x;
			y = temp_y;
			if (on_frame == frame)
			{
				return vector<float> {(float)x, (float)y};
			}
			on_frame++;
		}
	}
	return vector<float> {0, 0};
}


