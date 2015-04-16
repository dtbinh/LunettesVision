#pragma once

#include "Area.h"
#include <vector>

class Profile
{
public:
	Profile(void);
	~Profile(void);

	std::string name;
	std::vector<Area*> listArea;
};
