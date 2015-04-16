#pragma once

#include <iostream>
#include <deque>

class OsdNode
{
public:

	enum Effect {
		BACK,
		QUIT,
		NONE,
		SWAP,
		SWAP_REMAP,
		SWAP_CROP,
		SWAP_HDR,
		HIDE_ZONE,
		CHANGE_ZONE,
		SAVE,
		ZONE_HEIGHT_INC,
		ZONE_HEIGHT_DEC,
		ZONE_WIDTH_INC,
		ZONE_WIDTH_DEC,
		ZONE_X_INC,
		ZONE_X_DEC,
		ZONE_Y_INC,
		ZONE_Y_DEC,
		ZOOM_REMAP,
		DEZOOM_REMAP
	};

	OsdNode(std::string s, Effect e);
	OsdNode(std::string s);
	~OsdNode(void);
	OsdNode* addChild(OsdNode* n);

	Effect effect;
	OsdNode* parent;
	std::deque<OsdNode*> children;
	std::string text;
};
