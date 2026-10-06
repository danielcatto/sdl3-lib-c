#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 320
#define HEIGHT 200

uint32_t framebuffer[WIDTH * HEIGHT];

void put_pixel(int x, int y, uint32_t color)
{
    framebuffer[WIDTH * y + x] = color;
}

            // Quadrado preenchido
void draw_filled_square(int x, int y, int size, uint32_t color) {
    for (int cy = y; cy < y + size; cy++) {
        for (int cx = x; cx < x + size; cx++) {
            // Checagem de limites para não desenhar fora da memória do framebuffer
            if (cx >= 0 && cx < WIDTH && cy >= 0 && cy < HEIGHT) {
                put_pixel(cx, cy, color);
            }
        }
    }
}

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Failed to initialize SDL\n");
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(
        "SDL3 Framebuffer Example",
        WIDTH * 3,
        HEIGHT * 3,
        0
    );

    if (window == NULL) {
        fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (renderer == NULL) {
        fprintf(stderr, "Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (texture == NULL) {
        fprintf(stderr, "Failed to create texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    while (true) {
        /* Puxar os eventos da janela */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return EXIT_SUCCESS;
            }
        }




        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            framebuffer[i] = 0xFFF00F30;
        }
        int x=0;
        int y=0;

        draw_filled_square(x, y, 5, 0xffd8);
        draw_filled_square(10, y, 5, 0xffd8);
        draw_filled_square(5, 10, 5, 0xffd8);

        for (int i = 0; i < WIDTH-1; i++) {
            
            for (int j = 0; j < HEIGHT-1; j++) {
                //draw_filled_square(x, y, 5, 0xffd8);
                y += 8;
            }
            
            y += 10;
        }

        /* here we can manipulate the framebuffer array as we wish */
        // Example: Clear the framebuffer
        //int x = 0;
        //int y = 0;
        //while (y < HEIGHT) {
        //    while (x < WIDTH) {
        //        put_pixel(x, y, 0xffffffff); // Set pixel to white
        //        x++;
        //    }
        //    x = 0;
        //    y++;
        //}
        
        //put_pixel(x, y, 0xFF0000FF); // Set pixel at (0, 0) to red

        /* Copy the contents of the framebuffer to the texture */
        SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            WIDTH * sizeof(uint32_t) // pitch --> the width of the image/texture in bytes
        );

        /* Display the window and the renderer */
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

