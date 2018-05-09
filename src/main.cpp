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

int main() {
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
