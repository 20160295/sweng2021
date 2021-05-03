#include "Tetris.h"
#include <cmath>


int Tetris::nBlockTypes;
int Tetris::nBlockDegrees;
vector<Matrix> Tetris::setOfBlockObjects;
int Tetris::iScreenDw;
int* Tetris::arrayBlk_Size;

Tetris::Tetris() {
}

Tetris::Tetris(int dy, int dx) {
	iScreenDy = dy;
	iScreenDx = dx;
	idxBlockDegree = 0;
	arrayScreen = createArrayScreen();
	iScreen = new Matrix(arrayScreen, iScreenDw + iScreenDy, iScreenDw * 2 + iScreenDx);
	oScreen = new Matrix(iScreen);
	justStarted = true;
	topline = new int[iScreenDx] {};
}

Tetris::~Tetris() {
	delete[] topline;
	delete[] arrayBlk_Size;
	delete[] arrayScreen;
	delete iScreen;
	delete oScreen;
	for (int i = 0; i < setOfBlockObjects.capacity(); i++)
		delete& setOfBlockObjects[i];
	setOfBlockObjects.clear();
	delete& tempBlk;
	delete& currBlk;
}

void Tetris::init(int** setOfBlockArrays, int max_blk_types, int max_blk_degrees) {
	nBlockTypes = max_blk_types;
	nBlockDegrees = max_blk_degrees;

	int arrayBlk_maxSize = 0;
	arrayBlk_Size = new int[nBlockDegrees * nBlockTypes];

	for (int i = 0; i < nBlockTypes * nBlockDegrees; i++) {
		for (int j = 0;; j++) {
			if (setOfBlockArrays[i][j] == -1) {
				arrayBlk_Size[i] = sqrt(j);
				break;
			}
		}

		if (arrayBlk_Size[i] > arrayBlk_maxSize) {
			arrayBlk_maxSize = arrayBlk_Size[i];
		}
	}

	iScreenDw = arrayBlk_maxSize;


	for (int i = 0; i < nBlockTypes; i++) {
		for (int j = 0; j < nBlockDegrees; j++)
			setOfBlockObjects.push_back(Matrix(setOfBlockArrays[i * nBlockDegrees + j], arrayBlk_Size[i * nBlockDegrees + j], arrayBlk_Size[i * nBlockDegrees + j]));
	}


}

int* Tetris::createArrayScreen() {
	int arrayScreenDx = iScreenDw * 2 + iScreenDx;
	int arrayScreenDy = iScreenDw + iScreenDy;
	arrayScreen = new int[arrayScreenDy * arrayScreenDx];
	for (int y = 0; y < iScreenDy; y++) {
		for (int x = 0; x < iScreenDw; x++)
			arrayScreen[y * arrayScreenDx + x] = 1;
		for (int x = 0; x < iScreenDx; x++)
			arrayScreen[y * arrayScreenDx + iScreenDw + x] = 0;
		for (int x = 0; x < iScreenDw; x++)
			arrayScreen[y * arrayScreenDx + iScreenDw + iScreenDx + x] = 1;
	}

	for (int y = 0; y < iScreenDw; y++) {
		for (int x = 0; x < arrayScreenDx; x++)
			arrayScreen[iScreenDy * arrayScreenDx + y * arrayScreenDx + x] = 1;
	}

	return arrayScreen;
}

TetrisState Tetris::accept(char key) {
	state = Running;

	if (key >= '0' && key <= '6') {
		if (justStarted == false)
			deleteFullLines();
		delete iScreen;
		iScreen = new Matrix(oScreen);
		idxBlockType = key - '0';
		idxBlockDegree = 0;
		currBlk = setOfBlockObjects[idxBlockType * nBlockDegrees + idxBlockDegree];
		top = 0;
		left = iScreenDw + iScreenDx / 2 - currBlk.get_dx() / 2;
		tempBlk = iScreen->clip(top, left, top + currBlk.get_dy(), left + currBlk.get_dx());
		tempBlk = tempBlk.add(&currBlk);
		justStarted = false;
		cout << endl;

		if (tempBlk.anyGreaterThan(1))
			state = Finished;
		delete oScreen;
		oScreen = new Matrix(iScreen);
		oScreen->paste(&tempBlk, top, left);
		return state;
	}

	else if (key == 'q')
		;
	else if (key == 'a')
		left -= 1;
	else if (key == 'd')
		left += 1;
	else if (key == 's')
		top += 1;
	else if (key == 'w') {
		idxBlockDegree = (idxBlockDegree + 1) % nBlockDegrees;
		currBlk = setOfBlockObjects[idxBlockType * nBlockDegrees + idxBlockDegree];
	}
	else if (key == ' ') {
		while (!tempBlk.anyGreaterThan(1)) {
			top += 1;
			tempBlk = iScreen->clip(top, left, top + currBlk.get_dy(), left + currBlk.get_dx());
			tempBlk = tempBlk.add(&currBlk);
		}
	}
	else
		cout << "Wrong key" << endl;

	tempBlk = iScreen->clip(top, left, top + currBlk.get_dy(), left + currBlk.get_dx());
	tempBlk = tempBlk.add(&currBlk);

	if (tempBlk.anyGreaterThan(1)) {
		if (key == 'a')
			left += 1;
		else if (key == 'd')
			left -= 1;
		else if (key == 's') {
			top -= 1;
			state = NewBlock;
		}
		else if (key == 'w') {
			idxBlockDegree = (idxBlockDegree - 1) % nBlockDegrees;
			currBlk = setOfBlockObjects[idxBlockType * nBlockDegrees + idxBlockDegree];
		}
		else if (key == ' ') {
			top -= 1;
			state = NewBlock;
		}
		tempBlk = iScreen->clip(top, left, top + currBlk.get_dy(), left + currBlk.get_dx());
		tempBlk = tempBlk.add(&currBlk);
	}

	delete oScreen;
	oScreen = new Matrix(iScreen);
	oScreen->paste(&tempBlk, top, left);

	return state;
}

void Tetris::deleteFullLines() {
	for (int i = 0; i < iScreenDy; i++) {
		tempBlk = oScreen->clip(i, iScreenDw, i + 1, iScreenDw + iScreenDx);
		if (tempBlk.sum() == iScreenDx) {
			tempBlk = oScreen->clip(0, iScreenDw, i, iScreenDw + iScreenDx);
			oScreen->paste(&tempBlk, 1, iScreenDw);
			tempBlk = Matrix(topline, 1, iScreenDx);
			oScreen->paste(&tempBlk, 0, iScreenDw);
		}
	}
}