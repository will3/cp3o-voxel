#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Runner.h"
#include "Camera.h"
#include "BoxGeometry.h"
#include "StandardMaterial.h"
#include "Mesh.h"
#include "ShaderMaterial.h"
#include "Terrain.h"
#include "CameraControl.h"
#include <thread>
#include <chrono>
//#include "CL\cl.h"

int main() {
	//cl_uint platformIdCount = 0;
	//clGetPlatformIDs(0, nullptr, &platformIdCount);
	//std::vector<cl_platform_id> platformIds(platformIdCount);
	//clGetPlatformIDs(platformIdCount, platformIds.data(), nullptr);

	//cl_uint deviceIdCount = 0;
	//clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, 0, nullptr,
	//	&deviceIdCount);
	//std::vector<cl_device_id> deviceIds(deviceIdCount);
	//clGetDeviceIDs(platformIds[0], CL_DEVICE_TYPE_ALL, deviceIdCount,
	//	deviceIds.data(), nullptr);

	//const cl_context_properties contextProperties[] =
	//{
	//	CL_CONTEXT_PLATFORM,
	//	reinterpret_cast<cl_context_properties> (platformIds[0]),
	//	0, 0
	//};

	//cl_int error;
	//cl_context context = clCreateContext(
	//	contextProperties, deviceIdCount,
	//	deviceIds.data(), nullptr,
	//	nullptr, &error);

	Window *window = new Window();
    window->show(1024, 768);
    
    Renderer *renderer = new Renderer();
    renderer->window = window;
     renderer->clearColor = glm::vec3(0.3, 0.3, 0.3);
    
    Scene *scene = new Scene();
    Camera *camera = new PerspectiveCamera(60, window->aspect(), 0.1, 1000);
    camera->position.z = 10;
    
    Runner *runner = new Runner();

    Terrain *terrain = new Terrain();
    terrain->scene = scene;
    runner->add(terrain);
    
    CameraControl *cameraControl = new CameraControl();
    cameraControl->window = window;
    cameraControl->camera = camera;
    runner->add(cameraControl);
	terrain->cameraControl = cameraControl;

    double minInterval = 1 / 60.0;
    do {
        double startTime = glfwGetTime();
        glfwPollEvents();
        
        runner->update();
        renderer->render(scene, camera);
        
        glfwSwapBuffers(window->window);
        double endTime = glfwGetTime();
        double delta = endTime - startTime;
        if (delta < minInterval) {
            int milliseconds = (minInterval - delta) * 1000;
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }
    } while(!window->shouldClose());
    
    window->hide();

	return 0;
}
