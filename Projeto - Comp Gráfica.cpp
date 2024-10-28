#include <SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define TILE_SIZE 100

// Função para desenhar o tabuleiro
void drawBoard(SDL_Renderer* renderer) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Define a cor do tile
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Preto
            }

            SDL_Rect tile = {col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

// Função para desenhar as peças
void drawPieces(SDL_Renderer* renderer) {
    // Exemplo: desenha algumas peças (reais desenhos devem ser substituídos por sprites)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Cor das peças brancas
    SDL_Rect whitePawn = {0 * TILE_SIZE + 35, 6 * TILE_SIZE + 35, 30, 30}; // Peão
    SDL_RenderFillRect(renderer, &whitePawn);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Cor das peças pretas
    SDL_Rect blackPawn = {0 * TILE_SIZE + 35, 1 * TILE_SIZE + 35, 30, 30}; // Peão
    SDL_RenderFillRect(renderer, &blackPawn);
}

int main(int argc, char* argv[]) {
    // Inicializa o SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Cria uma janela
    SDL_Window* window = SDL_CreateWindow("Jogo de Xadrez",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Cria um renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Loop principal
    int running = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Limpa a tela
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Desenha o tabuleiro e as peças
        drawBoard(renderer);
        drawPieces(renderer);

        // Atualiza a tela
        SDL_RenderPresent(renderer);
    }

    // Limpeza
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}