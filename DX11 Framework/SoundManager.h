#pragma once
#include "SingletonManager.h"
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>

#ifdef _M_IX86		// 32비트
	#pragma comment (lib, "../Setting/FMOD/Lib/fmodex_vc.lib")
#elif _M_X64		// 64비트
	#pragma comment (lib, "../Setting/FMOD/Lib/fmodex64_vc.lib")
#endif

using namespace FMOD;

#define MAXDISTANCE 75
#define BACK_MAX_DISTANCE 50
static System*	g_pSystem;
static Sound*	g_pSound[static_cast<int>(SoundTag::SoundCount)];
static Channel*	g_pChannel;
static Channel*	g_pBGMChannel;
static Channel*	g_pThunderChannel;
static Channel*	g_pBeepChannel;
static float	g_fMainVolume = 1.0f;
static float	g_fBeepVolume = 0.0f;

struct CSound3D 
{
public:
	CSound3D() {};
	CSound3D(SoundTag tag, XMFLOAT3 pos, float maxVolume, Channel* channel);
	CSound3D(SoundTag tag, XMFLOAT3 pos, XMFLOAT3 dir, float nowSpeed, float addSpeed, float maxVolume, Channel* channel);
	virtual ~CSound3D() {};

	virtual void Update(float fTimeDelta);

public:
	XMFLOAT3	m_f3Position = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_f3Direction = XMFLOAT3(0, 0, 0);;
	float		m_fNowSpeed = 0.0f;
	float		m_fAddSpeed = 0.0f;
	float		m_fMaxVolume = 1.0f;
	SoundTag	m_tagSound = SoundTag::eNone;
	Channel*	m_pChannel = nullptr;
};

class CSoundManager : public CSingletonManager<CSoundManager>
{
	list<CSound3D*>		g_listSound3DContainer;	
	list<CSound3D*>		g_listSound3DEnvironmentContainer;
public:
#ifdef USE_SOUND
	CSoundManager() {}
	virtual ~CSoundManager() {}

	virtual void InitializeManager() override;
	void UpdateManager(float fTimeDelta);
	virtual void ReleseManager() override;

	void ClearContainer();
	void LoadAllSound();
	void LoadBGMSound();
	void LoadEffectSound();

	void Play3DSound(SoundTag soundTag, XMFLOAT3 position, XMFLOAT3 direction, float nowSpeed, float addSpeed, float volume = 1.0f);
	void StopSound();
	void StopBGMSound();
	void StopBeepSound();
	void Play2DSound(SoundTag soundTag, float volume = 1.0f);
	void PlayBgm(SoundTag soundTag, float vol = 0.7f);

	void AddVolume(float volume);
	bool IsMaxSound();
#else
	CSoundManager() {}
	virtual ~CSoundManager() {}

	virtual void InitializeManager() {};
	void UpdateManager(float fTimeDelta) {};
	virtual void ReleseManager() {};

	void ClearContainer() {}
	void LoadAllSound() {}
	void LoadBGMSound() {}
	void LoadEffectSound() {}

	void Play3DSound(SoundTag soundTag, XMFLOAT3 position, XMFLOAT3 direction, float nowSpeed, float addSpeed, float volume = 1.0f) {}
	void StopSound() {}
	void StopBGMSound() {}
	void StopBeepSound() {}
	void Play2DSound(SoundTag soundTag, float volume = 1.0f) {}
	void PlayBgm(SoundTag soundTag, float vol = 0.7f) {}

	void AddVolume(float volume) {}
	bool IsMaxSound() {}
#endif
};