#include "Tetris.h"

int Tetris::iScreenDw = 0;
int Tetris::myMAX_BLK_TYPES = 0;
int Tetris::myMAX_BLK_DEGREES = 0;
std::vector<Matrix> Tetris::setofBlockObjects;

Tetris::Tetris(int dy, int dx){
    iScreenDy = dy;
    iScreenDx = dx;
    idxBlockDegree = 0;
    arrayscreen = createArrayscreen();
    iScreen = new Matrix(arrayscreen, dy+iScreenDw, dx+iScreenDw*2);
    oScreen = iScreen;
    juststarted = true;
    
};


void Tetris::init(int* setOfBlockArrays[], int MAX_BLK_TYPES, int MAX_BLK_DEGREES){
    myMAX_BLK_DEGREES = MAX_BLK_DEGREES;
    myMAX_BLK_TYPES = MAX_BLK_TYPES;
    int arrayBlk_maxsize = 0;
    std::vector<int> blockdxdy;
    for(int i=0; i<MAX_BLK_TYPES; i++){ 
        int k = 0;
        while(setOfBlockArrays[i*MAX_BLK_DEGREES][k] != -1){
            k++;
        }
        if (k > arrayBlk_maxsize)
            arrayBlk_maxsize = sqrt(k);
        blockdxdy.push_back(sqrt(k));
    }
    iScreenDw = arrayBlk_maxsize;
    for (int i=0; i<MAX_BLK_TYPES; i++){
        for(int j=0; j<MAX_BLK_DEGREES; j++){   
            setofBlockObjects.push_back(Matrix(setOfBlockArrays[i*MAX_BLK_DEGREES + j],
             blockdxdy[i], blockdxdy[i]));
        }
    }
    //initialize iscreenDw, setOfBlockObjects(Matrix)
};

TetrisState Tetris::accept(int key){
    state = Running;
    if(key - '0' >= 0 && key - '0' <= 6){
        if (juststarted == false)
            deleteFullLines();
        iScreen = new Matrix(oScreen);
        idxBlockType = key - '0';
        idxBlockDegree = 0;
        currBlk = setofBlockObjects[idxBlockType*myMAX_BLK_DEGREES + idxBlockDegree];
        top = 0;
        left = iScreenDw + iScreenDx/2 - currBlk.get_dy()/2;
        tempBlk = iScreen->clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
        tempBlk = tempBlk.add(&currBlk);
        juststarted = false;
        cout << "";
        if (tempBlk.anyGreaterThan(1))
            state = Finished;
        oScreen = new Matrix(iScreen);
        oScreen->paste(&tempBlk, top, left);
        return state;
    }
    else if(char(key) == 'a')
        left -= 1;
    else if(char(key) == 'd')
        left += 1;
    else if(char(key) == 'w'){
        tempDegree = idxBlockDegree;
        idxBlockDegree = (idxBlockDegree + 1) % myMAX_BLK_DEGREES;
        currBlk = setofBlockObjects[idxBlockType*myMAX_BLK_DEGREES + idxBlockDegree];
    }
    else if(char(key) == ' '){
        while(!tempBlk.anyGreaterThan(1)){
            top += 1;
            tempBlk = iScreen->clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
            tempBlk = tempBlk.add(&currBlk);        
        }
    }
    else if(char(key) == 's')
        top += 1;
    else 
        cout << "Wrong key \n";
    tempBlk = iScreen->clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
    tempBlk = tempBlk.add(&currBlk);
    if (tempBlk.anyGreaterThan(1)){
        if(char(key) == 'a')
            left += 1;
        else if(char(key) == 'd')
            left -= 1;
        else if(char(key) == 's'){
            top -= 1;
            state = NewBlock;
        }
        else if(char(key) == ' '){
            top -= 1;
            state = NewBlock;
        }
        else if(char(key) == 'w'){
            idxBlockDegree = tempDegree;
            currBlk = setofBlockObjects[idxBlockType*myMAX_BLK_DEGREES + idxBlockDegree];
        }
        tempBlk = iScreen->clip(top, left, top+currBlk.get_dy(), left+currBlk.get_dx());
        tempBlk = tempBlk.add(&currBlk);
    }
    oScreen = new Matrix(iScreen);
    oScreen->paste(&tempBlk, top, left);
    
    return state;
};





int* Tetris::createArrayscreen(){
    int arrayScreenDx = iScreenDw * 2 + iScreenDx;
    int arrayScreenDy = iScreenDw + iScreenDy;
    int* arrayscreen = new int[arrayScreenDy * arrayScreenDx];
    for(int i=0; i<arrayScreenDy-iScreenDw; i++){
        for(int j=0; j<iScreenDw; j++)
            arrayscreen[i*arrayScreenDx +j] = 1;
        for(int j=iScreenDw; j<arrayScreenDx-iScreenDw; j++)
            arrayscreen[i*arrayScreenDx +j] = 0;
        for(int j=arrayScreenDx-iScreenDw; j<arrayScreenDx; j++)

            arrayscreen[i*arrayScreenDx +j] = 1;
    }
    for(int i=arrayScreenDy-iScreenDw; i<arrayScreenDy; i++){ //segfault
        for(int j=0; j<arrayScreenDx; j++){
            arrayscreen[i*arrayScreenDx + j] = 1;
        }
    }
    return arrayscreen;
};

void Tetris::deleteFullLines(){
    for(int i=0; i<iScreenDy; i++){
        if(oScreen->clip(i, iScreenDw, i+1, iScreenDw+iScreenDx)->sum() == iScreenDx){
            oScreen->paste(oScreen->clip(0, iScreenDw, i, iScreenDw+iScreenDx), 1, iScreenDw);
        }    
    }
    return;
}

Tetris::~Tetris(){
    
}