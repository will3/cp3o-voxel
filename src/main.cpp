#include <GL/glew.h>
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"
#include "Runner.h"
#include "Camera.h"

int main() {
    Window *window = new Window();
    window->show(1024, 768);
    
    Renderer *renderer = new Renderer();
    renderer->window = window;

    Scene *scene = new Scene();
    Camera *camera = new PerspectiveCamera(60, 1.0, 0.1, 1000);
    Runner *runner = new Runner();
    
    do {
        glfwPollEvents();
        
        runner->update();
        renderer->render(scene, camera);
        
        glfwSwapBuffers(window->window);
    } while(glfwWindowShouldClose(window->window) == 0);
    
    window->hide();

	return 0;
}
