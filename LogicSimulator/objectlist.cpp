#include "objectlist.h" 
#include <iostream>   
#include <cmath> 

using namespace std;

// default constructor for ObjectList class 
ObjectList::ObjectList()
{
	head = nullptr;
	set_common();
}

// constructor for ObjectList class with arguments 
ObjectList::ObjectList(Object* object)
{
	head = new Node;
	head->object = object;
	head->next = nullptr;
	set_common();
}

// function that is called by both constructors to initialize common variables 
void ObjectList::set_common()
{
	window = nullptr;
	current = nullptr;
	carry = false;
}

// add an object to linked list 
void ObjectList::add(Object* object)
{
	if (head == nullptr)
	{
		head = new Node;
		head->object = object;
		head->next = nullptr;

		return;
	}
	Node* temp = head;
	while (temp->next != nullptr)
	{
		temp = temp->next;
	}
	Node* emptoadd = new Node;
	emptoadd->object = object;
	emptoadd->next = nullptr;
	temp->next = emptoadd;
}

// print linked list 
void ObjectList::print()
{
	if (head == nullptr)
	{
		cout << "Object list is empty" << endl;
		return;
	}
	int i = 1;
	Node* temp = head;
	while (temp)
	{
		cout << i << ".th element: " << temp->object->type << " id: " << temp->object->getId() << endl;
		temp = temp->next;
		i++;
	}
}

// delete an object with given id from linked list 
void ObjectList::delete_object(int id)
{
	if (head == nullptr) 
	{
		cout << "Object list is empty" << endl;
		return;
	}
	if (head->next == nullptr)
	{
		if (head->object->id == id)
		{
			Node* temp_head = head;
			head = nullptr;
			delete temp_head;
			return;
		}
		return;
	}
	if (head->object->id == id)
	{
		Node* temp_head = head;
		head = head->next;
		delete temp_head;
		return;
	}
	Node* temp = head;
	while (temp->next->next != nullptr)
	{
		if (temp->next->object->id == id)
		{
			Node* temp_del = temp->next;
			temp->next = temp->next->next;
			delete temp_del;
		}
		else
		{

			temp = temp->next;
		}
	}
	if (temp->next->object->id == id)
	{
		Node* temp_del = temp->next;
		temp->next = nullptr;
		delete temp_del;
	}
}

// return the head of linked list that stores logic elements that exist in workbench 
Node* ObjectList::getObjectList()
{
	return head;
}

// set all the default objects such as logic elements in the left side and start and stop buttons for simulation 
void ObjectList::set_default_objects(sf::RenderWindow* window)
{
	this->window = window;

	// create all default objects   
	Object* andgate = new AndGate(window);
	Object* orgate = new OrGate(window);
	Object* notgate = new NotGate(window);
	Object* xorgate = new XorGate(window);
	Object* dff = new DFF(window);
	Object* clock = new CLOCK(window);
	Object* vdd = new VDD(window);
	Object* ground = new GROUND(window);
	Object* led = new LED(window);

	// create an ObjectList instance 
	ObjectList list;   

	// add every to the list 
	list.add(andgate);
	list.add(orgate);
	list.add(notgate);
	list.add(xorgate);
	list.add(dff);
	list.add(clock);
	list.add(vdd);
	list.add(ground);
	list.add(led);

	// get head of the list 
	default_objects_head = list.getObjectList();

	// create a temporary pointer to iterate over the list 
	Node* temp = default_objects_head;

	// get location an size info 
	int* object_info = Object::startWidthHeight();
	int x = object_info[0];
	int y = object_info[1];
	int object_width = object_info[2];
	int object_heigth = object_info[3];

	while (temp)
	{
		temp->object->set_coordinates(x, y);
		temp = temp->next;
		y += object_heigth;
	}

	// int originY = Object::startWidthHeight()[1]; 
	int originY = 0;

	// set coordinates of both button 
	start_button = new StartButton(window);
	start_button->set_coordinates(x + object_width * 3, originY + 10);
	stop_button = new StopButton(window);
	stop_button->set_coordinates(x + object_width * 3 + 300, originY + 10);
}

// show the logic elements on the left side of the screen 
void ObjectList::show_default_objects()
{
	Node* temp = default_objects_head;
	while (temp)
	{
		int* object_coordinates = temp->object->get_coordinates();
		temp->object->setPosition(object_coordinates[0], object_coordinates[1]);
		temp->object->draw();
		temp = temp->next;
	}
}

// return the length of linked list that stores selected elements on workbench by default   
// but the length of the linked list that stores default elements that are shown  
// in the left side can be returned if the argument of the function is true 
int ObjectList::length(bool is_default)
{
	Node* temp = nullptr;
	int len = 0;

	if (is_default)
	{
		temp = default_objects_head;
	}
	else
	{
		temp = head;
	}

	while (temp != nullptr)
	{
		len++;
		temp = temp->next;
	}

	return len;
}

// return the type of logic element that is selected 
string ObjectList::which_selected(int x)
{
	int len = ObjectList::length(true);
	if (x > 0 && x <= len)
	{
		Node* temp = default_objects_head;
		while (x != 1)
		{
			temp = temp->next;
			x--;
		}
		return temp->object->type;
	}
	return "";
}

// create and return a logic element based on the type 
Object* ObjectList::createObject(string object_type)
{
	if (object_type == "AND")
	{
		Object* andgate = new AndGate(window);
		return andgate;
	}
	else if (object_type == "OR")
	{
		Object* orgate = new OrGate(window);
		return orgate;
	}
	else if (object_type == "NOT")
	{
		Object* notgate = new NotGate(window);
		return notgate;
	}
	else if (object_type == "XOR")
	{
		Object* xorgate = new XorGate(window);
		return xorgate;
	}
	else if (object_type == "DFF")
	{
		Object* dff = new DFF(window);
		return dff;
	}
	else if (object_type == "CLOCK")
	{
		Object* clock = new CLOCK(window);
		return clock;
	}
	else if (object_type == "VDD")
	{
		Object* vdd = new VDD(window);
		return vdd;
	}
	else if (object_type == "GND")
	{
		Object* ground = new GROUND(window);
		return ground;
	}
	else if (object_type == "LED")
	{
		Object* led = new LED(window);
		return led;
	}
	return nullptr;
}

// carry is a boolean value referring to whether an object from the left side is selected and dropped to the workbench 
bool ObjectList::get_carry()
{
	return carry;
}

// set the value of carry variable 
void ObjectList::set_carry(bool expression)
{
	carry = expression;
}

// check if the default elements section is clicked on 
bool ObjectList::is_left(int x, int y)
{
	int *object_info = Object::startWidthHeight();
	int startX = object_info[0];
	int startY = object_info[1];
	int object_width = object_info[2];
	int object_height = object_info[3];
	int len = ObjectList::length(true);
	int lower_vertical_limit = startY + len * object_height;
	if (startX <= x && startX + object_width >= x && y <= lower_vertical_limit && startY <= y)
	{
		carry = true;
		return true;
	}
	return false;
}

// current refers to the object that is selected from the left side but is not dropped to the workbench yet 
void ObjectList::set_current(int x, int y)
{
	int *object_info = Object::startWidthHeight();
	int startY = object_info[1];
	int selected_object = ceil((y - startY) / 100.f);
	current = createObject(which_selected(selected_object));
	current->set_coordinates(x, y);
} 

// set coordinates for current  
void ObjectList::set_current_coordinates(int x, int y)
{
	if (current == nullptr)
	{
		cout << "current is nullptr " << endl;
		return;
	}
	if (current != nullptr)
	{
		current->set_coordinates(x, y);

	}
}

// draw every object to screen 
void ObjectList::show_everything()
{
	Node* temp = head;

	while (temp)
	{
		int* object_coordinates = temp->object->get_coordinates();
		temp->object->setPosition(object_coordinates[0], object_coordinates[1]);
		temp->object->draw();
		temp = temp->next;
	}
	if (!is_current_null())
	{

		// get coordinates of current, set its position and draw screen 
		int* object_coordinates = current->get_coordinates();
		current->setPosition(object_coordinates[0], object_coordinates[1]);
		current->draw();
	}

	// set position of start button 
	int* start_button_coords = start_button->get_coordinates();
	int start_button_x = start_button_coords[0];
	int start_button_y = start_button_coords[1];
	start_button->setPosition(start_button_x, start_button_y);

	// set position of the stop button 
	int* stop_button_coords = stop_button->get_coordinates();
	int stop_button_x = stop_button_coords[0];
	int stop_button_y = stop_button_coords[1];
	stop_button->setPosition(stop_button_x, stop_button_y);

	// draw start button 
	start_button->draw();

	// draw stop button 
	stop_button->draw();
} 

// check if pointer that points to current object is null meaning that only return true 
// if user selects an object from the left side but does not dropped to the workbench 
bool ObjectList::is_current_null()
{
	if (current == nullptr)
	{
		return true;
	}
	return false;
}

// add current object to the linked list if user selectes an object from the left side and drops to the workbench 
void ObjectList::add_current()
{
	if (current != nullptr)
	{
		int* current_position = current->get_coordinates();
		int current_position_x = current_position[0];
		int current_position_y = current_position[1];
		current->handlePins(current_position_x, current_position_y);
		// cout << "curent positions: x: " << current->get_coordinates()[0] << " y: " << current->get_coordinates()[1] << endl; 
		add(current);
		current = nullptr;
		// cout << "current positions: " << "x: " << current->get_coordinates()[0] << " y:  " << current->get_coordinates()[1] << endl; 
	}
}

// set pointer that points to the current object to nullptr  
void ObjectList::del_current()
{
	if (current != nullptr)
	{
		current = nullptr;
	}
}

// check if the place where user wants to drop the selected object is available 
bool ObjectList::is_available(int x, int y)
{
	int *object_info = Object::startWidthHeight();
	int object_width = object_info[2];
	int object_height = object_info[3];

	Node* temp = head;
	while (temp != nullptr)
	{
		int objectX = temp->object->coordinates[0];
		int objectY = temp->object->coordinates[1];

		// objects can not be placed in the same place and there must be some distance between them 
		if (objectX - object_width <= x && x <= objectX + object_width && objectY - object_height <= y && y <= objectY + object_height)
		{
			cout << "this item cannot be dropped here. " << endl;
			return false;
		}
		temp = temp->next;
	}
	return true;
}

// check if user has clicked on any of the objects in workbench 
void ObjectList::selectedObject(int x, int y)
{
	int *object_info = Object::startWidthHeight();
	int object_width = object_info[2];
	int object_height = object_info[3];

	Node* temp = head;

	while (temp != nullptr)
	{
		int objectX = temp->object->coordinates[0];
		int objectY = temp->object->coordinates[1];
		if (objectX <= x && x <= objectX + object_width && objectY <= y && y <= objectY + object_height && is_current_null())
		{
			cout << "selected object is: " << temp->object->type << ", id: " << temp->object->getId() << endl;

			// check if any pins of selected logic element are selected or not 
			int clicked_pin = Pin::clicked_pin_index(objectX, objectY, x, y, temp->object->type, temp->object->getId());
			if (temp->object->selected)
			{
				if (clicked_pin == -1)
				{
					temp->object->selected = false;
				}
			}
			else
			{
				if (clicked_pin == -1)  
				{
					temp->object->selected = true;
				}
			}
			return;
		}
		temp = temp->next;
	}
	return;
} 

// delete selected objects on workbench 
void ObjectList::delete_pressed()
{
	int selected_num = number_of_selected_objects();
	for (int i = 0; i < selected_num; i++)
	{
		Node* temp = head;
		while (temp != nullptr)
		{
			if (temp->object->selected)
			{
				int object_id = temp->object->getId();
				delete_object(object_id); 
				cout << "Object with an id of " << object_id << " has been deleted " << endl;  
				Pin::wires_array_delete(object_id);
				break;
			}
			else
			{
				temp = temp->next;
			}
		}
	}
	cout << "delete is pressed." << endl;
}

// set selected variables of all the objects in workbench to false 
void ObjectList::esc_pressed()
{
	Node* temp = head;
	while (temp != nullptr)
	{
		temp->object->selected = false;
		temp = temp->next;
	}
	Pin::pin_esc_pressed();
	cout << "esc is pressed." << endl;
}

// get the number of selected objects 
int ObjectList::number_of_selected_objects()
{
	Node* temp = head;
	int selected = 0;
	while (temp != nullptr)
	{
		if (temp->object->selected)
		{
			selected++;
		}
		temp = temp->next;
	}
	return selected;
}

// draw a rectangle shape that is empty inside for every logic element in the workbench that has been selected  
void ObjectList::selected_shape()
{
	int *object_info = Object::startWidthHeight();
	int object_width = object_info[2] + 20;
	int object_height = object_info[3] + 20;
	int difference = 10;

	Node* temp = head;

	while (temp)
	{
		if (temp->object->selected)
		{
			int selected_id = temp->object->getId();
			int* selected_coordinates = temp->object->get_coordinates();
			int selectedX = selected_coordinates[0] - 10;
			int selectedY = selected_coordinates[1] - 25;

			// rectangle shape that surrounds the selected object 
			sf::RectangleShape line1(sf::Vector2f(object_width, 5.f));
			sf::RectangleShape line2(sf::Vector2f(object_width, 5.f));
			sf::RectangleShape line3(sf::Vector2f(object_height + 5.f, 5.f));
			line3.rotate(90.f);
			sf::RectangleShape line4(sf::Vector2f(object_height, 5.f));
			line4.rotate(90.f);

			// set their color to red 
			line1.setFillColor(sf::Color::Red);
			line2.setFillColor(sf::Color::Red);
			line3.setFillColor(sf::Color::Red);
			line4.setFillColor(sf::Color::Red);

			// set all the lines' positions 
			line1.setPosition(sf::Vector2f(float(selectedX), float(selectedY)));
			line2.setPosition(sf::Vector2f(float(selectedX), float(selectedY + object_height)));
			line3.setPosition(sf::Vector2f(float(selectedX), float(selectedY)));
			line4.setPosition(sf::Vector2f(float(selectedX + object_width), float(selectedY)));

			// draw lines 
			window->draw(line1);
			window->draw(line2);
			window->draw(line3);
			window->draw(line4);

		}

		temp = temp->next;
	}

	// check if a pin is selected 
	if (Pin::is_pin_from_selected)
	{
		int pin_dims = Pin::pin_dims();
		sf::CircleShape pin_selected(pin_dims);
		int pinX = Pin::pin_from_x;
		int pinY = Pin::pin_from_y;
		pin_selected.setPosition(sf::Vector2f(float(pinX - pin_dims / 2), float(pinY - pin_dims / 2)));
		pin_selected.setFillColor(sf::Color::Green);

		// draw a green circle to the selected pin 
		window->draw(pin_selected);
	}

	// draw all the connections       
	for (int i = 0; i < Pin::wires_array_length; i++)
	{
		if (Pin::wires_array[i][0] != -1)
		{
			int pin_from_x = Pin::wires_array[i][0];
			int pin_from_y = Pin::wires_array[i][1];
			int pin_to_x = Pin::wires_array[i][2];
			int pin_to_y = Pin::wires_array[i][3];
			int pin_dims = Pin::pin_dims() / 2; 

			sf::Vertex line[] = {
				sf::Vertex(sf::Vector2f(pin_from_x + pin_dims, pin_from_y + pin_dims)),
				sf::Vertex(sf::Vector2f(pin_to_x + pin_dims, pin_to_y + pin_dims))
			};

			window->draw(line, 2, sf::Lines);
		}
	}
}

// check if user clicks on workbench 
bool ObjectList::is_workbench(int x, int y)
{
	int *object_info = Object::startWidthHeight();
	int startX = object_info[0];
	int startY = object_info[1];
	int object_width = object_info[2];
	int object_height = object_info[3];

	if (x > startX + object_width * 2 && y > startY + object_height)
	{
		return true;
	}
	return false;
} 

// check if user clicks on simulation section where start and stop buttons are 
bool ObjectList::is_simulation(int x, int y)
{
	int *object_info = Object::startWidthHeight();
	int startX = object_info[0];
	int startY = object_info[1];
	int object_width = object_info[2];
	int object_height = object_info[3];

	if (x > startX + object_width * 2 && y < startY + object_height)
	{
		return true;
	}
	return false;
}	 

// execute here when simulation section is clicked and do anything if start or stop buttons are clicked 
void ObjectList::simulation_clicked(int x, int y)
{
	int button_width = 215;
	int button_height = 35;

	int* start_button_coords = start_button->get_coordinates();
	int start_button_x = start_button_coords[0];
	int start_button_y = start_button_coords[1];

	int* stop_button_coords = stop_button->get_coordinates();
	int stop_button_x = stop_button_coords[0];
	int stop_button_y = stop_button_coords[1];

	if (start_button_x < x && x < start_button_x + button_width && start_button_y < y && y < start_button_y + button_height)
	{
		cout << "start button is clicked " << endl;
	}

	else if (stop_button_x < x && x < stop_button_x + button_width && stop_button_y < y && y < stop_button_y + button_height)
	{
		cout << "stop button is clicked " << endl;
	}

	else
	{
		cout << "simulation area is clicked " << endl;
	}
}

// select every logic element in workbench 
void ObjectList::select_all()
{
	Node* temp = head;
	while (temp)
	{
		temp->object->selected = true;
		temp = temp->next;
	}
	cout << "A is clicked" << endl;
} 

// change all LED elements' sprite if they are in workbench 
void ObjectList::Z_pressed() 
{
	Node* temp = head;
	while (temp)
	{
		if (temp->object->type == "LED")
		{
			temp->object->changeSprite(); 
		}
		temp = temp->next;
	}

	cout << "Z is clicked" << endl;
}