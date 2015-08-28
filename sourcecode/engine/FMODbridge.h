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
#include <engine.h>


typedef struct torch
{
    entity* t;
    FMOD::Studio::EventInstance* s;
    
    bool operator==(const torch &other)
    {
        return t == other.t;
    }
    
}torch;

typedef struct torches
{
    vector<torch> ts;
    int n_ts = 0;
    

    
    void add(entity* x, FMOD::Studio::EventInstance* s)
    {
       
        torch t;
        t.t = x;
        t.s = s;
        
        ts.put(t);
        n_ts++;
    };
    
    void remove(entity* x)
    {
        int p = find(x);
        if(p==-1) return;
        
        ts.removeobj(ts[p]);
        n_ts--;
    };
    
    int find(entity* x)
    {
        int res = -1;
        loopi(n_ts)
        {
            if(ts[i].t==x)
            {
                res = i;
                break;
            }
        }
        return res;
    }
    
    torch &operator[](int i) { ASSERT(i>=0 && i<n_ts); return ts[i]; }

    
    
}torches;

class FMODbridge
{
public:
    FMODbridge();
    //FMODbridge();
    
    void playOST();
    void playSound(char* name);
    void startTorch(entity* e);
    void stopTorch(entity* e);

private:
    
    torches buf_torches;
    static void* main_thread(void* data);
    
    
    bool OST;
    
    
    void *extraDriverData = NULL;
    
    FMOD::Studio::EventDescription* explosionDescription = NULL;
    FMOD::Studio::Bank* ambienceBank = NULL;
    FMOD::Studio::EventDescription* portale = NULL;
    FMOD::Studio::EventDescription* itemPick = NULL;
    FMOD::Studio::EventDescription* torcia = NULL;

    
    FMOD::Studio::EventDescription* loopingAmbienceDescription = NULL;
    FMOD::Studio::EventInstance* loopingAmbienceInstance = NULL;
    
    FMOD::Studio::Bank* weaponsBank = NULL;
    FMOD::Studio::Bank* monsters = NULL;
    FMOD::Studio::Bank* objects = NULL;
    FMOD::Studio::Bank* character = NULL;
    FMOD::Studio::Bank* speak = NULL;
    
};




#endif /* defined(__sauerbraten__FMODbridge__) */
