#include <SFML/Graphics.hpp>
#include <iostream>  
#include "objects.h"  
#include "objectlist.h" 

using namespace std;

int main()
{
	// initialize width and height of window  
	int width = 1920;
	int height = 1080;

	// create a sfml window 
	sf::RenderWindow window(sf::VideoMode(width, height), "Logic Simulator");

	// set fps to 60 
	window.setFramerateLimit(60);

	// create an ObjectList instance to keep track of all the objects 
	ObjectList list;

	// set default objects where located on the left side of window 
	list.set_default_objects(&window);

	while (window.isOpen())
	{
		// create an event object to keep track of all events 
		sf::Event event;

		// check if there are any events 
		while (window.pollEvent(event))
		{
			// close the window 
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			// check if user presses a key on the keyboard 
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Delete)
				{
					list.delete_pressed();
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					list.esc_pressed();
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					list.select_all();
				}
				else if (event.key.code == sf::Keyboard::Z)
				{
					list.Z_pressed(); 
				}
			}

			// check if user presses a mouse button 
			if (event.type == sf::Event::MouseButtonPressed)
			{
				// check if user presses left button 
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					// get the coordinates of where user is clicked  
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;

					// update all selected objects 
					list.selectedObject(x, y);

					// check if object is dropped in an available place in workbench 
					if (list.get_carry() && list.is_workbench(x, y) && list.is_available(x, y))
					{
						list.add_current();
						list.set_carry(false);
					}

					// check if user clicks on simulation area while not dragging an object 
					if (!list.get_carry() && list.is_simulation(x, y))
					{
						list.simulation_clicked(x, y);
					}

					// print coordinates of where user has clicked to console   
					// cout << "x: " << x << " y: " << y << endl;

					// check if user selects any object from left 
					if (list.is_left(x, y))
					{
						list.set_current(x, y);
					}
				}
			}

			// check if user moves mouse 
			if (event.type == sf::Event::MouseMoved)
			{
				// check if user drags a selected object 
				if (list.get_carry())
				{
					if (!list.is_current_null())
					{
						list.set_current_coordinates(float(event.mouseMove.x), event.mouseMove.y);
					}
					// cout << "x: " << event.mouseMove.x << " y: " << event.mouseMove.y << endl; 
				}
			}
		}

		// clear window 
		window.clear(sf::Color::Black);

		// show all necessary drawings 
		list.show_default_objects();
		list.show_everything();
		list.selected_shape();

		// display   
		window.display();
	}
	return 0;
}