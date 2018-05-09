
#include "Terrain.h"

Voxel Terrain::get(int i, int j, int k) {
    float gradient = j / maxHeight;

    Voxel v{};
    float solid = gradient + heightNoise.GetSimplexFractal(i, j * heightScale, k);
    v.solid = solid > 0;
    v.color = glm::vec3(1.0, 1.0, 1.0);

    return v;
}

void Terrain::update() {
    int size = 2;
    int height = 2;
    for (int i = -size; i <= size; i++) {
        for(int k = -size; k <= size; k++) {
            for (int j = 0; j < height; j ++) {
                updateChunk(Coord3(i, j, k));
            }
        }
    }
}

void Terrain::updateChunk(Coord3 origin) {
    Coord3 offset = origin * chunkSize;
    if (map.find(origin) == map.end()) {
        map[origin] = new TerrianChunk();
    }

    TerrianChunk *tc = map[origin];

    if (!tc->dirty) {
        return;
    }

    tc->dirty = false;

    Chunk<Voxel> *chunk = chunks->get_or_create_chunk(origin);

    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
            for (int k = 0; k < chunkSize; k++) {
                Voxel v = get(i + offset.i, j + offset.j, k + offset.k);
                v.color = glm::vec3(1.0, 1.0, 1.0);
                Coord3 coord = Coord3(i, j, k);
                chunk->set(coord, v);
            }
        }
    }

    VoxelGeometry *geometry = Mesher::mesh(chunk, chunks);
    auto *material = new ShaderMaterial(new VoxelShader());
    auto *mesh = new Mesh(geometry, material);
    mesh->position = glm::vec3(offset.i, offset.j, offset.k);
    scene->add(mesh);
}
