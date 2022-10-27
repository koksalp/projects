#include "objects.h"
#include <iostream>   

using namespace std;

// initialize next id to 0 so that each object can have a unique id 
int Object::next_id = 0;

// Object constructor 
Object::Object(sf::RenderWindow* window, string image1, string image2 = "")
{
	id = ++next_id;
	state = false;
	selected = false;
	locked = false;
	this->window = window;
	sf::Texture texture1;
	textures[0] = texture1;

	// load the texture with given image 
	if (!textures[0].loadFromFile("../assets/" + image1 + ".png"))
	{
		cout << "Error loading the image." << endl;
	}

	// execute here if the object has 2 textures 
	if (image2 != "")
	{
		sf::Texture texture2;
		textures[1] = texture2;
		if (!textures[1].loadFromFile("../assets/" + image2 + ".png"))
		{
			cout << "Error loading the image." << endl;
		}
	}

	// set texture for sprite 
	sprite.setTexture(textures[0]);
}

// return sprite 
sf::Sprite &Object::getSprite()
{
	return sprite;
}

// set position for the object's sprite 
void Object::setPosition(int x, int y)
{
	sprite.setPosition(sf::Vector2f(float(x), float(y)));
}

// draw object's sprite 
void Object::draw()
{
	window->draw(sprite);
}

// get the object's id 
int Object::getId()
{
	return id;
}

// get object's texture 
sf::Texture* Object::getTexture()
{
	return textures;
}

// virtual function of base class 
// any object that has 2 textures (LED) have this function overriden in its derived class 
void Object::changeSprite()
{
	cout << "Object class " << endl;
	return;
}

// virtual method for Object class 
void Object::handlePins(int objectX, int objectY)
{
	return;
}

// get window object 
sf::RenderWindow* Object::getWindow()
{
	return window;
}

// set coordinates of object 
void Object::set_coordinates(int x, int y)
{
	if (x >= 0)
	{
		coordinates[0] = x;
	}
	if (y >= 0)
	{
		coordinates[1] = y;
	}
}

// get coordinates of object 
int* Object::get_coordinates()
{
	return coordinates;
}

// static function that returns an array that has information for 
// starting coordinates of default logic elements that are located 
// in the left side of the window that user can drag and drop to workbench 
// last two values are width and height of logic elements 
int* Object::startWidthHeight()
{
	int* object_info = new int[4];

	object_info[0] = 100;
	object_info[1] = 0;
	object_info[2] = 100;
	object_info[3] = 100;

	return object_info;
}

// constructor for LogicElement class 
// create new pin instances for how many pins that logic element will have  
// and set their pin type as well as their index 
LogicElement::LogicElement(sf::RenderWindow* window, string type, string type2) : Object(window, type, type2)
{
	if (type == "LEDOFF" || type == "LEDON")
	{
		type = "LED";
	}
	this->type = type;
	numPins = Pin::pinNums(type);
	int* pin_types = Pin::pintypes_array(type);
	int input_number = pin_types[0];
	int output_number = pin_types[1];
	for (int i = 0; i < numPins; i++)
	{
		string pintype = "OUTPUT";
		if (i < input_number)
		{
			pintype = "INPUT";
		}
		pins[i] = new Pin(pintype, i);
	} 
} 

// free memory allocated for pins pointer array 
LogicElement::~LogicElement()
{
	int pin_nums = Pin::maxPins(); 
	for (int i = 0; i < pin_nums; i++)
	{
		delete pins[i]; 
	}
}

// set all the coordinates of pins 
// this method gets called if an object is dropped to workbench 
// thus pins' coordinates can be set based on the object position 
// and absolute positions of pins taken from pinPositions which is a static method  
// that returns an array of elements indicating absolute positions of the pins 
void LogicElement::handle_pins(int objectX, int objectY)
{
	// cout << "pin coordinates: " << endl;
	string object_type = type;
	int** pin_positions = Pin::pinPositions(object_type);
	numPins = Pin::pinNums(type);
	for (int i = 0; i < numPins; i++)
	{
		pins[i]->set_coordinates(objectX + pin_positions[i][0], objectY + pin_positions[i][1]);
		// cout << objectX + pin_positions[i][0] << " " << objectY + pin_positions[i][1] << endl;
	}
}

// constructor for and gate 
AndGate::AndGate(sf::RenderWindow* window) : LogicElement(window, "AND")
{
	cout << "And gate is called " << endl;
}

// call LogicElement's handle_pin method with the given arguments 
void AndGate::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for led 
LED::LED(sf::RenderWindow* window) : LogicElement(window, "LEDOFF", "LEDON")
{
	currentImage = "LEDOFF";
	cout << "LED is called" << endl;
}

// function that is called when led needs to be switched to on or off 
void LED::changeSprite()
{
	currentImage = currentImage == "LEDOFF" ? "LEDON" : "LEDOFF";
	sf::Sprite &sprite = getSprite();
	int imageIndex = currentImage == "LEDOFF" ? 0 : 1;
	sf::Texture * textures = getTexture();
	sprite.setTexture(textures[imageIndex]);
	string now = currentImage == "LEDOFF" ? "OFF" : "ON"; 
	cout << "LED is  " << now << endl; 
} 

void LED::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
} 

// constructor for or gate 
OrGate::OrGate(sf::RenderWindow* window) : LogicElement(window, "OR")
{
	cout << "Or gate is called " << endl;
}

void OrGate::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for xor gate 
XorGate::XorGate(sf::RenderWindow* window) : LogicElement(window, "XOR")
{
	cout << "Xor gate is called " << endl;
}

void XorGate::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for not gate 
NotGate::NotGate(sf::RenderWindow* window) : LogicElement(window, "NOT")
{
	cout << "Not gate is called " << endl;
}

void NotGate::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for DFF 
DFF::DFF(sf::RenderWindow* window) : LogicElement(window, "DFF")
{
	cout << "Dff is called " << endl;
}

void DFF::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for CLOCK 
CLOCK::CLOCK(sf::RenderWindow* window) : LogicElement(window, "CLOCK")
{
	cout << "Clock is called " << endl;
}

void CLOCK::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for VDD 
VDD::VDD(sf::RenderWindow* window) : LogicElement(window, "VDD")
{
	cout << "Vdd is called " << endl;
}

void VDD::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for ground 
GROUND::GROUND(sf::RenderWindow* window) : LogicElement(window, "GND")
{
	cout << "Ground is called " << endl;
}

void GROUND::handlePins(int objectX, int objectY)
{
	handle_pins(objectX, objectY);
}

// constructor for start button 
StartButton::StartButton(sf::RenderWindow* window) : Object(window, "START")
{
	cout << "Start button is called " << endl;
	type = "START";
}

// constructor for stop button 
StopButton::StopButton(sf::RenderWindow* window) : Object(window, "STOP")
{
	cout << "Stop button is called " << endl;
	type = "STOP";
}

// Pin constructor 
Pin::Pin(std::string pinType, int index)
{
	this->pinType = pinType;
	this->index = index;
	state = 0;
	number_of_connections = 0;
	int max_pins = Pin::maxPins();
	for (int i = 0; i < max_pins; i++)
	{
		connected_to[i] = nullptr;
		wires[i] = nullptr;
		is_source[i] = false;
	}
}

// length of wires_array 
int Pin::wires_array_length = 50;

// array for storing information about connection between two elements  
// first 4 elements hold information of positions of logic elements  
// last 2 elements hold information of id's of logic elements 
// x and y coordinates of the element to be connected from 
// x and y coordinates of the element to be connected to 
// id of first element and id of last element respectively  
// a linked list is a better option for dynamically store all of these information  
// everything related to the length of the array depends on the variable named wires_array_length in anywhere in program 
// thus the length of the array can be changed by only changing the value of the variable and by changing the value in array initialization in both .h and .cpp files 
int Pin::wires_array[50][6] = {
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1}, {-1, -1, -1, -1, -1, -1},
};

// add the information of a new wire to the array named wires_array 
void Pin::wires_array_add(int pin_from_x, int pin_from_y, int pin_to_x, int pin_to_y, int from_id, int to_id)
{
	for (int i = 0; i < wires_array_length; i++)
	{
		if (wires_array[i][0] == -1)
		{
			wires_array[i][0] = pin_from_x;
			wires_array[i][1] = pin_from_y;
			wires_array[i][2] = pin_to_x;
			wires_array[i][3] = pin_to_y;
			wires_array[i][4] = from_id;
			wires_array[i][5] = to_id;
			break;
		}
	}
}

// delete wire or multiple wires connected to a logic element having the id of object_id 
void Pin::wires_array_delete(int object_id)
{
	int arr_len = Pin::wires_array_length;
	for (int i = 0; i < arr_len; i++)
	{
		if (Pin::wires_array[i][4] == object_id || Pin::wires_array[i][5] == object_id)
		{
			Pin::wires_array[i][0] = -1;
		}
	}
	if (pin_from_id == object_id)
	{
		pin_esc_pressed();
	}
}

// destructor for Pin class  
// frees all the allocated memory  
Pin::~Pin()
{
	int max_pins = Pin::maxPins();
	for (int i = 0; i < max_pins; i++)
	{
		delete connected_to[i];
		delete wires[i];
		// delete is_source[i];
	}
	cout << "Pin destructor" << endl; 
}

// set coordinates of a pin 
void Pin::set_coordinates(int x, int y)
{
	if (x > 0)
	{
		pin_coordinates[0] = x;
	}
	if (y > 0)
	{
		pin_coordinates[1] = y;
	}
}

// get coordinates of pin 
int* Pin::get_coordinates()
{
	return pin_coordinates;
}

// initialize all the static variables related to connections between the elements 
int Pin::pin_from_id = -1;
int Pin::pin_to_id = -1;
int Pin::pin_from_x = -1;
int Pin::pin_from_y = -1;
int Pin::pin_to_x = -1;
int Pin::pin_to_y = -1;
bool Pin::is_pin_from_selected = false;
bool Pin::is_pin_to_selected = false;

// no pin is selected after pressing esc 
void Pin::pin_esc_pressed()
{
	Pin::is_pin_from_selected = false;
	Pin::is_pin_to_selected = false;
}

// static function that returns the maximum pin number 
int Pin::maxPins()
{
	return 4;
}

// static function that returns a 2D array indicating the coordinates of each pin  
// relative to starting point of the chosen logic element assuming the starting point of the element is 0, 0 
int** Pin::pinPositions(std::string object_type)
{
	if (object_type == "AND" || object_type == "OR" || object_type == "XOR")
	{
		int **array = new int*[3];
		for (int i = 0; i < 3; i++)
		{
			array[i] = new int[2];
		}
		array[0][0] = 0;
		array[0][1] = 10;
		array[1][0] = 0;
		array[1][1] = 40;
		array[2][0] = 90;
		array[2][1] = 25;
		return array;
	}
	else if (object_type == "NOT")
	{
		int **array = new int*[2];
		for (int i = 0; i < 2; i++)
		{
			array[i] = new int[2];
		}
		array[0][0] = 0;
		array[0][1] = 25;
		array[1][0] = 90;
		array[1][1] = 25;
		return array;
	}
	else if (object_type == "LED")
	{
		int **array = new int*[2];
		for (int i = 0; i < 2; i++)
		{
			array[i] = new int[2];
		}
		array[0][0] = 10;
		array[0][1] = 65;
		array[1][0] = 25;
		array[1][1] = 65;
		return array;
	}
	else if (object_type == "DFF")
	{
		int **array = new int*[4];
		for (int i = 0; i < 4; i++)
		{
			array[i] = new int[2];
		}
		array[0][0] = 0;
		array[0][1] = 15;
		array[1][0] = 0;
		array[1][1] = 35;
		array[2][0] = 90;
		array[2][1] = 15;
		array[3][0] = 90;
		array[3][1] = 55;
		return array;
	}
	else if (object_type == "CLOCK" || object_type == "VDD" || object_type == "GND")
	{
		int **array = new int*[1];
		for (int i = 0; i < 1; i++)
		{
			array[i] = new int[2];
		}
		if (object_type == "CLOCK")
		{
			array[0][0] = 70;
			array[0][1] = 25;
		}
		else if (object_type == "VDD")
		{
			array[0][0] = 25;
			array[0][1] = 50;
		}
		else
		{
			array[0][0] = 15;
			array[0][1] = 0;
		}
		return array;
	}

	int **array = new int*[1];
	array[0] = new int[1];
	array[0][0] = -1;
	return array;
}

// static function that returns the number of pin for a specific element  
int Pin::pinNums(std::string object_type)
{
	if (object_type == "AND" || object_type == "OR" || object_type == "XOR")
	{
		return 3;
	}
	else if (object_type == "NOT" || object_type == "LED")
	{
		return 2;
	}
	else if (object_type == "DFF")
	{
		return 4;
	}
	else if (object_type == "CLOCK" || object_type == "VDD" || object_type == "GND")
	{
		return 1;
	}
	return 0;
} 

// get all the logic element types 
string* Pin::gateTypes()
{
	string temp_types[9] = { "AND", "OR", "NOT", "XOR", "DFF", "CLOCK", "VDD", "GND", "LED" };
	string* types = new string[9];

	// copy from local variable to pointer in order not to return the address of local variable 
	for (int i = 0; i < 9; i++)
	{
		types[i] = temp_types[i];
	}
	return types;
}

// dimensions of a pin which are 10 by 10  
// returning only one value is enough since all the pins are square shaped 
int Pin::pin_dims()
{
	return 10;
} 

// return an array containing information of how many input and output pin a specific logic element has 
int* Pin::pintypes_array(string object_type)
{
	int* array = new int[2];
	if (object_type == "AND" || object_type == "OR" || object_type == "XOR")
	{
		array[0] = 2;
		array[1] = 1;
		return array;
	}
	else if (object_type == "NOT")
	{
		array[0] = 1;
		array[1] = 1;
		return array;
	}
	else if (object_type == "DFF")
	{
		array[0] = 2;
		array[1] = 2;
		return array;
	}
	else if (object_type == "CLOCK" || object_type == "VDD" || object_type == "GND")
	{
		array[0] = 1;
		array[1] = 0;
		return array;
	}
	else if (object_type == "LED")
	{
		array[0] = 2;
		array[1] = 0;
		return array;
	}
	return 0;
} 

// return the index of clicked pin and add to array for connect two elements if user wants to connect two elements 
int Pin::clicked_pin_index(int gateX, int gateY, int x, int y, string object_type, int object_id)
{

	// get absolute position of given element's pins 
	int** pin_positions = Pin::pinPositions(object_type);

	// number of pins of given element  
	int pin_numbers = Pin::pinNums(object_type);

	// cout << "pin numbers: " << pin_numbers << endl;

	// pin dimensions 
	int pin_dims = Pin::pin_dims();

	// iterate over each pin 
	for (int i = 0; i < pin_numbers; i++)
	{

		// get each pin's position 
		int pinX = pin_positions[i][0];
		int pinY = pin_positions[i][1];

		// check if the pin is clicked 
		if (gateX + pinX <= x && x <= gateX + pinX + pin_dims && gateY + pinY <= y && y <= gateY + pinY + pin_dims)
		{
			cout << "Pin is clicked, index: " << i << endl;

			// store positions of the pin and id of the element 
			// if a pin is selected and no pins have been selected before 
			if (!Pin::is_pin_from_selected)
			{
				Pin::pin_from_x = gateX + pinX;
				Pin::pin_from_y = gateY + pinY;
				Pin::pin_from_id = object_id;
				Pin::is_pin_from_selected = true;
			}

			// add to the array if a pin has already been selected 
			// which means first pin is selected and now second pin is selected thus a wire should be connected 
			else
			{
				Pin::pin_to_x = gateX + pinX;
				Pin::pin_to_y = gateY + pinY;
				Pin::pin_to_id = object_id;

				// user can not connect the logic element with itself  
				if (Pin::pin_from_id == object_id)
				{
					cout << "Another logic element must be selected." << endl;
				}

				// add to the array if two pins from two different logic gates are selected 
				else
				{
					Pin::wires_array_add(Pin::pin_from_x, Pin::pin_from_y, Pin::pin_to_x, Pin::pin_to_y, Pin::pin_from_id, Pin::pin_to_id);
					Pin::is_pin_from_selected = false;
				}
			}

			// return the index of clicked pin   
			return i;
		}

		// cout << "[ (" << gateX + pinX << ", " << gateX + pinX + pin_dims << "), (" << gateY + pinY << ", " << gateY + pinY + pin_dims << ") ]" << endl; 
	}

	// return -1 if no pin is selected 
	return -1;
}
