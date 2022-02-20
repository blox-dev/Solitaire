#include <iostream>


class A {
public:
	A() { x = 12; y = 1.4f; };

	bool draw() { return x + 1 == true; }
	int getX() { return x; };
	float getY() { return y; };
private:
	int x;
	float y;
};

int main() {
	A obj;

	A* p = &obj;

	// set private variables, as you do
	*(int*)((void*)p) = 14;
	
	*((float*)((void*)p) + 1) = 1.7f;

	std::cout << obj.getX() << ", " << obj.getY() << std::endl;
	return 0;
}