#include "UserInterface.h"

#include <stdexcept>

#include "ProjectionPlane.h"

namespace rc {

	static void sdl_null_check(const void* pointer) {
		if (!pointer)
			throw std::runtime_error(SDL_GetError());
	}

	static void sdl_return_check(const int rc) {
		if (rc != 0)
			throw std::runtime_error(SDL_GetError());
	}

	Image::Image(const std::string& file_path, SDL_Renderer* renderer)
	{
		surface = SDL_LoadBMP(file_path.c_str());  // Assuming it handles bad files.
		sdl_null_check(surface);

		texture = SDL_CreateTextureFromSurface(renderer, surface);
		sdl_null_check(texture);
	}

	Image::~Image()
	{
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}

	UserInterface::UserInterface() :
		main_window(nullptr),
		main_window_surface(nullptr),
		renderer(nullptr),
		halt_game_loop(true)  // Safe default.
	{
	}

	UserInterface::~UserInterface()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(main_window);
		SDL_Quit();
	}


	void UserInterface::openWindow()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw std::runtime_error(SDL_GetError());

		main_window = SDL_CreateWindow(
			"Ray Cast Exercise", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			UserInterface::SCREEN_WIDTH, UserInterface::SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
		sdl_null_check(main_window);

		main_window_surface = SDL_GetWindowSurface(main_window);
		sdl_null_check(main_window_surface);

		renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		sdl_null_check(renderer);
	}
	
	void UserInterface::poll_input(Player& player)
	{
		SDL_Event user_input;
		while (SDL_PollEvent(&user_input) != 0)
			if (user_input.type == SDL_QUIT)
				halt_game_loop = true;


		// Intentionally ignore nonsensical key combos.
		const Uint8* key_states = SDL_GetKeyboardState(nullptr);
		if (key_states[SDL_SCANCODE_UP])
			player.advance(1);
		else if (key_states[SDL_SCANCODE_DOWN])
			player.advance(-1);
		
		if (key_states[SDL_SCANCODE_LEFT])
			player.turn(-1);
		else if (key_states[SDL_SCANCODE_RIGHT])
			player.turn(+1);
	}

	void UserInterface::draw_background() {
		int rc = 0;

		rc = SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
		sdl_return_check(rc);
		
		rc = SDL_RenderClear(renderer);
		sdl_return_check(rc);

		// Draw the floor darker than the ceiling and the texture. It looks better.
		rc = SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
		sdl_return_check(rc);

		SDL_Rect half_screen;
		half_screen.x = 0;
		half_screen.y = UserInterface::SCREEN_HEIGHT / 2;
		half_screen.h = UserInterface::SCREEN_HEIGHT / 2;
		half_screen.w = UserInterface::SCREEN_WIDTH;

		rc = SDL_RenderFillRect(renderer, &half_screen);
		sdl_return_check(rc);
	}

	void UserInterface::game_loop(const Grid& world, Player& player)
	{
		ProjectionPlane projection(UserInterface::SCREEN_WIDTH, UserInterface::SCREEN_HEIGHT, 60);

		halt_game_loop = false;
		while (! halt_game_loop) {
			poll_input(player);

			if (halt_game_loop)
				return;

			draw_background();
			projection.project_walls(world, player, *this);
			SDL_RenderPresent(renderer);
		}
	}

	void UserInterface::set_wall_texture(const std::string& file_path)
	{
		wall_texture = std::make_unique<Image>(file_path, renderer);
	}

	void UserInterface::draw_wall_slice(const uint16_t column, const int16_t top_row, const uint16_t height, const uint16_t texture_offset)
	{
		SDL_Rect source_slice;
		source_slice.x = texture_offset;
		source_slice.y = 0;
		source_slice.w = 1;
		source_slice.h = wall_texture->surface->w;  // Assume widht and height match.

		SDL_Rect dest_slice;
		dest_slice.x = column;
		dest_slice.y = top_row;
		dest_slice.w = 1;
		dest_slice.h = height;

		const int rc = SDL_RenderCopy(renderer, wall_texture->texture, &source_slice, &dest_slice);
		sdl_return_check(rc);
	}


}