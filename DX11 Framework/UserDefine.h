#pragma once

#define PROJECT_NAME					L"Attack Field ("

//#define DEVELOP_MODE				// Ŭ���̾�Ʈ ���� ���
//#define FASTLOAD_MODE				// ���� �ε� ��� - �׽�Ʈ��

#define USE_SERVER					// ���� Ȱ��ȭ ���

#define USE_CONSOLE					// Open Console Windows
#define USE_AUTOIP					// Use Config File
#define USE_SOUND

#define USE_DEFERRD_RENDER

// ==================== Game System Option ==================== //
#define OCCUPY_TRYTIME					3000	// ���� �õ� �ð� 3��
#define OCCUPY_TIME						10000	// ���� �ð� 30��
#define ROUNDWAIT_TIME					6000	// ���� ���� 6�� �� �� ����
#define RESPAWN_TIME					5000	// ������ �ð� 5��
#define TOTAL_KILLSCORE					50		// ���� ���� ų
#define TOTAL_OCCUPYSCORE				2		// ���� ���� ���� ���ھ�
#define PLAYER_HP						100		// ĳ���� �⺻ HP
#define	DEATHMATCH_TIME					600		// ������ġ ���� �ð� 600��
// ============================================================ //

#define MAXBONECOUNT					60		// ĳ���� �ִ� ���� ����


//#define _WITH_MSAA4_MULTISAMPLING
#define _WITH_TERRAIN_PARTITION
#define _WITH_FRUSTUM_CULLING_BY_OBJECT
//#define _WITH_FRUSTUM_CULLING_BY_SUBMESH

//#define _WITH_SKYBOX_TEXTURE_ARRAY
#define _WITH_SKYBOX_TEXTURE_CUBE
//#define _WITH_TERRAIN_TEXTURE_ARRAY

// --------------------- Manager -------------------- //
#define STATEOBJ_MGR CStateObjectManager::GetInstance()
#define RESOURCE_MGR CResourceManager::GetInstance()
#define SCENE_MGR CSceneManager::GetInstance()
#define TEXT_MGR CTextManager::GetInstance()
#define TWBAR_MGR CAntTweakBarManager::GetInstance()
#define GLOBAL_MGR CGlobalVariableManager::GetInstance()
#define COLLISION_MGR CCollisionManager::GetInstance()
#define MAPDATA_MGR CMapDataManager::GetInstance()
#define SOUND_MGR CSoundManager::GetInstance()
#define SERVER_MGR CServerManager::GetInstance()
#define PARTICLE_MGR CParticleManager::GetInstance()
#define SPRITE_MGR CSpriteImageManager::GetInstance()
#define LIGHT_MGR CLightManager::GetInstance()
// -------------------------------------------------- //

// Check Timer
#define STARTTIMER(msg)			{ cout << msg; GLOBAL_MGR->g_startTime = chrono::system_clock::now();}

#define ENDTIMER_MILLI			{GLOBAL_MGR->g_GapMilliseconds = chrono::duration_cast<chrono::milliseconds> (chrono::system_clock::now() - GLOBAL_MGR->g_startTime); \
								 cout << GLOBAL_MGR->g_GapMilliseconds.count() << " milliseconds" << endl;}
#define ENDTIMER_MICRO			{GLOBAL_MGR->g_GapMicroseconds = chrono::duration_cast<chrono::microseconds> (chrono::system_clock::now() - GLOBAL_MGR->g_startTime); \
								 cout << GLOBAL_MGR->g_GapMicroseconds.count() << " microseconds" << endl;}
#define ENDTIMER_NANO			{GLOBAL_MGR->g_GapNanoseconds = chrono::duration_cast<chrono::nanoseconds> (chrono::system_clock::now() - GLOBAL_MGR->g_startTime); \
								 cout << GLOBAL_MGR->g_GapNanoseconds.count() << " nanoseconds" << endl;}

// Returns random float in [a, b).
#define RAND_FLOAT(a, b) float(a + ((float)(rand()) / (float)RAND_MAX) * (b - a))

//---------------------------------------------------------------------------------------
// Convenience macro for releasing, deleteing COM objects.
#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }
#define SafeDelete(x) { if(x) { delete x; x = nullptr;} }
#define SafeDeleteArray(x) { if(x) { delete[] x; x = nullptr; }}
//---------------------------------------------------------------------------------------

// ===== FRAME SIZE ���� ���� ===== //		--> UI ��ġ �ȸ���
#define FRAME_BUFFER_WIDTH		1600
#define FRAME_BUFFER_HEIGHT		900


// ���� ���� ��ũ�� D3DX11SaveTextureToFile(STATEOBJ_MGR->g_pd3dImmediateDeviceContext.Get(), &texture2d, D3DX11_IFF_JPG, L"save.jpg");

#define VK_A 0x41 
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44	
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

#if defined(DEBUG) || defined(_DEBUG)
	#ifndef HR
		#define HR(x) {HRESULT hr = (x); if (FAILED(hr)) DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);}
	#endif
#else
	#ifndef HR
		#define HR(x) (x)
	#endif
#endif