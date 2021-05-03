#include "CTetris.h"
std::vector<Matrix> CTetris::setofCBlockObjects;

void CTetris::init(int* setOfBlockArrays[], int MAX_BLK_TYPES, int MAX_BLK_DEGREES){
    Tetris::init(setOfBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
    for(int i=0; i<MAX_BLK_TYPES; i++){
        for(int k=0; k<MAX_BLK_DEGREES; k++){
            Matrix temp = Matrix(setofBlockObjects[i*MAX_BLK_DEGREES + k]);
            temp.mulc(i+1);
            setofCBlockObjects.push_back(temp);
        }
    }
    return;
};

CTetris::CTetris(int cy, int cx) : Tetris(cy, cx){
    arrayscreen = createArrayscreen();
    iCScreen = new Matrix(arrayscreen, cy+iScreenDw, cx+iScreenDw*2);
    oCScreen = iCScreen;
    return;
};

TetrisState CTetris::accept(int key){
    if(key - '0' >= 0 && key - '0' <= 6){
        if (juststarted == false)
            deleteFullLines();
        iCScreen = new Matrix(oCScreen);
    }
    state = Tetris::accept(key);
    currCBlk = setofCBlockObjects[idxBlockType*myMAX_BLK_DEGREES + idxBlockDegree];
    tempBlk = iCScreen->clip(top, left, top+currCBlk.get_dy(), left+currCBlk.get_dy());
    tempBlk = tempBlk.add(&currCBlk);

    oCScreen = new Matrix(iCScreen);
    oCScreen->paste(&tempBlk, top, left);
    return state;
};

void CTetris::deleteFullLines(){
    for(int i=0; i<iScreenDy; i++){
        if(oCScreen->binary()->clip(i, iScreenDw, i+1, iScreenDw+iScreenDx)->sum() == iScreenDx){
            oCScreen->paste(oCScreen->clip(0, iScreenDw, i, iScreenDw+iScreenDx), 1, iScreenDw);
        }    
    }
    return;
};

CTetris::~CTetris(){

}