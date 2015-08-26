//
//  FMODbridge.cpp
//  sauerbraten
//
//  Created by Samuele De Giuseppe on 12/07/15.
//
//

#include "FMODbridge.h"

bool OST;
bool SHOT;

void *extraDriverData = NULL;
FMOD::Studio::System* fmod_sys;
FMOD::Studio::EventDescription* explosionDescription;
FMOD::Studio::Bank* ambienceBank;
FMOD::Studio::EventDescription* loopingAmbienceDescription;
FMOD::Studio::EventInstance* loopingAmbienceInstance;


int FMOD_Main();

FMODbridge::FMODbridge()
{
    FMOD_Main();
    
    OST = false;
    SHOT = false;
    
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
    
    ambienceBank = NULL;
    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("Surround_Ambience.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &ambienceBank) );
    
    FMOD::Studio::Bank* menuBank = NULL;
    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("UI_Menu.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &menuBank) );
    
    FMOD::Studio::Bank* weaponsBank = NULL;
    ERRCHECK( fmod_sys->loadBankFile(Common_MediaPath("Weapons.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &weaponsBank) );
    
    explosionDescription = NULL;
    ERRCHECK( fmod_sys->getEvent("event:/Explosions/Single Explosion", &explosionDescription) );
    
    // Start loading explosion sample data and keep it in memory
    ERRCHECK( explosionDescription->loadSampleData() );
    
    
    loopingAmbienceDescription = NULL;
    ERRCHECK( fmod_sys->getEvent("event:/Ambience/Country", &loopingAmbienceDescription) );
    
   loopingAmbienceInstance = NULL;
    ERRCHECK( loopingAmbienceDescription->createInstance(&loopingAmbienceInstance) );

   
    
    
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
        
        if(SHOT)
        {
            // One-shot event
            FMOD::Studio::EventInstance* eventInstance = NULL;
            ERRCHECK( explosionDescription->createInstance(&eventInstance) );
            
            ERRCHECK( eventInstance->start() );
            
            // Release will clean up the instance when it completes
            ERRCHECK( eventInstance->release() );
            SHOT = false;
        }
         
        
        ERRCHECK( fmod_sys->update() );

        Common_Sleep(10);
    } while (true);
    ERRCHECK( loopingAmbienceInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
    ERRCHECK( ambienceBank->unload() );

}

void FMODbridge::playOST()
{
    OST = true;
}




void FMODbridge::playSound(char* name)
{
    printf("NAME :- %s\n",name);
    SHOT = true;
    
}


