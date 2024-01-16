#pragma once
#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ComponentCamera.h"

//------------------ Wwise Includes ------------------//
#include <../include AK/SoundEngine/Common/AkMemoryMgr.h>
#include <../include AK/SoundEngine/Common/AkModule.h>
#include <../include AK/SoundEngine/Common/IAkStreamMgr.h> 
#include <../include AK/Tools/Common/AkPlatformFuncs.h>
#include <../include AK/SoundEngine/Win32/AkDefaultIOHookDeferred.h>
#include <../include AK/SoundEngine/Win32/AkFilePackageLowLevelIOBlocking.h>
#include <../include AK/SoundEngine/Common/AkSoundEngine.h>
#include <../include AK/MusicEngine/Common/AkMusicEngine.h>   
#include <../include AK/SpatialAudio/Common/AkSpatialAudio.h>
#ifndef AK_OPTIMIZED
#include <../include AK/Comm/AkCommunication.h>
#endif

class ModuleAudio : public Module
{
public:
	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	update_status PreUpdate(float dt) override;
	update_status Update(float dt) override;
	bool CleanUp();

	//------------------ Wwise Funcions ------------------//
	bool InitAudioEngine();
	void ProcessAudio();
	void ModuleAudio::SetDefaultListener(const AkGameObjectID id);

	//------------------ ID Funcions ------------------//
	AkUInt32 ChangeNametoID(std::string name);

public:
	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	AkUInt32 ListenersNum;
};