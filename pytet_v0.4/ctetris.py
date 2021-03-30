from tetris import *

class CTetris(Tetris):
    @classmethod
    def init(cls, setOfBlockArrays):
        Tetris.nBlockTypes = len(setOfBlockArrays)
        Tetris.nBlockDegrees = len(setOfBlockArrays[0])
        Tetris.setOfBlockObjects = [[0] * Tetris.nBlockDegrees for _ in range(Tetris.nBlockTypes)]
        arrayBlk_maxSize = 0
        for i in range(Tetris.nBlockTypes):
            if arrayBlk_maxSize <= len(setOfBlockArrays[i][0]):
                arrayBlk_maxSize = len(setOfBlockArrays[i][0])
        Tetris.iScreenDw = arrayBlk_maxSize     # larget enough to cover the largest block

        for i in range(Tetris.nBlockTypes):
            for j in range(Tetris.nBlockDegrees):
                Tetris.setOfBlockObjects[i][j] = Matrix(setOfBlockArrays[i][j])
                Matrix.mulc(Tetris.setOfBlockObjects[i][j],i+1)
                            
                
        return
    
    
    def accept(self, key):
        self.state = TetrisState.Running

        if key >= '0' and key <= '6':
            if self.justStarted == False:
                self.deleteFullLines()
            self.iScreen = Matrix(self.oScreen)
            self.idxBlockType = int(key)
            self.idxBlockDegree = 0
            self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
            self.top = 0
            self.left = Tetris.iScreenDw + self.iScreenDx//2 - self.currBlk.get_dx()//2
            self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
            self.tempBlk = self.tempBlk + self.currBlk
            self.justStarted = False
            print()

            if not self.compTempCurr(self.tempBlk,self.currBlk):
                    self.state = TetrisState.Finished
            self.oScreen = Matrix(self.iScreen)
            self.oScreen.paste(self.tempBlk, self.top, self.left)
            return self.state
        elif key == 'q':
            pass
        elif key == 'a': # move left
            self.left -= 1
        elif key == 'd': # move right
            self.left += 1
        elif key == 's': # move down
            self.top += 1
        elif key == 'w': # rotate the block clockwise
            self.idxBlockDegree = (self.idxBlockDegree + 1) % Tetris.nBlockDegrees
            self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
        elif key == ' ': # drop the block
            while self.compTempCurr(self.tempBlk,self.currBlk):
                    self.top += 1
                    self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
                    self.tempBlk = self.tempBlk + self.currBlk
        else:
            print('Wrong key!!!')
            
        self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
        self.tempBlk = self.tempBlk + self.currBlk

        if not self.compTempCurr(self.tempBlk,self.currBlk):   ## 벽 충돌시 undo 수행
            if key == 'a': # undo: move right
                self.left += 1
            elif key == 'd': # undo: move left
                self.left -= 1
            elif key == 's': # undo: move up
                self.top -= 1
                self.state = TetrisState.NewBlock
            elif key == 'w': # undo: rotate the block counter-clockwise
                self.idxBlockDegree = (self.idxBlockDegree - 1) % Tetris.nBlockDegrees
                self.currBlk = Tetris.setOfBlockObjects[self.idxBlockType][self.idxBlockDegree]
            elif key == ' ': # undo: move up
                self.top -= 1
                self.state = TetrisState.NewBlock
            
            self.tempBlk = self.iScreen.clip(self.top, self.left, self.top+self.currBlk.get_dy(), self.left+self.currBlk.get_dx())
            self.tempBlk = self.tempBlk + self.currBlk

        self.oScreen = Matrix(self.iScreen)
        self.oScreen.paste(self.tempBlk, self.top, self.left)

        return self.state
    
    def compTempCurr(self,tempBlk,currBlk):
        temp = tempBlk.get_array()
        curr = currBlk.get_array()
        for y in range(len(curr)):
            for x in range(len(curr[y])):
                if curr[y][x] != 0:
                    if curr[y][x] != temp[y][x]:
                        return False
        return True
    

    def deleteFullLines(self):
        self.tempBlk = self.oScreen.clip(0, self.iScreenDw, self.iScreenDy, self.iScreenDw+self.iScreenDx)
        temp_array = self.tempBlk.get_array()
        full_line = []
        for i in range(len(temp_array)):
            temp = 0
            for j in range(len(temp_array[i])):
                if temp_array[i][j] == 0:
                    temp = 1
                    break
            if temp == 0:
                full_line += [i]
        if len(full_line) != 0:
            for i in range(len(full_line)):
                self.tempBlk = self.oScreen.clip(0, self.iScreenDw, full_line[i], self.iScreenDw+self.iScreenDx)
                self.oScreen.paste(self.tempBlk, 1, self.iScreenDw)
            self.oScreen.paste(Matrix([[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]]), 0, self.iScreenDw)
        full_line = []
        
            
                           
        return
    
    
    
    
    
    
