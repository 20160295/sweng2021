#pragma once
#include "Matrix.h"
#include <vector>
#include <cmath>
#include <string>

enum TetrisState { Running, NewBlock, Finished };

class Tetris {
    public:
        Matrix* oScreen;
        Matrix* iScreen;
        //Matrix* iScreen;
        Matrix tempBlk;
        Matrix currBlk;
        static int iScreenDw;
        static std::vector<Matrix> setofBlockObjects;
        static int myMAX_BLK_DEGREES;
        static int myMAX_BLK_TYPES;
        TetrisState state;
        int iScreenDx;
        int iScreenDy;
        int top;
        int left;
        int idxBlockType;
        int idxBlockDegree;
        int tempDegree;
        int* arrayscreen;
        bool juststarted;

        Tetris();
        ~Tetris();
        Tetris(int dy, int dx);
        static void init(int* setOfBlaockArrays[], int MAX_BLK_TYPES, int MAX_BLK_DEGREES);
        TetrisState accept(int key);
        int* createArrayscreen();
        void deleteFullLines();
        
};
