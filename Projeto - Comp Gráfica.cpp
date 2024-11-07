#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <vector>

#define TILE_SIZE 60
#define WINDOW_WIDTH TILE_SIZE*8
#define WINDOW_HEIGHT TILE_SIZE*8

// Estrutura das peças
struct Piece{
    char type; //k = Rei, q = Dama, b = Bispo, n = Cavalo, r = Torre, p = Peão
    int color; //0 = Brancas, 1 = Pretas

    int windowX, windowY;
    int boardX, boardY;
};

//Estrutura do tabuleiro
struct BoardData{
    bool exists = false;
    int pieceID = -1;
};

//Função para fazer o setup inicial do tabuleiro
std::vector<Piece> setupBoard(BoardData board[][8]){
    Piece temp;
    std::vector<Piece> pieces;

    //Torres
    temp = {'r',1,0*TILE_SIZE,0*TILE_SIZE,0,0};
    pieces.push_back(temp);
    board[0][0].exists = true;
    board[0][0].pieceID = pieces.size()-1;
    temp = {'r',1,7*TILE_SIZE,0*TILE_SIZE,7,0};
    pieces.push_back(temp);
    board[7][0].exists = true;
    board[7][0].pieceID = pieces.size()-1;
    temp = {'r',0,0*TILE_SIZE,7*TILE_SIZE,0,7};
    pieces.push_back(temp);
    board[0][7].exists = true;
    board[0][7].pieceID = pieces.size()-1;
    temp = {'r',0,7*TILE_SIZE,7*TILE_SIZE,7,7};
    pieces.push_back(temp);
    board[7][7].exists = true;
    board[7][7].pieceID = pieces.size()-1;

    //Cavalos
    temp = {'n',1,1*TILE_SIZE,0*TILE_SIZE,1,0};
    pieces.push_back(temp);
    board[1][0].exists = true;
    board[1][0].pieceID = pieces.size()-1;
    temp = {'n',1,6*TILE_SIZE,0*TILE_SIZE,6,0};
    pieces.push_back(temp);
    board[6][0].exists = true;
    board[6][0].pieceID = pieces.size()-1;
    temp = {'n',0,1*TILE_SIZE,7*TILE_SIZE,1,7};
    pieces.push_back(temp);
    board[1][7].exists = true;
    board[1][7].pieceID = pieces.size()-1;
    temp = {'n',0,6*TILE_SIZE,7*TILE_SIZE,6,7};
    pieces.push_back(temp);
    board[6][7].exists = true;
    board[6][7].pieceID = pieces.size()-1;

    //Bispos
    temp = {'b',1,2*TILE_SIZE,0*TILE_SIZE,2,0};
    pieces.push_back(temp);
    board[2][0].exists = true;
    board[2][0].pieceID = pieces.size()-1;
    temp = {'b',1,5*TILE_SIZE,0*TILE_SIZE,5,0};
    pieces.push_back(temp);
    board[5][0].exists = true;
    board[5][0].pieceID = pieces.size()-1;
    temp = {'b',0,2*TILE_SIZE,7*TILE_SIZE,2,7};
    pieces.push_back(temp);
    board[2][7].exists = true;
    board[2][7].pieceID = pieces.size()-1;
    temp = {'b',0,5*TILE_SIZE,7*TILE_SIZE,5,7};
    pieces.push_back(temp);
    board[5][7].exists = true;
    board[5][7].pieceID = pieces.size()-1;

    //Damas
    temp = {'q',1,3*TILE_SIZE,0*TILE_SIZE,3,0};
    pieces.push_back(temp);
    board[3][0].exists = true;
    board[3][0].pieceID = pieces.size()-1;
    temp = {'q',0,3*TILE_SIZE,7*TILE_SIZE,3,7};
    pieces.push_back(temp);
    board[3][7].exists = true;
    board[3][7].pieceID = pieces.size()-1;

    //Reis
    temp = {'k',1,4*TILE_SIZE,0*TILE_SIZE,4,0};
    pieces.push_back(temp);
    board[4][0].exists = true;
    board[4][0].pieceID = pieces.size()-1;
    temp = {'k',0,4*TILE_SIZE,7*TILE_SIZE,4,7};
    pieces.push_back(temp);
    board[4][7].exists = true;
    board[4][7].pieceID = pieces.size()-1;

    //Peãos
    for(int i=0; i<8; i++){
        temp = {'p',1,i*TILE_SIZE,1*TILE_SIZE,i,1};
        pieces.push_back(temp);
        board[i][1].exists = true;
        board[i][1].pieceID = pieces.size()-1;
        temp = {'p',0,i*TILE_SIZE,6*TILE_SIZE,i,6};
        pieces.push_back(temp);
        board[i][6].exists = true;
        board[i][6].pieceID = pieces.size()-1;
    }

    return pieces;
}

// Função para desenhar o tabuleiro
void drawBoard(SDL_Renderer* renderer) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            // Define a cor do tile
            if ((row + col) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Branco
            } else {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Preto
            }

            SDL_Rect tile = {col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

// Função para desenhar as peças
void drawPieces(SDL_Renderer* renderer, std::vector<Piece> pieces, SDL_Texture* tex, int movingPieceID) {
    SDL_Rect src, dest;

    src = {0,0,60,60};
    dest = {0,0,TILE_SIZE,TILE_SIZE};

    for(int i=0; i<pieces.size(); i++)
    {
        //Determina a posição na textura fonte a partir da informação da peça
        switch(pieces[i].color){
            case 0: src.y=60;
            break;
            case 1: src.y=0;
            break;
        }
        switch(pieces[i].type){
            case 'q': src.x=0;
            break;
            case 'k': src.x=60;
            break;
            case 'r': src.x=120;
            break;
            case 'n': src.x=180;
            break;
            case 'b': src.x=240;
            break;
            case 'p': src.x=300;
            break;
        }

        dest.x = pieces[i].windowX;
        dest.y = pieces[i].windowY;

        SDL_RenderCopy(renderer, tex, &src, &dest);
    }

    //Mostra a peça sendo movida por cima
    if(movingPieceID >= 0){
        switch(pieces[movingPieceID].color){
            case 0: src.y=60;
            break;
            case 1: src.y=0;
            break;
        }
        switch(pieces[movingPieceID].type){
            case 'q': src.x=0;
            break;
            case 'k': src.x=60;
            break;
            case 'r': src.x=120;
            break;
            case 'n': src.x=180;
            break;
            case 'b': src.x=240;
            break;
            case 'p': src.x=300;
            break;
        }

        dest.x = pieces[movingPieceID].windowX;
        dest.y = pieces[movingPieceID].windowY;

        SDL_RenderCopy(renderer, tex, &src, &dest);
    }
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

    //Cria e armazena a textura das peças
    SDL_Surface* temp = IMG_Load("ChessPiecesArray.png");
	SDL_Texture* pieces_tex = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);

    //Cria a estrutura do tabuleiro e faz o setup inicial
    BoardData board[8][8];

    std::vector<Piece> pieces = setupBoard(board);

    // Loop principal
    int running = 1;
    SDL_Event event;
    int mouseX, mouseY;
    bool movingPiece = false;
    int movingPieceID = -1;
    while (running) {

        //Checa Inputs e atualiza o tabuleiro
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            //Checa se o botão do mouse foi pressionado, e se houver uma peça no quadrado, ela começa a ser movida
            if (event.type == SDL_MOUSEBUTTONDOWN && !movingPiece)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                if(board[mouseX/TILE_SIZE][mouseY/TILE_SIZE].exists){
                    movingPiece = true;
                    movingPieceID = board[mouseX/TILE_SIZE][mouseY/TILE_SIZE].pieceID;
                }
            }

            //Quando o botão é solto, a peça vai para o novo quadrado se estiver vazio
            if(event.type == SDL_MOUSEBUTTONUP && movingPiece)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                if(!board[mouseX/TILE_SIZE][mouseY/TILE_SIZE].exists){
                    if(pieces[movingPieceID].boardX != mouseX/TILE_SIZE || pieces[movingPieceID].boardY != mouseY/TILE_SIZE){
                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].exists = false;
                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].pieceID = -1;
                    
                    pieces[movingPieceID].boardX = mouseX/TILE_SIZE;
                    pieces[movingPieceID].boardY = mouseY/TILE_SIZE;
                    pieces[movingPieceID].windowX = (mouseX/TILE_SIZE) * TILE_SIZE;
                    pieces[movingPieceID].windowY = (mouseY/TILE_SIZE) * TILE_SIZE;

                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].exists = true;
                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].pieceID = movingPieceID;

                    movingPiece = false;
                    movingPieceID = -1;
                    }
                }
                //Se não estiver vazio, ela volta para o quadrado original
                else {
                    pieces[movingPieceID].windowX = pieces[movingPieceID].boardX * TILE_SIZE;
                    pieces[movingPieceID].windowY = pieces[movingPieceID].boardY * TILE_SIZE;
                    
                    movingPiece = false;
                    movingPieceID = -1;
                }
            }
        }

        //Faz peça mover com o mouse quando o botão está apertado
        if(movingPiece){
            SDL_GetMouseState(&mouseX, &mouseY);
            pieces[movingPieceID].windowX = mouseX-(TILE_SIZE/2);
            pieces[movingPieceID].windowY = mouseY-(TILE_SIZE/2);
        }

        // Limpa a tela
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Desenha o tabuleiro e as peças
        drawBoard(renderer);
        drawPieces(renderer, pieces, pieces_tex, movingPieceID);

        // Atualiza a tela
        SDL_RenderPresent(renderer);
    }

    // Limpeza
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}