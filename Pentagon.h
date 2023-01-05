#pragma once

class Pentagon {
public:
	double side1;		// first side of the pentagon
	double side2;		// second side of the pentagon
	double side3;		// third side of the pentagon
	double side4;		// fourth side of the pentagon
	double side5;		// fifth side of the pentagon
	double perimeter;	// pentagon perimeter
	Pentagon();
	Pentagon(double first, double second, double third, double fourth, double fifth);
	Pentagon(double first, double second, double third, double fourth, double fifth, double given_perimeter);
};
