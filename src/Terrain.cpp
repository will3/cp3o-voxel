
#include "Terrain.h"
#include "VoxelBSP.h"

#define color_light_gray glm::ivec3(194, 195, 199)
#define color_dark_green glm::ivec3(0, 135, 81)

bool Terrain::getSolid(int i, int j, int k) {
    float gradient = 1 - j / (float)maxHeight;

    float solid = gradient + heightNoise.GetSimplexFractal(i, j * heightScale, k);
    return solid > 0.5;
}

void Terrain::start() {
	VoxelBSP bsp;
	bsp.set(0, 0, 0, 1);
	int result = bsp.get(0, 0, 0);
	if (result != 1) {
		throw std::runtime_error("oh no");
	}

	bsp.set(-1, -1, -1, 2);
	result = bsp.get(-1, -1, -1);
	if (result != 2) {
		throw std::runtime_error("oh no");
	}

	bsp.set(-33, -100, -9999, 3);
	result = bsp.get(-33, -100, -9999);
	if (result != 3) {
		throw std::runtime_error("oh no");
	}
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

	Coord3 offset = origin * CHUNK_SIZE;

	VoxelChunk *chunk = bsp.getChunk(offset.i, offset.j, offset.k);

	getColorFuncType getColor = [](Coord3 coord) {
		return glm::ivec3(255, 255, 255);
	};

    VoxelGeometry *geometry = Mesher::mesh(chunk, &bsp, getColor);
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

	Coord3 offset = origin * CHUNK_SIZE;

	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			for (int k = 0; k < chunkSize; k++) {
				bool solid = getSolid(i + offset.i, j + offset.j, k + offset.k);
				bsp.set(i + offset[0], j + offset[1], k + offset[2], solid ? 1 : 0);
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
