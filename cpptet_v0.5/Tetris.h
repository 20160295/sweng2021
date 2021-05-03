#pragma once
#include "Matrix.h"
#include <vector>

enum TetrisState { Running, NewBlock, Finished };

class Tetris {
public:
	static int nBlockTypes;
	static int nBlockDegrees;
	static vector<Matrix> setOfBlockObjects;
	static int iScreenDw;
	static int* arrayBlk_Size;
	int* topline;
	int iScreenDy;
	int iScreenDx;
	int idxBlockDegree;
	int idxBlockType;
	int* arrayScreen;
	int top;
	int left;
	TetrisState state;
	bool justStarted;
	Matrix currBlk;
	Matrix tempBlk;
	Matrix* iScreen;
	Matrix* oScreen;


	Tetris();
	Tetris(int dy, int dx);
	~Tetris();
	static void init(int** setOfBlockArrays, int max_blk_tpyes, int max_blk_degrees);
	int* createArrayScreen();
	TetrisState accept(char key);
	void deleteFullLines();


};