#pragma once

/*
Max Sampler State				: 16
Max Constant Buffer				: 14 (2개는 내부 사용)
Max Constant Buffer Size		: 4096 Byte
Max Resource(Texture, Buffer)	: 128

※ 주의 : 쉐이더 코드와 연동되므로 register 값과 일치해야 한다.
*/


// ============================================ //
// ============ Constant Buffers ============== //

// Vertex Shader
#define VS_CB_SLOT_CAMERA_VIEW_PROJECTION	0x00
#define VS_CB_SLOT_WORLD_MATRIX				0x01
#define VS_CB_SLOT_TEXTURE_MATRIX			0x02
#define VS_CB_SLOT_TERRAIN					0x03
#define VS_CB_SLOT_SKYBOX					0x04
#define VS_CB_SLOT_PROJECTION				0x05
#define VS_CB_SLOT_SHADOW					0x06
#define VS_CB_SLOT_CAMERA_POSITION			10

// Geometry Shader
#define GS_CB_SLOT_CAMERA_VIEW_PROJECTION	0x00
#define GS_CB_SLOT_PARTICLE					0x01

// Domain Shader
#define DS_CB_SLOT_CAMERA_VIEW_PROJECTION	0x00
#define DS_CB_SLOT_CAMERA_POSITION			10

// Computer Shader
#define CS_SLOT_WEIGHTS						0x00

// Pixel Shader
#define PS_CB_SLOT_LIGHT					0x00
#define PS_CB_SLOT_MATERIAL					0x01
#define PS_CB_SLOT_TERRAIN					0x03
#define PS_CB_SLOT_SKYBOX					0x04
#define PS_CB_SLOT_RENDEROPTION				0x05
#define PS_CB_SLOT_CAMERA_POSITION			10


// ============================================ //
// ================== Slot ==================== //
// texture
#define PS_SLOT_TEXTURE						0x00
#define PS_SLOT_TEXTURE_TERRAIN_DIFUSE		0x02
//		PS_SLOT_TEXTURE_TERRAIN_DETAIL		0x03
//		PS_SLOT_TEXTURE_TERRAIN_NORMAL		0x04
//		PS_SLOT_TEXTURE_TERRAIN_HEIGHT		0x05

#define PS_SLOT_TEXTURE_SKYBOX				0x0D
#define PS_SLOT_TEXTURE_CUBEMAPPED			0x0E
#define PS_SLOT_TEXTURE_PROJECTION			0x0F
#define PS_SLOT_TEXTURE_PROJECTED_DEPTH		0x10
#define PS_SLOT_TEXTURE_SHADOW				0x11
#define PS_SLOT_TEXTURE_SCREEN				0x12

#define PS_SLOT_TEXTURE_DIFFUSE				18
//		PS_SLOT_TEXTURE_NORMAL				19


// Sampler
#define PS_SLOT_SAMPLER						0x00
#define PS_SLOT_SAMPLER_DETAIL				0x01
#define PS_SLOT_SAMPLER_TERRAIN				0x02
//		PS_SLOT_SAMPLER_TERRAINDETAIL		0x03
#define PS_SLOT_SAMPLER_SKYBOX				0x04
#define PS_SLOT_SAMPLER_CUBEMAPPED			0x05
#define PS_SLOT_SAMPLER_PROJECTION			0x06
#define PS_SLOT_SAMPLER_SHADOW				0x07