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
#define VS_CB_SLOT_BONETRANSFORM			0x07
#define VS_CB_SLOT_CAMERA_POSITION			10

// Hull Shader
#define HS_CB_SLOT_CAMERA_POSITION			10

// Domain Shader
#define DS_CB_SLOT_CAMERA_VIEW_PROJECTION	0x00
#define DS_CB_SLOT_CAMERA_POSITION			10

// Geometry Shader
#define GS_CB_SLOT_CAMERA_VIEW_PROJECTION	0x00
#define GS_CB_SLOT_PARTICLE					0x01

// Computer Shader
#define CS_SLOT_WEIGHTS						0x00

// Pixel Shader
#define PS_CB_SLOT_LIGHT					0x00
#define PS_CB_SLOT_MATERIAL					0x01
#define PS_CB_SLOT_TERRAIN					0x03
#define PS_CB_SLOT_SKYBOX					0x04
#define PS_CB_SLOT_RENDEROPTION				0x05
#define PS_CB_SLOT_GBUFFER_UNPACK			0x06

#define PS_CB_SLOT_CAMERA_POSITION			10


// ============================================ //
// ================== Slot ==================== //
// Texture

#define CS_TEXTURE_SLOT_BLUR				0x00

#define PS_TEXTURE_SLOT_DIFFUSE				0x00
//		PS_TEXTURE_SLOT_NORMAL				0x01
//		PS_TEXTURE_SLOT_DISPLACE			0x02
//		PS_TEXTURE_SLOT_SPEC				0x03

#define PS_TEXTURE_SLOT_TERRAIN_DIFUSE		0x04
//		PS_TEXTURE_SLOT_TERRAIN_DETAIL		0x05
//		PS_TEXTURE_SLOT_TERRAIN_NORMAL		0x06
//		PS_TEXTURE_SLOT_TERRAIN_HEIGHT		0x07

#define PS_TEXTURE_SLOT_SKYBOX				0x0D
#define PS_TEXTURE_SLOT_CUBEMAPPED			0x0E
#define PS_TEXTURE_SLOT_PROJECTION			0x0F
#define PS_TEXTURE_SLOT_PROJECTED_DEPTH		0x10
#define PS_TEXTURE_SLOT_SHADOW				0x11
#define PS_TEXTURE_SLOT_SCREEN				0x12
#define PS_TEXTRUE_SLOT_GBUFFER				19

// Sampler
#define PS_SAMPLER_SLOT						0x00
#define PS_SAMPLER_SLOT_DETAIL				0x01
#define PS_SAMPLER_SLOT_TERRAIN				0x02
//		PS_SAMPLER_SLOT_TERRAINDETAIL		0x03
#define PS_SAMPLER_SLOT_SKYBOX				0x04
#define PS_SAMPLER_SLOT_CUBEMAPPED			0x05
#define PS_SAMPLER_SLOT_PROJECTION			0x06
#define PS_SAMPLER_SLOT_SHADOW				0x07