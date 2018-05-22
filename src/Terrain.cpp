
#include "Terrain.h"

#define color_light_gray glm::ivec3(194, 195, 199)
#define color_dark_green glm::ivec3(0, 135, 81)

Voxel Terrain::get(int i, int j, int k) {
    float gradient = 1 - j / (float)maxHeight;

    Voxel v{};
    float solid = gradient + heightNoise.GetSimplexFractal(i, j * heightScale, k);
    v.solid = solid > 0.5;
    v.color = color_dark_green;

    return v;
}

void Terrain::update() {
	createChunks();
	drawChunks();
}

void Terrain::createChunks()
{
	int size = drawDis + 1;
	int height = maxChunkY;
	for (int i = -size; i <= size; i++) {
		for (int k = -size; k <= size; k++) {
			for (int j = 0; j < height; j++) {
				createChunk(Coord3(i, j, k));
			}
		}
	}
}

void Terrain::drawChunks() {
	int size = drawDis;
	int height = maxChunkY;
	for (int i = -size; i <= size; i++) {
		for (int k = -size; k <= size; k++) {
			for (int j = 0; j < height; j++) {
				drawChunk(Coord3(i, j, k));
			}
		}
	}
}

void Terrain::drawChunk(Coord3 origin) {
	TerrianChunk *tc = get_or_create_chunk(origin);

	if (!tc->dirty) {
		return;
	}

	tc->dirty = false;

	auto chunk = chunks->get_chunk(origin);
	auto offset = chunk->get_offset();
    VoxelGeometry *geometry = Mesher::mesh(chunk, chunks);
    auto *material = new ShaderMaterial(new VoxelShader());
    auto *mesh = new Mesh(geometry, material);
    mesh->position = glm::vec3(offset.i, offset.j, offset.k);
    scene->add(mesh);
}

void Terrain::createChunk(Coord3 &origin)
{
	TerrianChunk *tc = get_or_create_chunk(origin);

	if (tc->initialized) {
		return;
	}

	tc->initialized = true;

	Chunk<Voxel> *chunk = chunks->get_or_create_chunk(origin);
	Coord3 offset = chunk->get_offset();

	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			for (int k = 0; k < chunkSize; k++) {
				Voxel v = get(i + offset.i, j + offset.j, k + offset.k);
				Coord3 coord = Coord3(i, j, k);
				chunk->set(coord, v);
			}
		}
	}
}

TerrianChunk* Terrain::get_or_create_chunk(Coord3 & origin)
{
	Coord3 offset = origin * chunkSize;
	if (map.find(origin) == map.end()) {
		map[origin] = new TerrianChunk();
	}

	TerrianChunk *tc = map[origin];
	return tc;
}
