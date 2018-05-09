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
    BoxGeometry *geometry = new BoxGeometry(1);

    StandardMaterial *material = new StandardMaterial();
    
    Mesh *mesh = new Mesh(geometry, material);
    scene->add(mesh);

    do {
        glfwPollEvents();
        
        runner->update();
        renderer->render(scene, camera);
        
        glfwSwapBuffers(window->window);
    } while(!window->shouldClose());
    
    window->hide();

	return 0;
}
