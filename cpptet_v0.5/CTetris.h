#pragma once
#include "Tetris.h"

class CTetris : public Tetris {
public:
	static vector<Matrix> setOfCBlockObjects;
	Matrix* iCScreen;
	Matrix* oCScreen;
	Matrix currCBlk;
	Matrix tempBlk;

public:
	CTetris();
	CTetris(int dy, int dx);
	~CTetris();
	static void init(int** setOfBlockArrays, int max_blk_tpyes, int max_blk_degrees);
	TetrisState accept(char key);
	void deleteFullLines();


};