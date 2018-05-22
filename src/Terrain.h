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
    int maxHeight = 256;
    float heightScale = 0.4;
	int maxChunkY = 8;
	int drawDis = 4;

    Scene *scene;

	bool getSolid(int i, int j, int k);

	void start() override;
	void update() override;
	void createChunks();
};
