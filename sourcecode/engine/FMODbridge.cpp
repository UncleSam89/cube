//
//  FMODbridge.cpp
//  sauerbraten
//
//  Created by Samuele De Giuseppe on 12/07/15.
//
//

#include "FMODbridge.h"

bool OST;


void *extraDriverData = NULL;
FMOD::Studio::System* fmod_sys;

FMOD::Studio::EventDescription* explosionDescription = NULL;
FMOD::Studio::Bank* ambienceBank = NULL;
FMOD::Studio::EventDescription* portale = NULL;
FMOD::Studio::EventDescription* itemPick = NULL;


FMOD::Studio::EventDescription* loopingAmbienceDescription = NULL;
FMOD::Studio::EventInstance* loopingAmbienceInstance = NULL;

FMOD::Studio::Bank* weaponsBank = NULL;
FMOD::Studio::Bank* monsters = NULL;
FMOD::Studio::Bank* objects = NULL;
FMOD::Studio::Bank* character = NULL;
FMOD::Studio::Bank* speak = NULL;


int FMOD_Main();

FMODbridge::FMODbridge()
{
    FMOD_Main();
    
    OST = false;
    
    pthread_attr_t  attr;
    pthread_t       posixThreadID;
    int             returnVal;
    
    returnVal = pthread_attr_init(&attr);
    assert(!returnVal);
    returnVal = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    assert(!returnVal);
    
    int threadError = pthread_create(&posixThreadID, &attr, FMODbridge::main_thread, this);
    
    returnVal = pthread_attr_destroy(&attr);
    assert(!returnVal);
    if (threadError != 0)
    {
        // Report an error.
    }

}



int FMOD_Main()
{
    
    Common_Init(&extraDriverData);
    
    fmod_sys = NULL;
    ERRCHECK( FMOD::Studio::System::create(&fmod_sys) );
    
    // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
    FMOD::System* lowLevelSystem = NULL;
    ERRCHECK( fmod_sys->getLowLevelSystem(&lowLevelSystem) );
    ERRCHECK( lowLevelSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );
    
    ERRCHECK( fmod_sys->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );
    
    FMOD::Studio::Bank* masterBank = NULL;
    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("Master Bank.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );

    FMOD::Studio::Bank* stringsBank = NULL;
    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("Master Bank.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );

    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("Ambientale.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &ambienceBank) );

    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("Armi.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &weaponsBank) );

    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("Oggetti.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &objects) );
    
    ERRCHECK( fmod_sys->getEvent("event:/Ambientali/portalentrata", &portale) );

    ERRCHECK( fmod_sys->getEvent("event:/Oggetti/heal", &itemPick) );
    
    // Start loading explosion sample data and keep it in memory
    ERRCHECK( portale->loadSampleData() );
    ERRCHECK( itemPick->loadSampleData() );

    return 0;
}




void* FMODbridge::main_thread(void* data)
{
    
    do
    {
        Common_Update();
        
        if(OST)
        {
            ERRCHECK( loopingAmbienceInstance->start() );
            OST = false;
        }
        
        
        
        ERRCHECK( fmod_sys->update() );

        Common_Sleep(10);
    } while (true);
    ERRCHECK( loopingAmbienceInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
    ERRCHECK( ambienceBank->unload() );

}

void FMODbridge::playOST()
{
    //OST = true;
}




void FMODbridge::playSound(char* name)
{
    if(!strcmp(name,"q009/pistol1") || !strcmp(name,"q009/pistol2") || !strcmp(name,"q009/pistol3") )
    {

        /*
        FMOD::Studio::EventInstance* eventInstance = NULL;
        ERRCHECK( explosionDescription->createInstance(&eventInstance) );
        
        ERRCHECK( eventInstance->start() );
        
        // Release will clean up the instance when it completes
        ERRCHECK( eventInstance->release() );
        */
    }
    
    if(!strcmp(name,"q009/teleport"))
    {
        FMOD::Studio::EventInstance* eventInstance = NULL;
        ERRCHECK( portale->createInstance(&eventInstance) );
        
        ERRCHECK( eventInstance->start() );
        ERRCHECK( eventInstance->release() );
        
    }
    
    if(!strcmp(name,"aard/itempick"))
    {
        FMOD::Studio::EventInstance* eventInstance = NULL;
        ERRCHECK( itemPick->createInstance(&eventInstance) );
        ERRCHECK( eventInstance->start() );
        ERRCHECK( eventInstance->release() );
        
    }
    
    
    
    //printf("NAME :- %s\n",name);
    
}


