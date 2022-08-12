#include <vector>
#include <iostream>

using namespace std;

class Level {
private:
//vector<u_short> map;
vector<vector<vector<u_short>>> level;
u_short map[99][99][99];
int width, height, depth;
public:
Level(int width, int height, int depth){
	this->width = width;
	this->height = height;
	this->depth = depth;
	//this->map = vector<u_short>(width*height*depth);
	for(int h = 0; h < width; h++){
		vector<vector<u_short>> vj;
		for(int j = 0; j < height; j++){
			vector<u_short> vi;
			for(int i = 0; i < depth; i++){
				vi.push_back(0);
			}
			vj.push_back(vi);
		}
		level.push_back(vj);
	}
}
void setBlock(int x, int y, int z, u_short block_type){
	//this->map[x+this->depth*(z+this->width*y)] = block_type;
	this->level[x][y][z] = block_type;
}
u_short getBlock(int x, int y, int z){
	//return this->map[x+this->depth*(z+this->width*y)];
	if(!this->hasBlock(x, y, z)) return 0;
	return this->level[x][y][z];
}
bool hasBlock(int x, int y, int z){
	return (0 <= x && x < this->width) && (0 <= y && y < this->height) && (0 <= z && z < this->depth);
}
int getWidth(){
	return this->width;
}
int getHeight(){
	return this->height;
}
int getDepth(){
	return this->depth;
}
};