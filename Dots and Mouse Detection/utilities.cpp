//
//  utilities.cpp
//  Dots and Mouse Detection
//
//  Created by Tyler Doll on 12/9/15.
//  Copyright © 2015 Tyler Doll. All rights reserved.
//

#include "utilities.hpp"
#include <stdlib.h>

int randInt(int min = 0, int max = RAND_MAX) {
    return rand() % max + min;
}