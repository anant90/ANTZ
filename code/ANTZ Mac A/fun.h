// Functions for telling intersection right/ left/ front/ back of current location
int rightIntX(int currentX, int currentDir) {
	if(currentDir==0) //i.e. North
		return currentX+1;
	else if(currentDir==2) //i.e. south
		return currentX-1;
	else return currentX;
}

int rightIntY(int currentY, int currentDir) {
	if(currentDir==1) //i.e. West
		return currentY+1;
	else if(currentDir==3) //i.e. East
		return currentY-1;
	else return currentY;
}

int leftIntX(int currentX, int currentDir) {
	if(currentDir==0) //i.e. North
		return currentX-1;
	else if(currentDir==2) //i.e. south
		return currentX+1;
	else return currentX;
}

int leftIntY(int currentY, int currentDir) {
	if(currentDir==1) //i.e. West
		return currentY-1;
	else if(currentDir==3) //i.e. East
		return currentY+1;
	else return currentY;
}

int frontIntX(int currentX, int currentDir) {
	if(currentDir==1) //i.e. West
		return currentX-1;
	else if(currentDir==3) //i.e. east
		return currentX+1;
	else return currentX;
}

int frontIntY(int currentY, int currentDir) {
	if(currentDir==0) //i.e. North
		return currentY+1;
	else if(currentDir==2) //i.e. South
		return currentY-1;
	else return currentY;
}

int backIntX(int currentX, int currentDir) {
	if(currentDir==1) //i.e. West
		return currentX+1;
	else if(currentDir==3) //i.e. east
		return currentX-1;
	else return currentX;
}

int backIntY(int currentY, int currentDir) {
	if(currentDir==0) //i.e. North
		return currentY-1;
	else if(currentDir==2) //i.e. South
		return currentY+1;
	else return currentY;
}
int abs(int a) {
	if(a<0) return -1*a;
	else return a;
}

int dist(int x1, int y1, int x2, int y2) {
	return (abs(x1-x2) + abs(y1-y2));
}