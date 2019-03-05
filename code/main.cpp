// Libraries
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

// Project
#include "Scene.hpp"


using namespace sf;
using namespace example;

static const size_t window_width  = 800;
static const size_t window_height = 600;

int main ()
{
    // Create the window and the scene that will be shown within the window:
    Window window(VideoMode(window_width, window_height), "3D Scene Render", Style::Titlebar | Style::Close, ContextSettings(32));
    Scene   scene  (window_width, window_height, "../../assets/scene.xml");

    // Initialization:
    window.setVerticalSyncEnabled (false);

    glDisable (GL_BLEND);
    glDisable (GL_DITHER);
    glDisable (GL_CULL_FACE);
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_TEXTURE_2D);

    glViewport     (0, 0, window_width, window_height);
    glMatrixMode   (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho        (0, GLdouble(window_width), 0, GLdouble(window_height), -1, 1);
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

    // Run the main loop:
    bool running = true;

    do
    {
        // Attend the window events:
        Event event;

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed)
            {
                running = false;
            }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				scene.camera->translation.add(2, 0, 0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				scene.camera->translation.add(-2, 0, 0);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				scene.camera->translation.add(0, 0, -5);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				scene.camera->translation.add(0,0, 5.f);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				scene.camera->translation.add(0, 2, 0);
	
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				scene.camera->translation.add(0, -2, 0);
        }

        // Update the view:
        scene.Update ();

        // Repaint the view:
        scene.Render ();

        // Swap the OpenGL buffers:
        window.display ();
    }
    while (running);

    // Close the application:
    return (EXIT_SUCCESS);
}
