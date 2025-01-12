#include "users.h"

double User::get_kg() {
	return user_kg;	
}

std::string User::get_name() {
	return name;	
}

User::User(std::string n, double kg) {
	this->user_kg=kg;
	this->name=n;
}

