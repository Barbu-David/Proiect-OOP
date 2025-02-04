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
		Exercise();
		ExerciseType type;
		Exercise(const Exercise &other);
		std::string get_name();
		std::string get_description();
		Texture2D get_photo();
		virtual double get_progress() = 0;
		virtual void update_progress() = 0;
		virtual double calculate_calories(double user_kg) = 0;
		virtual double calculate_calories_max(double user_kg) = 0;
		ExerciseType get_type();
	protected:
		std::string name;
		std::string description;
		Texture2D photo;
};

class Calisthenics : public Exercise {
	public:
		Calisthenics(const Calisthenics &other);
		Calisthenics(std::string Name, std::string Description, Texture2D Photo, double Calories_per_rep, std::string Muscle_group);	
		double get_progress() override;
		void update_progress() override;
		virtual double calculate_calories(double user_kg) override;
		virtual double calculate_calories_max(double user_kg) override;
		void set_reps(int reps);
		void set_sets(int sets);
		int get_repetitons();
		int get_max_repetitions();
		int get_sets();
		int get_max_sets();
		void reset_sets();
		void reset_reps();
		std::string get_muscle_group();	
	protected:
		int repetitions, max_repetitions;
		int sets, max_sets;
		double calories_per_rep;
		std::string muscle_group;
};

class Running : public Exercise {
	public:
		Running(const Running &other);
		Running(std::string Name, std::string Description, Texture2D Photo, double intensity_factor);
		void set_distance(double distance);
		void set_time(double time);	
		double get_progress() override;
		void update_progress() override;
		double calculate_calories(double user_kg) override;
		double calculate_calories_max(double user_kg) override;
		double get_distance();
		double get_max_distance();
		double get_time();
		double get_intensity_factor();
		void reset_distance();
	private:
		double distance, max_distance;
		double time;
		double intensity_factor;
};

class Weight : public Calisthenics {
	public:
		Weight(const Weight &other, double nweight);
		double calculate_calories(double user_kg) override;
		double calculate_calories_max(double user_kg) override;
		using Calisthenics::Calisthenics;
		void set_weight(double nweight);
		double get_weight();
	private:
		double weight=0;
};

#endif
