#pragma once
#include <SFML/Graphics.hpp> 
#include <iostream>     

// object class is the base class for every drawable derived class 
class Object
{
private:
	static int next_id;
	int id;
	bool locked;
	bool state;
	bool selected;
	sf::Texture textures[2];
	sf::Sprite sprite;
	sf::RenderWindow* window;
	int coordinates[2];
	friend class ObjectList;
protected:
	std::string type;
public:
	Object(sf::RenderWindow*, std::string, std::string);
	sf::Texture* getTexture();
	sf::Sprite &getSprite();
	void setPosition(int, int);
	void draw();
	virtual void changeSprite();
	int getId();
	void set_coordinates(int, int);
	int* get_coordinates();
	sf::RenderWindow* getWindow();
	static int* startWidthHeight();
	virtual void handlePins(int, int);
};

// pin class for describing pins that are contained by logic elements 
class Pin
{
private:
	std::string pinType;
	int index;
	int number_of_connections;
	int state;
	Pin* connected_to[4];
	Object* wires[4];
	bool is_source[4];
	int pin_coordinates[2];
	static int wires_array_length;
	static int pin_from_id;
	static int pin_to_id;
	static int pin_from_x;
	static int pin_from_y;
	static int pin_to_x;
	static int pin_to_y;
	static bool is_pin_from_selected;
	static bool is_pin_to_selected;
	static int wires_array[50][6]; // follow wires 
	friend class ObjectList;
public:
	Pin(std::string, int);
	~Pin();
	void set_coordinates(int, int);
	int* get_coordinates();
	static int clicked_pin_index(int, int, int, int, std::string, int);
	static std::string* gateTypes();
	static int maxPins();
	static int** pinPositions(std::string);
	static int pinNums(std::string);
	static int pin_dims(); 
	static int* pintypes_array(std::string);
	static void wires_array_add(int, int, int, int, int, int);
	static void pin_esc_pressed();
	static void wires_array_delete(int);
};

// all the logic gates derive from this class 
class LogicElement : public Object
{
private:
	Pin* pins[4];
	int numPins;
public:
	LogicElement(sf::RenderWindow*, std::string, std::string type2 = "");
	~LogicElement(); 
	void handle_pins(int, int); // on progress 
};

class AndGate : public LogicElement
{
private:
public:
	AndGate(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class LED : public LogicElement
{
private:
	std::string currentImage;
public:
	LED(sf::RenderWindow*);
	void changeSprite() override;
	void handlePins(int, int) override;
};

class OrGate : public LogicElement
{
private:
public:
	OrGate(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class XorGate : public LogicElement
{
private:
public:
	XorGate(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class NotGate : public LogicElement
{
private:
public:
	NotGate(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class DFF : public LogicElement
{
private:
public:
	DFF(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class CLOCK : public LogicElement
{
private:
public:
	CLOCK(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class VDD : public LogicElement
{
private:
public:
	VDD(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class GROUND : public LogicElement
{
private:
public:
	GROUND(sf::RenderWindow*);
	void handlePins(int, int) override;
};

class StartButton : public Object
{
public:
	StartButton(sf::RenderWindow*);
};

class StopButton : public Object
{
public:
	StopButton(sf::RenderWindow*);
};
