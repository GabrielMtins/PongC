
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

SDL_Window* window = NULL;
SDL_Renderer* render = NULL;

#define PI 3.141592

int end = 0;
int won = 0;

typedef struct{
    int x, y;
    SDL_Rect box;
} player;

player p1;
player p2;

typedef struct{
    double x, y;
    SDL_Rect box;
    double angle;
} ball;

ball mainBall;

SDL_Texture* playeronewin;
SDL_Texture* playertwowin;

void init(){
    window = SDL_CreateWindow("PONG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 576, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    p1.x = 0;
    p1.y = 576/2;
    p2.x = 1024-20;
    p2.y = 576/2;
    mainBall.x = 1024/2;
    mainBall.y = 576/2;
    mainBall.box.w = 16;
    mainBall.box.h = 16;
    mainBall.angle = PI/4;
    SDL_Surface* surface = SDL_LoadBMP("player1.bmp");
    playeronewin = SDL_CreateTextureFromSurface(render, surface);
    surface = SDL_LoadBMP("player2.bmp");
    playertwowin = SDL_CreateTextureFromSurface(render, surface);
    SDL_FreeSurface(surface);
}

void input(){
    Uint8* currentKeyState = SDL_GetKeyboardState(NULL);
    if(currentKeyState[SDL_SCANCODE_W]){
        if(p1.y >= 15) p1.y-=15;
    }
    if(currentKeyState[SDL_SCANCODE_S]){
        if(p1.y <= 576-15-150) p1.y+=15;
    }
    if(currentKeyState[SDL_SCANCODE_I]){
        if(p2.y >= 15) p2.y-=15;
    }
    if(currentKeyState[SDL_SCANCODE_K]){
        if(p2.y <= 576-15-150) p2.y+=15;
    }
}

int checkCollision(SDL_Rect a, SDL_Rect b){
    if(a.y+a.h <= b.y){
        return 0;
    }
    if(a.y >= b.y+b.h){
        return 0;
    }
    if(a.x+a.w <= b.x){
        return 0;
    }
    if(a.x >= b.x+b.w){
        return 0;
    }
    return 1;
}

void updateBallLogic(){
    mainBall.x += 10*cos(mainBall.angle);
    mainBall.y += 10*sin(mainBall.angle);
    mainBall.box.x = mainBall.x;
    mainBall.box.y = mainBall.y;
    if(checkCollision(mainBall.box, p1.box) || checkCollision(mainBall.box, p2.box) || mainBall.y < 0 || mainBall.y > 576-16){
        mainBall.angle+=PI/2;
    }
    if(mainBall.x < 0){
        end = 1;
        won = 2;
    }
    if(mainBall.x > 1024){
        end = 1;
        won = 1;
    }
}

void renderImage(){
    p1.box.x = p1.x;
    p1.box.y = p1.y;
    p1.box.w = 20;
    p1.box.h = 150;
    p2.box.x = p2.x;
    p2.box.y = p2.y;
    p2.box.w = 20;
    p2.box.h = 150;
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderFillRect(render, &p1.box);
    SDL_RenderFillRect(render, &p2.box);
    SDL_RenderFillRect(render, &mainBall.box);
    updateBallLogic();
    SDL_RenderPresent(render);
}

void gameloop(){
    int quit = 0;
    int maxdelay = 1000/60;
    Uint32 framestart;
    int framedelay;
    while(!quit){
        framestart = SDL_GetTicks();
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = 1;
            }
        }
        if(!end){
            input();
            renderImage();
        }
        else{
            if(won == 1){
                SDL_RenderCopy(render, playeronewin, NULL, NULL);
                SDL_RenderPresent(render);
            }
            else{
                SDL_RenderCopy(render, playertwowin, NULL, NULL);
                SDL_RenderPresent(render);
            }
            Uint8* state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_RETURN]){
                end = 0;
                won = 0;
                mainBall.x = 512;
                mainBall.y = 576/2;
                mainBall.angle = PI/4;
            }
        }
        framedelay = SDL_GetTicks()-framestart;
        if(framedelay<maxdelay){
            SDL_Delay(maxdelay-framedelay);
        }
    }
}

int main(int argc, char* args[]){
    init();
    gameloop();
    SDL_DestroyTexture(playeronewin);
    SDL_DestroyTexture(playertwowin);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    return 0;
}
