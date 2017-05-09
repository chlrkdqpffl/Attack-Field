#include "stdafx.h"
#include "SoundManager.h"


CSound3D::CSound3D(XMFLOAT3 listenerPos, XMFLOAT3 pos, XMFLOAT3 dir, float nowSpeed, float addSpeed, float maxVolume, Channel* channel)
	: m_f3ListenerPosition(listenerPos), m_f3Position(pos), m_f3Direction(dir), m_fNowSpeed(nowSpeed), m_fAddSpeed(addSpeed), m_fMaxVolume(maxVolume), m_pChannel(channel)
{
}

void CSound3D::Update(float fTimeDelta)
{
	XMVECTOR position = XMLoadFloat3(&m_f3Position);
	XMVECTOR listenerPosition = XMLoadFloat3(&m_f3ListenerPosition);

	if ( XMVectorGetX(XMVector3LengthEst(position - listenerPosition)) > MAXDISTANCE)
		m_pChannel->stop();
	
	m_fNowSpeed += m_fAddSpeed;
	position += m_fNowSpeed * XMLoadFloat3(&m_f3Direction);

	float maxDistance = FRONT_MAX_DISTANCE;
	float volume = 1.0f;

	XMVECTOR direction = XMVectorSet(m_f3Position.x - m_f3ListenerPosition.x, m_f3Position.y - m_f3ListenerPosition.y, m_f3Position.z - m_f3ListenerPosition.z, 0.0f);
	if (XMVectorGetZ(direction) < 0) 
		maxDistance = BACK_MAX_DISTANCE;
	
	volume = ((maxDistance - XMVectorGetX(XMVector3LengthEst(direction))) / maxDistance) * m_fMaxVolume;
	
	if (volume <= 0) 
		m_pChannel->stop();
	else 
		m_pChannel->setVolume(volume);
}


// ----------------------------------------------------------------------------------------------------------------------------- //
// ---------------------------------------------------- Sound Manager ---------------------------------------------------------- //
CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
}

void CSoundManager::InitializeManager()
{
	System_Create(&g_pSystem);
	g_pSystem->init(64, FMOD_INIT_NORMAL, 0);
	LoadAllSound();
}

void CSoundManager::ReleseManager()
{
	g_pSystem->release();
	g_pSystem->close();

	for (auto& sound : g_listSound3DContainer)
		SafeDelete(sound);
}

void CSoundManager::LoadAllSound()
{
	LoadBGMSound();
	LoadEffectSound();
}

void CSoundManager::LoadBGMSound()
{
//	g_pSystem->createStream("../Assets/Sound/BGM/Audiomachine.mp3",			FMOD_LOOP_NORMAL, 0, &g_pSound[static_cast<int>(SoundTag::eBGM_TitleScene)]);	
	g_pSystem->createStream("../Assets/Sound/BGM/normalmode.mp3",			FMOD_LOOP_NORMAL, 0, &g_pSound[static_cast<int>(SoundTag::eBGM_TitleScene)]);	
}
 
void CSoundManager::LoadEffectSound()
{
	g_pSystem->createSound("../Assets/Sound/Effect/Fire.mp3",			FMOD_HARDWARE, 0, &g_pSound[static_cast<int>(SoundTag::eFire)]);
	g_pSystem->createSound("../Assets/Sound/Effect/Reload.mp3",			FMOD_HARDWARE, 0, &g_pSound[static_cast<int>(SoundTag::eReload)]);
	g_pSystem->createSound("../Assets/Sound/Effect/Walk.mp3",			FMOD_HARDWARE, 0, &g_pSound[static_cast<int>(SoundTag::eWalk)]);
	g_pSystem->createSound("../Assets/Sound/Effect/Run.mp3",			FMOD_HARDWARE, 0, &g_pSound[static_cast<int>(SoundTag::eRun)]);
	g_pSystem->createSound("../Assets/Sound/Effect/Death.mp3",			FMOD_HARDWARE, 0, &g_pSound[static_cast<int>(SoundTag::eDeath)]);
	g_pSystem->createSound("../Assets/Sound/Effect/ShellsFall.mp3",		FMOD_HARDWARE, 0, &g_pSound[static_cast<int>(SoundTag::eShellsFall)]);
}

void CSoundManager::Update(float fTimeDelta)
{
	g_pSystem->update();

	for (auto& iter = g_listSound3DContainer.begin(); iter != g_listSound3DContainer.end(); ) {
		bool bIsPlay = false;
		(*iter)->m_pChannel->isPlaying(&bIsPlay);
	//	(*iter)->m_pChannel->getPaused(&bIsPlay);
		if (bIsPlay) 
			(*iter)->Update(fTimeDelta);
		else 
			g_listSound3DContainer.erase(iter++);
	}
}

void CSoundManager::AllStop()
{
	for(int i = 0; i < ChannelCount; ++i) 
		g_pChannel[i]->stop();
}

void CSoundManager::Play3DSound(SoundTag soundTag, XMFLOAT3 position, XMFLOAT3 direction, float nowSpeed, float addSpeed, float volume)
{
	FMOD_RESULT result;
	g_pChannel[eChannel_Effect]->setVolume(volume);
	result = g_pSystem->playSound(FMOD_CHANNEL_FREE, g_pSound[static_cast<int>(soundTag)], false, &g_pChannel[eChannel_Effect]);
//	g_listSound3DContainer.push_back(new CSound3D(SCENE_MGR->g_pCamera->GetPosition(), position, direction, nowSpeed, addSpeed, volume, g_pChannel[eChannel_Effect]));
}

void CSoundManager::PlayBgm(SoundTag soundTag, float volume)
{
	g_pChannel[eChannel_Bgm]->setVolume(volume);
	g_pSystem->playSound(FMOD_CHANNEL_FREE, g_pSound[static_cast<int>(soundTag)], 0, &g_pChannel[eChannel_Bgm]);
}

void CSoundManager::Play2DSound(SoundTag soundTag, float volume) 
{
	g_pChannel[eChannel_Effect]->setVolume(volume);
	g_pSystem->playSound(FMOD_CHANNEL_FREE, g_pSound[static_cast<int>(soundTag)], false, &g_pChannel[eChannel_Effect]);
}

void CSoundManager::VolumeControl(float volume)
{
	g_fMainVolume += volume;

	if (1.0f < g_fMainVolume)
		g_fMainVolume = 1.0f;
	else if (g_fMainVolume < 0)
		g_fMainVolume = 0.0f;

	for (int i = 0; i < ChannelCount; ++i)
		g_pChannel[i]->setVolume(g_fMainVolume);
}