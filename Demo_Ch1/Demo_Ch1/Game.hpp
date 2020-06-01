//
//  Game.hpp
//  Demo_Ch1
//
//  Created by 彭力 on 2020/6/1.
//  Copyright © 2020 P&L. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <SDL2/SDL.h>
#include <vector>

struct Vector {
    double x, y;
};

struct PADDLE {
    int Dir;
    Vector Pos;
};

struct BALL {
    Vector Pos, Vel;
};

class Game {
private:
    SDL_Window* mWindow;
    
    SDL_Renderer* mRenderer;
    
    Uint32 mTicksCount;
    
    bool mIsRunning;
    
    PADDLE Paddle[2];
    
    std :: vector <BALL> ballset;
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    void BALL_CHECK(BALL &);
    
public:
    Game();
    
    bool Initialize();
    void RunLoop();
    void Shutdown();
};

#endif /* Game_hpp */
