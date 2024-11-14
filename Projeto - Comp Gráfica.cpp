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

    bool captured = false;
    bool hasMoved = false;
};

//Estrutura do tabuleiro
struct BoardData{
    int X;
    int Y;
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
void drawBoard(SDL_Renderer* renderer, std::vector<BoardData> legalMoves, BoardData lastStart, BoardData lastEnd) {
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

    //Desenha os quadrados do último movimento em azul
    if(lastStart.X != -1)
    {
    if ((lastStart.X + lastStart.Y) % 2 == 0) {
        SDL_SetRenderDrawColor(renderer, 192, 192, 255, 255); // Branco
    } else {
        SDL_SetRenderDrawColor(renderer, 96, 96, 128, 255); // Preto
    }

    SDL_Rect tile = {lastStart.X * TILE_SIZE, lastStart.Y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &tile);

    if ((lastEnd.X + lastEnd.Y) % 2 == 0) {
        SDL_SetRenderDrawColor(renderer, 192, 192, 255, 255); // Branco
    } else {
        SDL_SetRenderDrawColor(renderer, 96, 96, 128, 255); // Preto
    }

    tile = {lastEnd.X * TILE_SIZE, lastEnd.Y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &tile);
    }

    //Desenha os quadrados com movimentos legais em verde
    for(int i=0; i<legalMoves.size(); i++){
        if ((legalMoves[i].X + legalMoves[i].Y) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 128, 255, 128, 255); // Branco
            } else {
                SDL_SetRenderDrawColor(renderer, 64, 128, 64, 255); // Preto
            }

            SDL_Rect tile = {legalMoves[i].X * TILE_SIZE, legalMoves[i].Y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &tile);
    }
}

// Função para desenhar as peças
void drawPieces(SDL_Renderer* renderer, std::vector<Piece> pieces, SDL_Texture* tex, int movingPieceID) {
    SDL_Rect src, dest;

    src = {0,0,60,60};
    dest = {0,0,TILE_SIZE,TILE_SIZE};

    for(int i=0; i<pieces.size(); i++){
        //Checa se a peça está capturada, e não a desenha caso esteja
        if(!pieces[i].captured){
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

            SDL_RenderCopy(renderer, tex, &src, &dest);}
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

// Função para checar se um quadrado está sob ataque
bool isThreatened(BoardData board[][8], BoardData target, std::vector<Piece> pieces, int color){
        for(int i=1; i<8; i++){
            if(target.X+i > 7) break;
            else if(board[target.X+i][target.Y].exists && pieces[board[target.X+i][target.Y].pieceID].color == color) break;
            else if(board[target.X+i][target.Y].exists){
                if(pieces[board[target.X+i][target.Y].pieceID].type == 'r' ||
                   pieces[board[target.X+i][target.Y].pieceID].type == 'q' ||
                   pieces[board[target.X+i][target.Y].pieceID].type == 'k') return true;
                else break;
            }
        }
        for(int i=1; i<8; i++){
            if(target.X-i < 0) break;
            else if(board[target.X-i][target.Y].exists && pieces[board[target.X-i][target.Y].pieceID].color == color) break;
            else if(board[target.X-i][target.Y].exists){
                if(pieces[board[target.X-i][target.Y].pieceID].type == 'r' ||
                   pieces[board[target.X-i][target.Y].pieceID].type == 'q' ||
                   pieces[board[target.X-i][target.Y].pieceID].type == 'k') return true;
                else break;
            }
        }
        for(int i=1; i<8; i++){
            if(target.Y+i > 7) break;
            else if(board[target.X][target.Y+i].exists && pieces[board[target.X][target.Y+i].pieceID].color == color) break;
            else if(board[target.X][target.Y+i].exists){
                if(pieces[board[target.X][target.Y+i].pieceID].type == 'r' ||
                   pieces[board[target.X][target.Y+i].pieceID].type == 'q' ||
                   pieces[board[target.X][target.Y+i].pieceID].type == 'k') return true;
                else break;
            }
        }
        for(int i=1; i<8; i++){
            if(target.Y-i < 0) break;
            else if(board[target.X][target.Y-i].exists && pieces[board[target.X][target.Y-i].pieceID].color == color) break;
            else if(board[target.X][target.Y-i].exists){
                if(pieces[board[target.X][target.Y-i].pieceID].type == 'r' ||
                   pieces[board[target.X][target.Y-i].pieceID].type == 'q' ||
                   pieces[board[target.X][target.Y-i].pieceID].type == 'k' ||
                   (pieces[board[target.X][target.Y-i].pieceID].type == 'p' && color == 0)) return true;
                else break;
            }
        }

        for(int i=1; i<8; i++){
            if(target.X+i > 7 || target.Y+i > 7) break;
            else if(board[target.X+i][target.Y+i].exists && pieces[board[target.X+i][target.Y+i].pieceID].color == color) break;
            else if(board[target.X+i][target.Y+i].exists){
                if(pieces[board[target.X+i][target.Y+i].pieceID].type == 'b' ||
                   pieces[board[target.X+i][target.Y+i].pieceID].type == 'q' ||
                   pieces[board[target.X+i][target.Y+i].pieceID].type == 'k' ||
                   (pieces[board[target.X][target.Y-i].pieceID].type == 'p' && color == 1)) return true;
                else break;
            }
        }
        for(int i=1; i<8; i++){
            if(target.X-i < 0 || target.Y-i < 0) break;
            else if(board[target.X-i][target.Y-i].exists && pieces[board[target.X-i][target.Y-i].pieceID].color == color) break;
            else if(board[target.X-i][target.Y-i].exists){
                if(pieces[board[target.X-i][target.Y-i].pieceID].type == 'b' ||
                   pieces[board[target.X-i][target.Y-i].pieceID].type == 'q' ||
                   pieces[board[target.X-i][target.Y-i].pieceID].type == 'k' ||
                   (pieces[board[target.X][target.Y-i].pieceID].type == 'p' && color == 0)) return true;
                else break;
            }
        }
        for(int i=1; i<8; i++){
            if(target.X-i < 0 || target.Y+i > 7) break;
            else if(board[target.X-i][target.Y+i].exists && pieces[board[target.X-i][target.Y+i].pieceID].color == color) break;
            else if(board[target.X-i][target.Y+i].exists){
                if(pieces[board[target.X-i][target.Y+i].pieceID].type == 'b' ||
                   pieces[board[target.X-i][target.Y+i].pieceID].type == 'q' ||
                   pieces[board[target.X-i][target.Y+i].pieceID].type == 'k' ||
                   (pieces[board[target.X][target.Y-i].pieceID].type == 'p' && color == 1)) return true;
                else break;
            }
        }
        for(int i=1; i<8; i++){
            if(target.X+i > 7 || target.Y-i < 0) break;
            else if(board[target.X+i][target.Y-i].exists && pieces[board[target.X+i][target.Y-i].pieceID].color == color) break;
            else if(board[target.X+i][target.Y-i].exists){
                if(pieces[board[target.X+i][target.Y-i].pieceID].type == 'b' ||
                   pieces[board[target.X+i][target.Y-i].pieceID].type == 'q' ||
                   pieces[board[target.X+i][target.Y-i].pieceID].type == 'k') return true;
                else break;
            }
        }

        
        if(target.X+2 <= 7 && target.Y+1 <= 7 && (board[target.X+2][target.Y+1].exists && pieces[board[target.X+2][target.Y+1].pieceID].color != color && pieces[board[target.X+2][target.Y+1].pieceID].type == 'n')){
            return true;
        }
        if(target.X+2 <= 7 && target.Y-1 >= 0 && (board[target.X+2][target.Y-1].exists && pieces[board[target.X+2][target.Y-1].pieceID].color != color && pieces[board[target.X+2][target.Y-1].pieceID].type == 'n')){
            return true;
        }
        if(target.X-2 >= 0 && target.Y+1 <= 7 && (board[target.X-2][target.Y+1].exists && pieces[board[target.X-2][target.Y+1].pieceID].color != color && pieces[board[target.X-2][target.Y+1].pieceID].type == 'n')){
            return true;
        }
        if(target.X-2 >= 0 && target.Y-1 >= 0 && (board[target.X-2][target.Y-1].exists && pieces[board[target.X-2][target.Y-1].pieceID].color != color && pieces[board[target.X-2][target.Y-1].pieceID].type == 'n')){
            return true;
        }
        
        if(target.X+1 <= 7 && target.Y+2 <= 7 && (board[target.X+1][target.Y+2].exists && pieces[board[target.X+1][target.Y+2].pieceID].color != color && pieces[board[target.X+1][target.Y+2].pieceID].type == 'n')){
            return true;
        }
        if(target.X+1 <= 7 && target.Y-2 >= 0 && (board[target.X+1][target.Y-2].exists && pieces[board[target.X+1][target.Y-2].pieceID].color != color && pieces[board[target.X+1][target.Y-2].pieceID].type == 'n')){
            return true;
        }
        if(target.X-1 >= 0 && target.Y+2 <= 7 && (board[target.X-1][target.Y+2].exists && pieces[board[target.X-1][target.Y+2].pieceID].color != color && pieces[board[target.X-1][target.Y+2].pieceID].type == 'n')){
            return true;
        }
        if(target.X-1 >= 0 && target.Y-2 >= 0 && (board[target.X-1][target.Y-2].exists && pieces[board[target.X-1][target.Y-2].pieceID].color != color && pieces[board[target.X-1][target.Y-2].pieceID].type == 'n')){
            return true;
        }

    return false;
}

// Função para gerar movimentos legais
std::vector<BoardData> getLegalMoves(BoardData board[][8], Piece p, std::vector<Piece> pieces, BoardData lastMove){
    std::vector<BoardData> legal;

    switch(p.type){
        //Peão
        case 'p':
        if(p.color == 0){
            if(!board[p.boardX][p.boardY-1].exists){
                legal.push_back(board[p.boardX][p.boardY-1]);
                if(!board[p.boardX][p.boardY-2].exists && !p.hasMoved)legal.push_back(board[p.boardX][p.boardY-2]);
            }

            //Captura
            if(p.boardX-1 >= 0 && board[p.boardX-1][p.boardY-1].exists && pieces[board[p.boardX-1][p.boardY-1].pieceID].color == 1)legal.push_back(board[p.boardX-1][p.boardY-1]);
            if(p.boardX+1 <= 7 && board[p.boardX+1][p.boardY-1].exists && pieces[board[p.boardX+1][p.boardY-1].pieceID].color == 1)legal.push_back(board[p.boardX+1][p.boardY-1]);

            //En Passant
            if(p.boardX-1 >= 0 && board[p.boardX-1][p.boardY].X == lastMove.X && board[p.boardX-1][p.boardY].Y == lastMove.Y){
                if(pieces[board[p.boardX-1][p.boardY].pieceID].type == 'p' && pieces[board[p.boardX-1][p.boardY].pieceID].color == 1){
                    legal.push_back(board[p.boardX-1][p.boardY-1]);
                }
            }
            else if(p.boardX+1 <= 7 && board[p.boardX+1][p.boardY].X == lastMove.X && board[p.boardX+1][p.boardY].Y == lastMove.Y){
                if(pieces[board[p.boardX+1][p.boardY].pieceID].type == 'p' && pieces[board[p.boardX+1][p.boardY].pieceID].color == 1){
                    legal.push_back(board[p.boardX+1][p.boardY-1]);
                }
            }
        }
        else{
            if(!board[p.boardX][p.boardY+1].exists){
                legal.push_back(board[p.boardX][p.boardY+1]);
                if(!board[p.boardX][p.boardY+2].exists && !p.hasMoved)legal.push_back(board[p.boardX][p.boardY+2]); 
            }

            //Captura
            if(p.boardX-1 >= 0 && board[p.boardX-1][p.boardY+1].exists && pieces[board[p.boardX-1][p.boardY+1].pieceID].color == 0)legal.push_back(board[p.boardX-1][p.boardY+1]);
            if(p.boardX+1 <= 7 && board[p.boardX+1][p.boardY+1].exists && pieces[board[p.boardX+1][p.boardY+1].pieceID].color == 0)legal.push_back(board[p.boardX+1][p.boardY+1]);

            //En Passant
            if(p.boardX-1 >= 0 && board[p.boardX-1][p.boardY].X == lastMove.X && board[p.boardX-1][p.boardY].Y == lastMove.Y){
                if(pieces[board[p.boardX-1][p.boardY].pieceID].type == 'p' && pieces[board[p.boardX-1][p.boardY].pieceID].color == 0){
                    legal.push_back(board[p.boardX-1][p.boardY+1]);
                }
            }
            else if(p.boardX+1 <= 7 && board[p.boardX+1][p.boardY].X == lastMove.X && board[p.boardX+1][p.boardY].Y == lastMove.Y){
                if(pieces[board[p.boardX+1][p.boardY].pieceID].type == 'p' && pieces[board[p.boardX+1][p.boardY].pieceID].color == 0){
                    legal.push_back(board[p.boardX+1][p.boardY+1]);
                }
            }
        }
        break;
        //Torre
        case 'r':
        for(int i=1; i<8; i++){
            if(p.boardX+i > 7) break;
            else if(board[p.boardX+i][p.boardY].exists && pieces[board[p.boardX+i][p.boardY].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY].exists){
                legal.push_back(board[p.boardX+i][p.boardY]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX-i < 0) break;
            else if(board[p.boardX-i][p.boardY].exists && pieces[board[p.boardX-i][p.boardY].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY].exists){
                legal.push_back(board[p.boardX-i][p.boardY]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY]);
        }
        for(int i=1; i<8; i++){
            if(p.boardY+i > 7) break;
            else if(board[p.boardX][p.boardY+i].exists && pieces[board[p.boardX][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX][p.boardY+i].exists){
                legal.push_back(board[p.boardX][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX][p.boardY+i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardY-i < 0) break;
            else if(board[p.boardX][p.boardY-i].exists && pieces[board[p.boardX][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX][p.boardY-i].exists){
                legal.push_back(board[p.boardX][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX][p.boardY-i]);
        }
        break;
        //Bispo
        case 'b':
        for(int i=1; i<8; i++){
            if(p.boardX+i > 7 || p.boardY+i > 7) break;
            else if(board[p.boardX+i][p.boardY+i].exists && pieces[board[p.boardX+i][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY+i].exists){
                legal.push_back(board[p.boardX+i][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY+i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX-i < 0 || p.boardY-i < 0) break;
            else if(board[p.boardX-i][p.boardY-i].exists && pieces[board[p.boardX-i][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY-i].exists){
                legal.push_back(board[p.boardX-i][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY-i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX-i < 0 || p.boardY+i > 7) break;
            else if(board[p.boardX-i][p.boardY+i].exists && pieces[board[p.boardX-i][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY+i].exists){
                legal.push_back(board[p.boardX-i][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY+i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX+i > 7 || p.boardY-i < 0) break;
            else if(board[p.boardX+i][p.boardY-i].exists && pieces[board[p.boardX+i][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY-i].exists){
                legal.push_back(board[p.boardX+i][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY-i]);
        }
        break;
        //Dama
        case 'q':
        for(int i=1; i<8; i++){
            if(p.boardX+i > 7) break;
            else if(board[p.boardX+i][p.boardY].exists && pieces[board[p.boardX+i][p.boardY].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY].exists){
                legal.push_back(board[p.boardX+i][p.boardY]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX-i < 0) break;
            else if(board[p.boardX-i][p.boardY].exists && pieces[board[p.boardX-i][p.boardY].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY].exists){
                legal.push_back(board[p.boardX-i][p.boardY]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY]);
        }
        for(int i=1; i<8; i++){
            if(p.boardY+i > 7) break;
            else if(board[p.boardX][p.boardY+i].exists && pieces[board[p.boardX][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX][p.boardY+i].exists){
                legal.push_back(board[p.boardX][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX][p.boardY+i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardY-i < 0) break;
            else if(board[p.boardX][p.boardY-i].exists && pieces[board[p.boardX][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX][p.boardY-i].exists){
                legal.push_back(board[p.boardX][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX][p.boardY-i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX+i > 7 || p.boardY+i > 7) break;
            else if(board[p.boardX+i][p.boardY+i].exists && pieces[board[p.boardX+i][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY+i].exists){
                legal.push_back(board[p.boardX+i][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY+i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX-i < 0 || p.boardY-i < 0) break;
            else if(board[p.boardX-i][p.boardY-i].exists && pieces[board[p.boardX-i][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY-i].exists){
                legal.push_back(board[p.boardX-i][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY-i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX-i < 0 || p.boardY+i > 7) break;
            else if(board[p.boardX-i][p.boardY+i].exists && pieces[board[p.boardX-i][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY+i].exists){
                legal.push_back(board[p.boardX-i][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY+i]);
        }
        for(int i=1; i<8; i++){
            if(p.boardX+i > 7 || p.boardY-i < 0) break;
            else if(board[p.boardX+i][p.boardY-i].exists && pieces[board[p.boardX+i][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY-i].exists){
                legal.push_back(board[p.boardX+i][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY-i]);
        }
        break;
        //Rei
        case 'k':
        for(int i=1; i<=1; i++){
            if(p.boardX+i > 7) break;
            else if(board[p.boardX+i][p.boardY].exists && pieces[board[p.boardX+i][p.boardY].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY].exists){
                legal.push_back(board[p.boardX+i][p.boardY]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY]);
        }
        for(int i=1; i<=1; i++){
            if(p.boardX-i < 0) break;
            else if(board[p.boardX-i][p.boardY].exists && pieces[board[p.boardX-i][p.boardY].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY].exists){
                legal.push_back(board[p.boardX-i][p.boardY]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY]);
        }
        for(int i=1; i<=1; i++){
            if(p.boardY+i > 7) break;
            else if(board[p.boardX][p.boardY+i].exists && pieces[board[p.boardX][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX][p.boardY+i].exists){
                legal.push_back(board[p.boardX][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX][p.boardY+i]);
        }
        for(int i=1; i<=1; i++){
            if(p.boardY-i < 0) break;
            else if(board[p.boardX][p.boardY-i].exists && pieces[board[p.boardX][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX][p.boardY-i].exists){
                legal.push_back(board[p.boardX][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX][p.boardY-i]);
        }
        for(int i=1; i<=1; i++){
            if(p.boardX+i > 7 || p.boardY+i > 7) break;
            else if(board[p.boardX+i][p.boardY+i].exists && pieces[board[p.boardX+i][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY+i].exists){
                legal.push_back(board[p.boardX+i][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY+i]);
        }
        for(int i=1; i<=1; i++){
            if(p.boardX-i < 0 || p.boardY-i < 0) break;
            else if(board[p.boardX-i][p.boardY-i].exists && pieces[board[p.boardX-i][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY-i].exists){
                legal.push_back(board[p.boardX-i][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY-i]);
        }
        for(int i=1; i<=1; i++){
            if(p.boardX-i < 0 || p.boardY+i > 7) break;
            else if(board[p.boardX-i][p.boardY+i].exists && pieces[board[p.boardX-i][p.boardY+i].pieceID].color == p.color) break;
            else if(board[p.boardX-i][p.boardY+i].exists){
                legal.push_back(board[p.boardX-i][p.boardY+i]);
                break;
            }
            else legal.push_back(board[p.boardX-i][p.boardY+i]);
        }
        for(int i=1; i<=1; i++){
            if(p.boardX+i > 7 || p.boardY-i < 0) break;
            else if(board[p.boardX+i][p.boardY-i].exists && pieces[board[p.boardX+i][p.boardY-i].pieceID].color == p.color) break;
            else if(board[p.boardX+i][p.boardY-i].exists){
                legal.push_back(board[p.boardX+i][p.boardY-i]);
                break;
            }
            else legal.push_back(board[p.boardX+i][p.boardY-i]);
        }

        //Roque
        if(!p.hasMoved){
            if(board[0][p.boardY].exists && !pieces[board[0][p.boardY].pieceID].hasMoved){
                bool isBlocked = false;
                for(int i=1; i<=3; i++){
                    if(board[i][p.boardY].exists || isThreatened(board, board[i][p.boardY], pieces, p.color)){
                        isBlocked = true;
                        break;
                    }
                }

                if(!isBlocked) legal.push_back(board[2][p.boardY]);
            }
        }
        if(!p.hasMoved){
            if(board[7][p.boardY].exists && !pieces[board[7][p.boardY].pieceID].hasMoved){
                bool isBlocked = false;
                for(int i=5; i<=6; i++){
                    if(board[i][p.boardY].exists || isThreatened(board, board[i][p.boardY], pieces, p.color)){
                        isBlocked = true;
                        break;
                    }
                }

                if(!isBlocked) legal.push_back(board[6][p.boardY]);
            }
        }
        break;
        //Cavalo
        case 'n':
        if(p.boardX+2 <= 7 && p.boardY+1 <= 7 && (!board[p.boardX+2][p.boardY+1].exists || pieces[board[p.boardX+2][p.boardY+1].pieceID].color != p.color)){
            legal.push_back(board[p.boardX+2][p.boardY+1]);
        }
        if(p.boardX+2 <= 7 && p.boardY-1 >= 0 && (!board[p.boardX+2][p.boardY-1].exists || pieces[board[p.boardX+2][p.boardY-1].pieceID].color != p.color)){
            legal.push_back(board[p.boardX+2][p.boardY-1]);
        }
        if(p.boardX-2 >= 0 && p.boardY+1 <= 7 && (!board[p.boardX-2][p.boardY+1].exists || pieces[board[p.boardX-2][p.boardY+1].pieceID].color != p.color)){
            legal.push_back(board[p.boardX-2][p.boardY+1]);
        }
        if(p.boardX-2 >= 0 && p.boardY-1 >= 0 && (!board[p.boardX-2][p.boardY-1].exists || pieces[board[p.boardX-2][p.boardY-1].pieceID].color != p.color)){
            legal.push_back(board[p.boardX-2][p.boardY-1]);
        }
        
        if(p.boardX+1 <= 7 && p.boardY+2 <= 7 && (!board[p.boardX+1][p.boardY+2].exists || pieces[board[p.boardX+1][p.boardY+2].pieceID].color != p.color)){
            legal.push_back(board[p.boardX+1][p.boardY+2]);
        }
        if(p.boardX+1 <= 7 && p.boardY-2 >= 0 && (!board[p.boardX+1][p.boardY-2].exists || pieces[board[p.boardX+1][p.boardY-2].pieceID].color != p.color)){
            legal.push_back(board[p.boardX+1][p.boardY-2]);
        }
        if(p.boardX-1 >= 0 && p.boardY+2 <= 7 && (!board[p.boardX-1][p.boardY+2].exists || pieces[board[p.boardX-1][p.boardY+2].pieceID].color != p.color)){
            legal.push_back(board[p.boardX-1][p.boardY+2]);
        }
        if(p.boardX-1 >= 0 && p.boardY-2 >= 0 && (!board[p.boardX-1][p.boardY-2].exists || pieces[board[p.boardX-1][p.boardY-2].pieceID].color != p.color)){
            legal.push_back(board[p.boardX-1][p.boardY-2]);
        }
        break;
    }
    return legal;
}

//Função para checar se movimento é legal
bool isLegal(std::vector<BoardData>legalMoves, BoardData target){
    for(int i=0; i<legalMoves.size(); i++){
        if((target.X == legalMoves[i].X) && (target.Y == legalMoves[i].Y)) return true;
    }
    return false;
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
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++){
        board[i][j].X = i;
        board[i][j].Y = j;
    }

    std::vector<Piece> pieces = setupBoard(board);

    // Loop principal
    int running = 1;
    SDL_Event event;
    int mouseX, mouseY;

    int turn = 0;

    bool movingPiece = false;
    int movingPieceID = -1;

    BoardData lastStart, lastEnd;
    lastStart.X = -1; //Para checar se algum movimento já foi feito
    std::vector<BoardData> legalMoves;

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
                if(board[mouseX/TILE_SIZE][mouseY/TILE_SIZE].exists && pieces[board[mouseX/TILE_SIZE][mouseY/TILE_SIZE].pieceID].color == turn){
                    movingPiece = true;
                    movingPieceID = board[mouseX/TILE_SIZE][mouseY/TILE_SIZE].pieceID;
                    legalMoves = getLegalMoves(board, pieces[movingPieceID], pieces, lastEnd);
                }
            }

            //Quando o botão é solto, a peça vai para o novo quadrado se estiver vazio
            if(event.type == SDL_MOUSEBUTTONUP && movingPiece)
            {
                SDL_GetMouseState(&mouseX, &mouseY);
                if(isLegal(legalMoves, board[mouseX/TILE_SIZE][mouseY/TILE_SIZE])){
                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].exists = false;
                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].pieceID = -1;
                    lastStart = board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY];
                    
                    pieces[movingPieceID].boardX = mouseX/TILE_SIZE;
                    pieces[movingPieceID].boardY = mouseY/TILE_SIZE;
                    pieces[movingPieceID].windowX = (mouseX/TILE_SIZE) * TILE_SIZE;
                    pieces[movingPieceID].windowY = (mouseY/TILE_SIZE) * TILE_SIZE;
                    lastEnd = board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY];

                    //Se houver uma peça no quadrado alvo, ela é capturada
                    if(board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].exists){
                        pieces[board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].pieceID].captured = true;
                    }

                    //Se a jogada for En Passant, captura o peão correspondente
                    if(pieces[movingPieceID].type == 'p' && lastEnd.X != lastStart.X && !board[lastEnd.X][lastEnd.Y].exists){
                        if(pieces[movingPieceID].color == 0){
                        pieces[board[lastEnd.X][lastEnd.Y+1].pieceID].captured = true;
                        board[lastEnd.X][lastEnd.Y+1].exists = false;
                        board[lastEnd.X][lastEnd.Y+1].pieceID = -1;
                        }
                        if(pieces[movingPieceID].color == 1){
                        pieces[board[lastEnd.X][lastEnd.Y-1].pieceID].captured = true;
                        board[lastEnd.X][lastEnd.Y-1].exists = false;
                        board[lastEnd.X][lastEnd.Y-1].pieceID = -1;
                        }
                    }

                    //Se um peão chegou ao fim do tabuleiro, promove para uma dama
                    if(pieces[movingPieceID].type == 'p' && (lastEnd.Y == 0 || lastEnd.Y == 7)){
                        pieces[movingPieceID].type = 'q';
                    }

                    //Se a jogada for um roque, move a torre correspondente
                    if(pieces[movingPieceID].type == 'k' && !pieces[movingPieceID].hasMoved && (lastEnd.X == 2 || lastEnd.X == 6)){
                        switch(lastEnd.X){
                            case 2:
                            board[3][lastEnd.Y].exists = true;
                            board[3][lastEnd.Y].pieceID = board[0][lastEnd.Y].pieceID;

                            pieces[board[0][lastEnd.Y].pieceID].boardX = 3;
                            pieces[board[0][lastEnd.Y].pieceID].windowX = 3*TILE_SIZE;

                            board[0][lastEnd.Y].exists = false;
                            board[0][lastEnd.Y].pieceID = -1;
                            break;
                            case 6:
                            board[5][lastEnd.Y].exists = true;
                            board[5][lastEnd.Y].pieceID = board[7][lastEnd.Y].pieceID;

                            pieces[board[7][lastEnd.Y].pieceID].boardX = 5;
                            pieces[board[7][lastEnd.Y].pieceID].windowX = 5*TILE_SIZE;

                            board[7][lastEnd.Y].exists = false;
                            board[7][lastEnd.Y].pieceID = -1;
                            break;
                        }
                    }

                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].exists = true;
                    board[pieces[movingPieceID].boardX][pieces[movingPieceID].boardY].pieceID = movingPieceID;

                    movingPiece = false;
                    pieces[movingPieceID].hasMoved = true;
                    movingPieceID = -1;

                    turn = turn == 0? 1 : 0;
                }
                //Se não estiver vazio, ela volta para o quadrado original
                else {
                    pieces[movingPieceID].windowX = pieces[movingPieceID].boardX * TILE_SIZE;
                    pieces[movingPieceID].windowY = pieces[movingPieceID].boardY * TILE_SIZE;
                    
                    movingPiece = false;
                    movingPieceID = -1;
                }
                legalMoves.clear();
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
        drawBoard(renderer, legalMoves, lastStart, lastEnd);
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