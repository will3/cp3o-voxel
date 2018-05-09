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

class TerrianChunk {
public:
    bool dirty = true;
};

class Terrain : public Entity {
private:
    FastNoise heightNoise;
    std::map<Coord3, TerrianChunk *> map;
    int chunkSize = 32;
    Chunks<Voxel> *chunks = new Chunks<Voxel>();
    
public:
    int maxHeight = 128;
    float heightScale = 0.4;
    Scene *scene;

	Voxel get(int i, int j, int k);

    void update() override;
    
    void updateChunk(Coord3 origin);
};
