#pragma once
#include "Tetris.h"

class CTetris : public Tetris {
    public:
        static std::vector<Matrix> setofCBlockObjects;
        Matrix* iCScreen;
        Matrix* oCScreen;
        Matrix currCBlk;
        Matrix tempBlk;
        int* arrayscreen;
        
        
        CTetris(int cy, int cx);
        ~CTetris();
        static void init(int *setofBlockArrays[], int MAX_BLK_TYPES, int MAX_BLK_DEGREES);
        TetrisState accept(int key);
        void deleteFullLines();
};