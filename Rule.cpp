#include "Rule.h"


/*--------------------------------RULE 1D---------------------------------------- */

Rule1D::Rule1D() : num(0)
{

}

Rule1D::Rule1D(const std::uint8_t i) : num(0){
		this->num=i;
}
std::uint8_t Rule1D::getnum(){
		return this->num;
}
void Rule1D::setnum(const std::uint8_t i=0){
		this->num=i;
}
bool Rule1D::calcNextState(const Grid1D<bool> &grid, const Index1D index) const{
	Index1D idx; 
	idx=index;
	idx.i--;
	int x =grid.getCell(idx)*4;
	idx.i++;
	x = x + grid.getCell(idx)*2; 
	idx.i++;
	x = x + grid.getCell(idx);
	bool newVal = (bool) (this->num >>x) & 1;
	return newVal; 
}

/*--------------------------------RULE 2D---------------------------------------- */

Rule2D::Rule2D() : born(255), survive(0)
{

}

Rule2D::Rule2D(const std::uint8_t i, const std::uint8_t j)
{
	this->born=i;
	this->survive=j;
}
std::uint8_t Rule2D::getborn(){
		return this->born;
}
std::uint8_t Rule2D::getsurvive(){
		return this->survive;
}
void Rule2D::setborn(const std::uint8_t i=255){
	this->born=i;
}
void Rule2D::setsurvive(const std::uint8_t i=0){
	this->survive=i;
}
bool Rule2D::calcNextState(const Grid2D<bool> &grid, const Index2D index) const{
	Index2D idx;
	idx=index;
	int voisins = 0;
		idx.row--; idx.col--;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);
		idx.col++;
		voisins = voisins + grid.getCell(idx);
		idx.row--; idx.row--;
		voisins = voisins + grid.getCell(idx);
		idx.col++;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);
		idx.row++;
		voisins = voisins + grid.getCell(idx);
	bool newVal;
	if (grid.getCell(index)){ //si la cell est vivante
		newVal = (this->survive >>voisins) &1;
	}
	else{ // si la cellule est morte
		newVal = (this->born >>voisins) & 1;
	}
	return newVal;
}
