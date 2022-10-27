#pragma once
#include <SFML/Graphics.hpp> 
#include "objects.h"
#include <iostream>    

class Node
{
public:
	Node* next;
	Object* object;
};

// linked list that stores all the logic gates 
// Objectlist includes two linked lists 
// head pointer is the first node of the linked list that stores all the logic gates that are created or deleted in workbench 
// default_object_head pointer is the first node of another linked list that stores all the default logic elements located to the left side of the screen 
class ObjectList
{
private:
	Node* head;
	Node* default_objects_head;
	sf::RenderWindow* window;
	Object* current;
	bool carry;
	sf::RectangleShape background_left;
	Object* start_button;
	Object* stop_button;
public:
	ObjectList();
	ObjectList(Object*);
	void set_common();
	void add(Object*);
	void print();
	void delete_object(int id);
	void set_default_objects(sf::RenderWindow*);
	Node* getObjectList();
	void show_default_objects();
	int length(bool is_default = false);
	std::string which_selected(int);
	bool is_left(int x, int y);
	Object* createObject(std::string);
	bool is_workbench(int x, int y);
	bool get_carry();
	void set_carry(bool expression = false);
	void set_current(int, int);
	void set_current_coordinates(int, int);
	void show_everything();
	bool is_current_null();
	void add_current();
	void del_current();
	bool is_available(int, int);
	void selectedObject(int, int);
	void delete_pressed();
	void esc_pressed();
	int number_of_selected_objects();
	void selected_shape();
	bool is_simulation(int, int);
	void simulation_clicked(int, int);
	void select_all();
	void Z_pressed(); 
};
