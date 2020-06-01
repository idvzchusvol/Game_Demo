//
//  Game.cpp
//  Demo_Ch1
//
//  Created by 彭力 on 2020/6/1.
//  Copyright © 2020 P&L. All rights reserved.
//

#include "Game.hpp"
#include <cstdlib>
#include <ctime>

const int width = 15;

const int SCREEN_WIDTH = 1024;

const int SCREEN_LENGTH = 768;

const int PADDLE_DIS = 10.0f;

const float paddleH = 100.0f;

const int BALL_NUMBER = 5;

const float SPEED = 300.0f;

Game :: Game() :
mWindow(nullptr), mRenderer(nullptr), mTicksCount(0),
mIsRunning(true) {}

bool Game :: Initialize() {
    
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    mWindow = SDL_CreateWindow("HANHAN_BALL", 100, 100, 1024, 768, 0);
    
    if (!mWindow) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    mRenderer = SDL_CreateRenderer(mWindow, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!mRenderer) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    
    Paddle[0].Dir = Paddle[1].Dir = 0;
    
    Paddle[0].Pos = (Vector) {PADDLE_DIS, SCREEN_LENGTH / 2.0f};
    Paddle[1].Pos = (Vector) {SCREEN_WIDTH - PADDLE_DIS, SCREEN_LENGTH / 2.0f};
    
    srand(time(NULL));
    
    for (int i = 0; i < BALL_NUMBER; i++) {
        double x = rand() % 200 + 1, y = rand() % 200 + 1;
        if (rand() % 2) x *= -1;
        if (rand() % 2) y *= -1;
        BALL STANDARD = (BALL) {(Vector) {SCREEN_LENGTH / 2.0f, SCREEN_WIDTH / 2.0f}, (Vector) {x, y}};
        ballset.push_back(STANDARD);
    }
    
    return true;
}

void Game :: RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game :: ProcessInput() {
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }
    
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    if (state[SDL_SCANCODE_ESCAPE]) mIsRunning = false;
    
    Paddle[0].Dir = Paddle[1].Dir = 0;
    
    if (state[SDL_SCANCODE_W]) Paddle[0].Dir -= 1;
    if (state[SDL_SCANCODE_S]) Paddle[0].Dir += 1;
    
    if (state[SDL_SCANCODE_I]) Paddle[1].Dir -= 1;
    if (state[SDL_SCANCODE_K]) Paddle[1].Dir += 1;
    
}

void Game :: BALL_CHECK(BALL &now) {
    
    float diff = now.Pos.y - Paddle[0].Pos.y;
    diff = diff > 0 ? diff : -diff;
    
    if (diff <= paddleH / 2.0f && now.Pos.x - width / 2.0f <= width + PADDLE_DIS
        && now.Vel.x <= 0.0f)
        now.Vel.x *= -1;
    
    diff = now.Pos.y - Paddle[1].Pos.y;
    diff = diff > 0 ? diff : -diff;
    
    if (diff <= paddleH / 2.0f && now.Pos.x >= SCREEN_WIDTH - PADDLE_DIS - width / 2.0f - width
        && now.Vel.x >= 0.0f)
        now.Vel.x *= -1;
        
    
}

void Game :: UpdateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    
    if (deltaTime > 0.05f) deltaTime = 0.05f;
    
    mTicksCount = SDL_GetTicks();
    
    for (int i = 0; i < 2; i++) {
        Paddle[i].Pos.y += Paddle[i].Dir * SPEED * deltaTime;
        
        if (Paddle[i].Pos.y < (1.0 * paddleH / 2.0f + width))
            Paddle[i].Pos.y = (1.0 * paddleH / 2.0f + width);
        
        if (Paddle[i].Pos.y > 1.0 * (SCREEN_LENGTH - width - paddleH / 2.0f))
            Paddle[i].Pos.y = 1.0 * SCREEN_LENGTH - width - paddleH / 2.0f;
        
    }
    
    for (int i = 0; i < BALL_NUMBER; i++) {
        
        ballset[i].Pos.x += ballset[i].Vel.x * deltaTime;
        ballset[i].Pos.y += ballset[i].Vel.y * deltaTime;
        
        if (ballset[i].Pos.x - width / 2.0f <= 0.0f)
            mIsRunning = false;
        
        if (ballset[i].Pos.x + width / 2.0f >= SCREEN_WIDTH)
            mIsRunning = false;
        
        if (ballset[i].Pos.y - width / 2.0f <= width && ballset[i].Vel.y <= 0.0f)
            ballset[i].Vel.y *= -1;
        
        if (ballset[i].Pos.y + width / 2.0f >= SCREEN_LENGTH - width && ballset[i].Vel.y > 0.0f)
            ballset[i].Vel.y *= -1;
        
        BALL_CHECK(ballset[i]);
        
    }
}

void Game :: GenerateOutput() {
    
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    
    SDL_RenderClear(mRenderer);
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    
    SDL_Rect wall {0, 0, SCREEN_WIDTH, width};
    
    SDL_RenderFillRect(mRenderer, &wall);
    
    wall = {0, SCREEN_LENGTH - width, SCREEN_WIDTH, width};
    
    SDL_RenderFillRect(mRenderer, &wall);
    
    SDL_Rect paddle {
        static_cast<int>(Paddle[0].Pos.x),
        static_cast<int>(Paddle[0].Pos.y - paddleH / 2.0f),
        width,
        static_cast<int>(paddleH)
    };
    
    SDL_RenderFillRect(mRenderer, &paddle);
    
    paddle = {
        static_cast<int>(Paddle[1].Pos.x - width),
        static_cast<int>(Paddle[1].Pos.y - paddleH / 2.0f),
        width,
        static_cast<int>(paddleH)
    };
    
    SDL_RenderFillRect(mRenderer, &paddle);
    
    for (int i = 0; i < BALL_NUMBER; i++) {
        SDL_Rect ball {
            static_cast<int>(ballset[i].Pos.x - width / 2.0f),
            static_cast<int>(ballset[i].Pos.y - width / 2.0f),
            width,
            width
        };
        SDL_RenderFillRect(mRenderer, &ball);
    }
    
    SDL_RenderPresent(mRenderer);
}

void Game :: Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
