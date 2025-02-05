#include "exercises.h"
#include <iostream>

//Default Constructors

Exercise::Exercise(std::string Name, std::string Description, Texture2D Photo) {
	this->name=Name;
	this->description=Description;
	this->photo=Photo;
} 

Running::Running(std::string Name, std::string Description, Texture2D Photo, double intensity_factor) : Exercise(Name, Description, Photo)
{
	this->intensity_factor=intensity_factor;
	this->distance=0;
}

Calisthenics::Calisthenics(std::string Name, std::string Description, Texture2D Photo, double Calories_per_rep, std::string Muscle_group) : Exercise(Name, Description, Photo)
{

	this->calories_per_rep=Calories_per_rep;
	this->muscle_group=Muscle_group;
	this->repetitions=0;
	this->sets=0;
}

Weight::Weight(std::string Name, std::string Description, Texture2D Photo, double Calories_per_rep, std::string Muscle_group) : Calisthenics(Name, Description, Photo, Calories_per_rep, Muscle_group) {}

//Copy Constructors

Exercise::Exercise(const Exercise &other) 
{
	this->name = other.name;
	this->description = other.description;
	this->photo = other.photo;
	this->type=other.type;
}

Running::Running(const Running &other) : Exercise(other) 
{
	this->distance = other.distance;
	this->intensity_factor = other.intensity_factor;
}

Calisthenics::Calisthenics(const Calisthenics &other) : Exercise(other) 
{
	this->repetitions = other.repetitions;
	this->sets = other.sets;
	this->calories_per_rep = other.calories_per_rep;
	this->muscle_group = other.muscle_group;
}

Weight::Weight(const Weight &other, double nweight) : Calisthenics(other)
{
	this->weight = nweight;
}

//Getters

//Exercise getters

std::string Exercise::get_name() 
{
	return name;
}

std::string Exercise::get_description() 
{
	return description;
}

Texture2D Exercise::get_photo() 
{
	return photo;
}

ExerciseType Exercise::get_type() 
{
	return type;
}

//Running getters

double Running::get_distance()
{
	return distance;
}
double Running::get_max_distance()
{
	return max_distance;
}
double Running::get_time()
{
	return time;
}
double Running::get_intensity_factor()
{
	return intensity_factor;
}

//Calisthenics getters

int Calisthenics::get_repetitons()
{
	return repetitions;
}
int Calisthenics::get_max_repetitions()
{
	return max_repetitions;
}
int Calisthenics::get_sets()
{
	return sets;
}
int Calisthenics::get_max_sets()
{
	return max_sets;
}
std::string Calisthenics::get_muscle_group()
{
	return muscle_group;
}

//Weight getters

double Weight::get_weight()
{
	return weight;
}

//Setters

void Exercise::set_type(ExerciseType type) 
{
	this->type=type;
}

void Running::set_distance(double distance)
{
	this->max_distance=distance;
}

void Running::set_time(double time)
{
	this->time=time;
}


void Calisthenics::set_reps(int reps)
{
	this->max_repetitions=reps;
}

void Calisthenics::set_sets(int sets) 
{
	this->max_sets=sets;
}

void Calisthenics::reset_reps() 
{
	this->repetitions=0;
}

void Calisthenics::reset_sets() 
{
	this->sets=0;
}

void Weight::set_weight(double nweight) 
{
	this->weight=nweight;
}

//Progress

double Running::get_progress() {
	return (distance/max_distance < 1)? distance/max_distance:1;
}

void Running::update_progress() {
	if(distance<max_distance) distance+=0.1;
}

double Calisthenics::get_progress() {
	return (double(sets)/double(max_sets));
}

void Calisthenics::update_progress() {
	if(repetitions<max_repetitions && sets<max_sets) repetitions++;
	else  if(sets<max_sets) {
		sets++;
		repetitions=0;
	}
}

//Calories

double Calisthenics::calculate_calories_max( double user_kg) {
	return max_sets * (calories_per_rep + user_kg);
}

double Running::calculate_calories_max(double user_kg)  {
	return intensity_factor*user_kg*max_distance*(max_distance/time);
}

double Weight::calculate_calories_max(double user_kg) {
	return max_sets * ((calories_per_rep+weight) + user_kg);
}

double Calisthenics::calculate_calories( double user_kg) {
	return sets * (calories_per_rep + user_kg);	
}

double Running::calculate_calories(double user_kg)  {
	return intensity_factor*user_kg*((distance/max_distance))*distance*(max_distance/time);
}

double Weight::calculate_calories(double user_kg) {
	return sets *( (calories_per_rep+weight) + user_kg);
}

