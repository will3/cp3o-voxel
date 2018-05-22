
#include "Terrain.h"
#include "VoxelBSP.h"
#include "Field3.h"

#define color_light_gray glm::ivec3(194, 195, 199)
#define color_dark_green glm::ivec3(0, 135, 81)

float Terrain::getDensity(int i, int j, int k) {
    float gradient = 1 - j / (float)maxHeight;

    return gradient + heightNoise.GetSimplexFractal(i, j * heightScale, k);
}

void Terrain::start() {

}

int terrain_fast_floor(int x, int y) {
	return x / y - (x % y < 0);
}

void Terrain::update() {
	if (cameraControl != 0) {
		int chunkSize = bsp.size;
		cameraOrigin = Coord3(
			terrain_fast_floor(cameraControl->position.x, chunkSize),
			0,
			terrain_fast_floor(cameraControl->position.z, chunkSize));
	}

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
				createChunk(Coord3(i, j, k) + cameraOrigin);
			}
		}
	}
}

void Terrain::drawChunks() {
	int chunkSize = bsp.size;
	int size = drawDis;
	int height = maxChunkY;
	for (int i = -size; i <= size; i++) {
		for (int k = -size; k <= size; k++) {
			for (int j = 0; j < height; j++) {
				drawChunk(Coord3(i, j, k) + cameraOrigin);
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

	Coord3 offset = origin * bsp.size;

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
	int size = bsp.size;

	if (tc->initialized) {
		return;
	}

	tc->initialized = true;

	Coord3 offset = origin * size;

	int sampleSize = chunkSize / 2 + 1;

	Field3<float> densityField(sampleSize);

	for (int i = 0; i < sampleSize; i++) {
		for (int j = 0; j < sampleSize; j++) {
			for (int k = 0; k < sampleSize; k++) {
				float density = getDensity(i * 2 + offset.i, j * 2 + offset.j, k * 2 + offset.k);
				densityField.set(i, j, k, density);
			}
		}
	}

	bsp.set(offset[0], offset[1], offset[2], 0); // Creates the chunk
	VoxelChunk *chunk = bsp.getChunk(offset.i, offset.j, offset.k);

	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			for (int k = 0; k < chunkSize; k++) {
				float density = densityField.sample((float)i, (float)j, (float)k, 2.0);
				bool solid = density > 0.5;
				chunk->set(i + offset[0], j + offset[1], k + offset[2], solid ? 1 : 0);
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
