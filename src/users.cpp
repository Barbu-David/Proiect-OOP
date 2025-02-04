#include "users.h"

User::User(std::string n, double kg) 
{
	this->user_kg=kg;
	this->name=n;
}

double User::get_kg() 
{
	return user_kg;	
}

std::string User::get_name() 
{
	return name;	
}
std::vector<Workout*> User::get_workout_vector()
{
	return workout_vector;
}
