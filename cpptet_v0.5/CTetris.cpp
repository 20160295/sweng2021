#include "CTetris.h"

vector<Matrix> CTetris::setOfCBlockObjects;

CTetris::CTetris(int cy, int cx) :Tetris(cy, cx) {
	arrayScreen = createArrayScreen();
	iCScreen = new Matrix(arrayScreen, iScreenDw + iScreenDy, iScreenDw * 2 + iScreenDx);
	oCScreen = new Matrix(iCScreen);
}

CTetris::~CTetris() {
	delete iCScreen;
	delete oCScreen;
	for (int i = 0; i < setOfCBlockObjects.capacity(); i++)
		delete& setOfCBlockObjects[i];
	setOfCBlockObjects.clear();
	delete& tempBlk;
	delete& currCBlk;
}

void CTetris::init(int** setOfBlockArrays, int max_blk_tpyes, int max_blk_degrees) {
	Tetris::init(setOfBlockArrays, max_blk_tpyes, max_blk_degrees);
	Matrix obj;

	for (int i = 0; i < nBlockTypes; i++) {
		for (int j = 0; j < nBlockDegrees; j++) {
			obj = Matrix(setOfBlockArrays[i * nBlockDegrees + j], arrayBlk_Size[i * nBlockDegrees + j], arrayBlk_Size[i * nBlockDegrees + j]);
			obj.mulc(i + 1);
			setOfCBlockObjects.push_back(obj);
		}
	}

}

TetrisState CTetris::accept(char key) {
	if (key >= '0' && key <= '6') {
		if (justStarted == false)
			deleteFullLines();
		delete iCScreen;
		iCScreen = new Matrix(oCScreen);
	}

	state = Tetris::accept(key);

	currCBlk = setOfCBlockObjects[idxBlockType * nBlockDegrees + idxBlockDegree];
	tempBlk = iCScreen->clip(top, left, top + currCBlk.get_dy(), left + currCBlk.get_dx());
	tempBlk = tempBlk.add(&currCBlk);

	delete oCScreen;
	oCScreen = new Matrix(iCScreen);
	oCScreen->paste(&tempBlk, top, left);
	return state;


}


void CTetris::deleteFullLines() {
	for (int i = 0; i < iScreenDy; i++) {
		tempBlk = oCScreen->clip(i, iScreenDw, i + 1, iScreenDw + iScreenDx);
		if (tempBlk.binary()->sum() == iScreenDx) {
			tempBlk = oCScreen->clip(0, iScreenDw, i, iScreenDw + iScreenDx);
			oCScreen->paste(&tempBlk, 1, iScreenDw);
			tempBlk = Matrix(topline, 1, iScreenDx);
			oScreen->paste(&tempBlk, 0, iScreenDw);
		}
	}
}