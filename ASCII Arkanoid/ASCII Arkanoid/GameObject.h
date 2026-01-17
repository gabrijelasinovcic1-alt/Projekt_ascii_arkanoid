#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <vector>
#include "konstanta.h"
class GameObject {
public:
	virtual void update() {}
	virtual void draw(std::vector<std::vector<char>>& buffer) = 0;
	virtual ~GameObject() {}
};

#endif
