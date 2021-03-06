#pragma once
#include <map>
#include <glm/glm.hpp>
#include "FastNoise.h"
#include "Entity.h"
#include "Scene.h"
#include "types.h"
#include "Mesher.h"
#include "VoxelShader.h"
#include "ShaderMaterial.h"
#include "Mesh.h"
#include "VoxelBSP.h"
#include "CameraControl.h"

class TerrianChunk {
public:
    bool dirty = true;
	bool initialized = false;
};

class Terrain : public Entity {
private:
	VoxelBSP bsp;
    FastNoise heightNoise;
    std::map<Coord3, TerrianChunk *> map;
    int chunkSize = 32;
	void drawChunk(Coord3 origin);
	void createChunk(Coord3 &origin);
	TerrianChunk* get_or_create_chunk(Coord3 & origin);
	void drawChunks();

public:
    int maxHeight = 128;
    float heightScale = 0.4;
	int maxChunkY = 6;
	int drawDis = 4;
	CameraControl *cameraControl = 0;
	Coord3 cameraOrigin = Coord3();

    Scene *scene;

	float getDensity(int i, int j, int k);

	void start() override;
	void update() override;
	void createChunks();
};
