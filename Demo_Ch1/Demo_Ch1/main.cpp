//
//  main.cpp
//  Demo_Ch1
//
//  Created by 彭力 on 2020/6/1.
//  Copyright © 2020 P&L. All rights reserved.
//

#include <iostream>
#include "Game.hpp"

int main(int argc, const char * argv[]) {
    
    Game game;
    
    bool success = game.Initialize();
    
    if (success) game.RunLoop();
    
    game.Shutdown();
    
    return 0;
}
