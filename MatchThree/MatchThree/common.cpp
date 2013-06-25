//
//  common.cpp
//  MatchThree
//
//  Created by Ankit Jain on 6/8/13.
//
//

#include "common.h"

Globals * _global;

//std::string play_filenames[kKindCount] = {"heart.png","leaf.png","round.png","star.png"};
//std::string play_filenames[kKindCount] = {"block_1.png","block_2.png","block_3.png","block_4.png"};

std::string play_filenames[kKindCount] = {
    "ba_heart_plain.png",
    "ba_donut_plain.png",
    "ba_oval_plain.png",
    "ba_peanut_plain.png",
    "ba_square_plain.png",
    "ba_star_plain.png"
};
std::string play_vstripe_filenames[kKindCount] = {
    "ba_heart_vstripe.png",
    "ba_donut_vstripe.png",
    "ba_oval_vstripe.png",
    "ba_peanut_vstripe.png",
    "ba_square_vstripe.png",
    "ba_star_vstripe.png"
};
std::string play_hstripe_filenames[kKindCount] = {
    "ba_heart_hstripe.png",
    "ba_donut_hstripe.png",
    "ba_oval_hstripe.png",
    "ba_peanut_hstripe.png",
    "ba_square_hstripe.png",
    "ba_star_hstripe.png"
};
std::string play_wrapped_filenames[kKindCount] = {
    "ba_heart_wrapped.png",
    "ba_donut_wrapped.png",
    "ba_oval_wrapped.png",
    "ba_peanut_wrapped.png",
    "ba_square_wrapped.png",
    "ba_star_wrapped.png"
};

std::string play_color_burst_filename = "ba_star_vstripe.png";


std::string tile_bg_filename = "BG5.png";
//std::string bg_filename = "Blue_Sky_Green_Grass_Wide_1920x1200_1996.jpg";
std::string bg_filename = "laputa2.png";
std::string burst_effect_filename = "burst2.plist";


bool Globals::init()
{
    _startX = kStartX;
    _startY = kStartY;
    return true;
}

int Globals::getStartX() {
    return _startX;
}

int Globals::getStartY() {
    return _startY;
}

void Globals::setStartX(int x) {
    _startX = x;
}

void Globals::setStartY(int y) {
    _startY = y;
}
