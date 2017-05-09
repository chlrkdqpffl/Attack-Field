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

#define FRONT_MAX_DISTANCE 150
#define BACK_MAX_DISTANCE 75
#define MAXDISTANCE 300

enum SoundChannel
{
	eChannel_Bgm,
	eChannel_Effect,

	ChannelCount
};

struct CSound3D 
{
public:
	CSound3D() {};
	CSound3D(XMFLOAT3 listenerPos, XMFLOAT3 pos, XMFLOAT3 dir, float nowSpeed, float addSpeed, float maxVolume, Channel* channel);
	virtual ~CSound3D() {};

	void Update(float fTimeDelta);

public:
	XMFLOAT3	m_f3ListenerPosition = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_f3Position = XMFLOAT3(0, 0, 0);
	XMFLOAT3	m_f3Direction = XMFLOAT3(0, 0, 0);;
	float		m_fNowSpeed = 0.0f;
	float		m_fAddSpeed = 0.0f;
	float		m_fMaxVolume = 1.0f;
	Channel*	m_pChannel = nullptr;
};

class CSoundManager : public CSingletonManager<CSoundManager>
{
	System*			g_pSystem;
	Sound*			g_pSound[static_cast<int>(SoundTag::SoundCount)];
	Channel*		g_pChannel[ChannelCount];
	float			g_fMainVolume = 1.0f;
	list<CSound3D*> g_listSound3DContainer;

public:
	CSoundManager();
	virtual ~CSoundManager();

	virtual void InitializeManager() override;
	virtual void ReleseManager() override;

	void LoadAllSound();
	void LoadBGMSound();
	void LoadEffectSound();

	void Update(float fTimeDelta);
	void AllStop();
	void Play3DSound(SoundTag soundTag, XMFLOAT3 position, XMFLOAT3 direction, float nowSpeed, float addSpeed, float volume = 1.0f);
	void Play2DSound(SoundTag soundTag, float volume = 1.0f);
	void PlayBgm(SoundTag soundTag, float vol = 0.7f);

	void VolumeControl(float volume);
};