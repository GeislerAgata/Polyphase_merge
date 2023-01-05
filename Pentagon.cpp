#include "Pentagon.h"

Pentagon::Pentagon() {}

Pentagon::Pentagon(double first, double second, double third, double fourth, double fifth) {
	side1 = first;
	side2 = second;
	side3 = third;
	side4 = fourth;
	side5 = fifth;
	perimeter = side1 + side2 + side3 + side4 + side5;
}

Pentagon::Pentagon(double first, double second, double third, double fourth, double fifth, double given_perimeter) {
	side1 = first;
	side2 = second;
	side3 = third;
	side4 = fourth;
	side5 = fifth;
	perimeter = given_perimeter;
}