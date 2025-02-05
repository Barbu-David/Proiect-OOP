#ifndef EXERCISES_H
#define EXERCISES_H

#include <raylib.h>
#include <string>

enum class ExerciseType {
	Calisthenics,
	Running,
	Weight
};

class Exercise {
	public:
	
	//Constructors
		Exercise(std::string Name, std::string Description, Texture2D Photo);
		Exercise(const Exercise &other);
	
	//Getters	
		std::string get_name();
		std::string get_description();
		Texture2D get_photo();
		ExerciseType get_type();
	
	//Setters
		void set_type(ExerciseType type);
	
	//Virtual info functions
		virtual double get_progress() = 0;
		virtual void update_progress() = 0;
		virtual double calculate_calories(double user_kg) = 0;
		virtual double calculate_calories_max(double user_kg) = 0;

	protected:
		ExerciseType type;
		std::string name;
		std::string description;
		Texture2D photo;
};

class Running : public Exercise {
	public:

	//Constructors
		Running(const Running &other);
		Running(std::string Name, std::string Description, Texture2D Photo, double intensity_factor);

	//Getters
		double get_distance();
		double get_max_distance();
		double get_time();
		double get_intensity_factor();

	//Setters
		void set_distance(double distance);
		void set_time(double time);	

	//Info functions
		double get_progress() override;
		void update_progress() override;
		double calculate_calories(double user_kg) override;
		double calculate_calories_max(double user_kg) override;

	private:
		double distance, max_distance;
		double time;
		double intensity_factor;
};

class Calisthenics : public Exercise {
	public:

	//Constructors
		Calisthenics(const Calisthenics &other);
		Calisthenics(std::string Name, std::string Description, Texture2D Photo, double Calories_per_rep, std::string Muscle_group);	

	//Getters
		int get_repetitons();
		int get_max_repetitions();
		int get_sets();
		int get_max_sets();
		std::string get_muscle_group();	

	//Setters
		void set_reps(int reps);
		void set_sets(int sets);
		void reset_sets();
		void reset_reps(); //Unfortunate naming

	//Info functions
		double get_progress() override;
		void update_progress() override;
		virtual double calculate_calories(double user_kg) override;
		virtual double calculate_calories_max(double user_kg) override;
	
	protected:
		int repetitions, max_repetitions;
		int sets, max_sets;
		double calories_per_rep;
		std::string muscle_group;
};

class Weight : public Calisthenics {
	public:
	
	//Constructors
		Weight(const Weight &other, double nweight);
		Weight(std::string Name, std::string Description, Texture2D Photo, double Calories_per_rep, std::string Muscle_group);
	//Getters
		double get_weight();

	//Setters
		void set_weight(double nweight);

	//Info functions
		double calculate_calories(double user_kg) override;
		double calculate_calories_max(double user_kg) override;

	private:
		double weight;
};

#endif
