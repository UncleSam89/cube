//
//  FMODbridge.h
//  sauerbraten
//
//  Created by Samuele De Giuseppe on 12/07/15.
//
//

#ifndef __sauerbraten__FMODbridge__
#define __sauerbraten__FMODbridge__

#include <stdio.h>
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"
#include <assert.h>
#include <pthread.h>


class FMODbridge
{
public:
    FMODbridge();
    //FMODbridge();
    
    void playOST();
    void playSound();

private:
 

    static void* main_thread(void* data);
    
};




#endif /* defined(__sauerbraten__FMODbridge__) */
