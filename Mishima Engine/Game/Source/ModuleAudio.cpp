#include "ModuleAudio.h"

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled) 
{

}

ModuleAudio::~ModuleAudio(){}

bool ModuleAudio::Init()
{
	LOG("Setting up ModuleAudio");
	bool ret = true;

	InitAudioEngine();

	return ret;
}

update_status ModuleAudio::PreUpdate(float dt) 
{

}

update_status ModuleAudio::Update(float dt) 
{

}

bool ModuleAudio::CleanUp()
{
	LOG("Cleaning ModuleAudio");

	return true;
}

bool ModuleAudio::InitAudioEngine() 
{
	//------------------ Memmory Manager ------------------//
	AkMemSettings memSettings;
	AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}

	//------------------ Streaming Manager ------------------//
    AkStreamMgrSettings stmSettings;
    AK::StreamMgr::GetDefaultSettings(stmSettings);

    if (!AK::StreamMgr::Create(stmSettings))
    {
        assert(!"Could not create the Streaming Manager");
        return false;
    }

    AkDeviceSettings deviceSettings;
    AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);


	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}

	//------------------ Sound Engine ------------------//
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	//------------------ Music Engine ------------------//
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

	//------------------ Spatial Audio ------------------//
	AkSpatialAudioInitSettings settings;

	if (AK::SpatialAudio::Init(&settings) != AK_Success)
	{
		assert(!"Could not initialize the Spatial Audio.");
		return false;
	}

	//------------------ Inializing communications ------------------//
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
	return true;
}