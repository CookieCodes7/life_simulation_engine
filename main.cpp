#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>
#include <ctime>

using namespace std;

// ============================================================
// RANDOM NUMBER GENERATOR
// ============================================================

// This class generates random numbers that we can use
// for things like personality, events and decisions.
class Random
{
private:
    mt19937 generator;

public:

    // Constructor: initializes the random number generator.
    Random() : generator(random_device{}()) {}

    // Returns a random integer between min and max.
    int integer(int min, int max)
    {
        uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    // Returns true if the random percentage succeeds.
    bool chance(int percentage)
    {
        return integer(1, 100) <= percentage;
    }
};


// ============================================================
// ENUMS
// ============================================================

// Different types of jobs a person can have.
enum class JobType
{
    Student,
    Employee,
    Entrepreneur,
    Unemployed
};


// Different actions that a person can perform.
enum class Action
{
    Eat,
    Sleep,
    Work,
    Study,
    Exercise,
    Socialize,
    Relax
};


// ============================================================
// NEEDS
// ============================================================

// This class stores the physical and emotional needs
// of a person.
class Needs
{
private:

    int hunger;
    int energy;
    int health;
    int happiness;

public:

    // Constructor: creates a person with default needs.
    Needs()
    {
        hunger = 20;
        energy = 80;
        health = 100;
        happiness = 70;
    }

    // Returns the current hunger level.
    int getHunger() const
    {
        return hunger;
    }

    // Returns the current energy level.
    int getEnergy() const
    {
        return energy;
    }

    // Returns the current health level.
    int getHealth() const
    {
        return health;
    }

    // Returns the current happiness level.
    int getHappiness() const
    {
        return happiness;
    }

    // Increases hunger as time passes.
    void increaseHunger(int amount)
    {
        hunger += amount;
        hunger = min(hunger, 100);
    }

    // Changes energy and keeps it between 0 and 100.
    void changeEnergy(int amount)
    {
        energy += amount;
        energy = max(0, min(energy, 100));
    }

    // Changes health and keeps it between 0 and 100.
    void changeHealth(int amount)
    {
        health += amount;
        health = max(0, min(health, 100));
    }

    // Changes happiness and keeps it between 0 and 100.
    void changeHappiness(int amount)
    {
        happiness += amount;
        happiness = max(0, min(happiness, 100));
    }

    // Updates needs naturally as one day passes.
    void dailyUpdate()
    {
        hunger += 5;
        hunger = min(hunger, 100);

        energy -= 5;
        energy = max(energy, 0);

        // Being extremely hungry damages health.
        if (hunger > 80)
        {
            health -= 5;
        }

        // Very low energy makes a person less happy.
        if (energy < 20)
        {
            happiness -= 5;
        }

        health = max(health, 0);
        happiness = max(happiness, 0);
    }
};


// ============================================================
// PERSONALITY
// ============================================================

// Stores personality traits that influence decisions.
class Personality
{
private:

    int ambition;
    int sociability;
    int discipline;

public:

    // Creates a personality with random traits.
    Personality(Random& random)
    {
        ambition = random.integer(20, 100);
        sociability = random.integer(20, 100);
        discipline = random.integer(20, 100);
    }

    // Returns ambition level.
    int getAmbition() const
    {
        return ambition;
    }

    // Returns sociability level.
    int getSociability() const
    {
        return sociability;
    }

    // Returns discipline level.
    int getDiscipline() const
    {
        return discipline;
    }

    // Displays personality information.
    void display() const
    {
        cout << "Ambition   : " << ambition << "/100\n";
        cout << "Sociability: " << sociability << "/100\n";
        cout << "Discipline : " << discipline << "/100\n";
    }
};


// ============================================================
// PERSON
// ============================================================

// Base class representing a person inside the simulation.
class Person
{
protected:

    string name;
    int age;
    int money;

    Needs needs;
    Personality personality;

    JobType job;

public:

    // Constructor: creates a person with basic information.
    Person(
        string name,
        int age,
        int money,
        Random& random
    )
        : name(name),
          age(age),
          money(money),
          personality(random)
    {
        job = JobType::Unemployed;
    }

    // Virtual destructor allows safe inheritance.
    virtual ~Person() = default;


    // ========================================================
    // GETTERS
    // ========================================================

    // Returns the person's name.
    string getName() const
    {
        return name;
    }

    // Returns the person's age.
    int getAge() const
    {
        return age;
    }

    // Returns the person's money.
    int getMoney() const
    {
        return money;
    }

    // Returns the person's needs.
    const Needs& getNeeds() const
    {
        return needs;
    }

    // Returns the person's job.
    JobType getJob() const
    {
        return job;
    }


    // ========================================================
    // BASIC ACTIONS
    // ========================================================

    // Makes the person buy food and reduce hunger.
    virtual void eat()
    {
        if (money >= 50)
        {
            money -= 50;

            needs.increaseHunger(-35);
            needs.changeHappiness(5);
        }
    }


    // Makes the person sleep and recover energy.
    virtual void sleep()
    {
        needs.changeEnergy(40);
        needs.increaseHunger(5);
    }


    // Makes the person work and earn money.
    virtual void work()
    {
        money += 100;

        needs.changeEnergy(-25);
        needs.increaseHunger(10);
        needs.changeHappiness(3);
    }


    // Makes the person study.
    virtual void study()
    {
        needs.changeEnergy(-20);
        needs.increaseHunger(8);
        needs.changeHappiness(2);
    }


    // Makes the person exercise.
    virtual void exercise()
    {
        needs.changeEnergy(-20);
        needs.increaseHunger(8);

        needs.changeHealth(3);
        needs.changeHappiness(8);
    }


    // Makes the person socialize with others.
    virtual void socialize()
    {
        needs.changeEnergy(-10);
        needs.changeHappiness(12);
    }


    // Makes the person relax.
    virtual void relax()
    {
        needs.changeEnergy(10);
        needs.changeHappiness(5);
    }


    // ========================================================
    // DECISION SYSTEM
    // ========================================================

    // Calculates how useful eating would be.
    virtual int scoreEat()
    {
        int score = 0;

        // The hungrier someone is, the more valuable eating becomes.
        score += needs.getHunger();

        // People with low happiness also benefit slightly from eating.
        if (needs.getHappiness() < 40)
        {
            score += 10;
        }

        // Can't eat if there isn't enough money.
        if (money < 50)
        {
            score = 0;
        }

        return score;
    }


    // Calculates how useful sleeping would be.
    virtual int scoreSleep()
    {
        int score = 100 - needs.getEnergy();

        return score;
    }


    // Calculates how useful working would be.
    virtual int scoreWork()
    {
        int score = 0;

        // Ambitious people value work more.
        score += personality.getAmbition();

        // Having little money makes earning money more important.
        if (money < 1000)
        {
            score += 30;
        }

        // Very low energy makes working less attractive.
        if (needs.getEnergy() < 25)
        {
            score -= 40;
        }

        return max(score, 0);
    }


    // Calculates how useful exercising would be.
    virtual int scoreExercise()
    {
        int score = 20;

        // Low health increases the importance of exercise.
        if (needs.getHealth() < 70)
        {
            score += 40;
        }

        // Exercise becomes less attractive when exhausted.
        if (needs.getEnergy() < 30)
        {
            score -= 30;
        }

        return max(score, 0);
    }


    // Calculates how useful socializing would be.
    virtual int scoreSocialize()
    {
        int score = personality.getSociability();

        // Low happiness increases the value of social interaction.
        if (needs.getHappiness() < 50)
        {
            score += 25;
        }

        return min(score, 100);
    }


    // Calculates how useful relaxing would be.
    virtual int scoreRelax()
    {
        int score = 30;

        // Very low happiness makes relaxation more attractive.
        if (needs.getHappiness() < 40)
        {
            score += 30;
        }

        return score;
    }


    // Chooses the action with the highest utility score.
    virtual Action decideAction()
    {
        vector<pair<Action, int>> actions =
        {
            {Action::Eat, scoreEat()},
            {Action::Sleep, scoreSleep()},
            {Action::Work, scoreWork()},
            {Action::Exercise, scoreExercise()},
            {Action::Socialize, scoreSocialize()},
            {Action::Relax, scoreRelax()}
        };

        // Sort actions from highest score to lowest score.
        sort(
            actions.begin(),
            actions.end(),
            [](const auto& a, const auto& b)
            {
                return a.second > b.second;
            }
        );

        // The first action now has the highest utility.
        return actions[0].first;
    }


    // Performs whichever action the decision engine selected.
    void performAction(Action action)
    {
        switch (action)
        {
            case Action::Eat:
                eat();
                break;

            case Action::Sleep:
                sleep();
                break;

            case Action::Work:
                work();
                break;

            case Action::Study:
                study();
                break;

            case Action::Exercise:
                exercise();
                break;

            case Action::Socialize:
                socialize();
                break;

            case Action::Relax:
                relax();
                break;
        }
    }


    // Updates the person's needs and then lets them make
    // an autonomous decision.
    virtual void simulateDay()
    {
        needs.dailyUpdate();

        Action action = decideAction();

        performAction(action);
    }


    // ========================================================
    // DISPLAY
    // ========================================================

    // Displays all important information about the person.
    virtual void display() const
    {
        cout << "\n----------------------------------------\n";

        cout << "Name       : " << name << '\n';
        cout << "Age        : " << age << '\n';
        cout << "Money      : Rs." << money << '\n';

        cout << "Health     : "
             << needs.getHealth()
             << "/100\n";

        cout << "Hunger     : "
             << needs.getHunger()
             << "/100\n";

        cout << "Energy     : "
             << needs.getEnergy()
             << "/100\n";

        cout << "Happiness  : "
             << needs.getHappiness()
             << "/100\n";

        cout << "Job        : ";

        switch (job)
        {
            case JobType::Student:
                cout << "Student";
                break;

            case JobType::Employee:
                cout << "Employee";
                break;

            case JobType::Entrepreneur:
                cout << "Entrepreneur";
                break;

            default:
                cout << "Unemployed";
        }

        cout << "\n\nPersonality\n";

        personality.display();

        cout << "----------------------------------------\n";
    }
};


// ============================================================
// STUDENT
// ============================================================

// Student is a specialized type of Person.
class Student : public Person
{
public:

    // Creates a student and assigns the Student job type.
    Student(
        string name,
        int age,
        int money,
        Random& random
    )
        : Person(name, age, money, random)
    {
        job = JobType::Student;
    }


    // Students prioritize studying more than normal people.
    Action decideAction() override
    {
        // Critical needs always come first.
        if (scoreEat() > 80)
            return Action::Eat;

        if (scoreSleep() > 80)
            return Action::Sleep;

        // Students use their discipline to decide whether
        // studying should be their main activity.
        if (personality.getDiscipline() > 50)
            return Action::Study;

        return Person::decideAction();
    }


    // Students earn less money from part-time work.
    void work() override
    {
        money += 50;

        needs.changeEnergy(-20);
        needs.increaseHunger(8);
    }


    // Students have their own study action.
    void study() override
    {
        needs.changeEnergy(-20);
        needs.increaseHunger(8);

        // Studying slightly increases happiness.
        needs.changeHappiness(3);
    }
};


// ============================================================
// EMPLOYEE
// ============================================================

// Employee represents a working adult.
class Employee : public Person
{
public:

    // Creates an employee.
    Employee(
        string name,
        int age,
        int money,
        Random& random
    )
        : Person(name, age, money, random)
    {
        job = JobType::Employee;
    }


    // Employees prioritize work when they have enough energy.
    Action decideAction() override
    {
        if (scoreEat() > 80)
            return Action::Eat;

        if (scoreSleep() > 80)
            return Action::Sleep;

        if (scoreWork() > 70)
            return Action::Work;

        return Person::decideAction();
    }


    // Employees earn a regular salary.
    void work() override
    {
        money += 150;

        needs.changeEnergy(-30);
        needs.increaseHunger(12);
    }
};


// ============================================================
// ENTREPRENEUR
// ============================================================

// Entrepreneur represents a person running a business.
class Entrepreneur : public Person
{
public:

    // Creates an entrepreneur.
    Entrepreneur(
        string name,
        int age,
        int money,
        Random& random
    )
        : Person(name, age, money, random)
    {
        job = JobType::Entrepreneur;
    }


    // Entrepreneurs strongly prioritize working.
    Action decideAction() override
    {
        if (scoreEat() > 80)
            return Action::Eat;

        if (scoreSleep() > 80)
            return Action::Sleep;

        // Entrepreneurs are strongly influenced by ambition.
        if (personality.getAmbition() > 60)
            return Action::Work;

        return Person::decideAction();
    }


    // Entrepreneurs have variable income.
    void work() override
    {
        int income = 100 + rand() % 401;

        money += income;

        needs.changeEnergy(-35);
        needs.increaseHunger(15);
    }
};


// ============================================================
// SIMULATION ENGINE
// ============================================================

// This class controls the entire simulated world.
class SimulationEngine
{
private:

    // Stores every person in the simulated population.
    vector<Person*> population;

    // Keeps track of simulation time.
    int day;
    int year;

    // Random number generator for the simulation.
    Random random;

public:

    // Constructor: starts the simulation at Day 1.
    SimulationEngine()
    {
        day = 1;
        year = 1;
    }


    // Destructor: cleans up dynamically allocated people.
    ~SimulationEngine()
    {
        for (Person* person : population)
        {
            delete person;
        }
    }


    // Adds a new person to the simulation.
    void addPerson(Person* person)
    {
        population.push_back(person);
    }


    // Returns the current population size.
    int getPopulation() const
    {
        return population.size();
    }


    // Simulates one complete day.
    void simulateDay()
    {
        cout << "\n========================================\n";
        cout << "              DAY " << day << '\n';
        cout << "========================================\n";

        // Every person updates their needs and makes
        // an autonomous decision.
        for (Person* person : population)
        {
            person->simulateDay();
        }

        // Random world events happen after people's actions.
        randomEvent();

        // Advance simulation time.
        day++;

        // After 365 days, move to the next year.
        if (day > 365)
        {
            day = 1;
            year++;

            cout << "\n*** A NEW YEAR HAS BEGUN ***\n";
        }
    }


    // Generates random events that affect the population.
    void randomEvent()
    {
        int event = random.integer(1, 100);

        // 5% chance of a festival.
        if (event <= 5)
        {
            cout << "\n*** FESTIVAL ***\n";

            // Festivals increase everyone's happiness.
            for (Person* person : population)
            {
                person->socialize();
            }
        }

        // 5% chance of an economic event.
        else if (event <= 10)
        {
            cout << "\n*** ECONOMIC BOOM ***\n";

            // Everyone receives a small financial boost.
            for (Person* person : population)
            {
                person->work();
            }
        }

        // 5% chance of a heat wave.
        else if (event <= 15)
        {
            cout << "\n*** HEAT WAVE ***\n";

            // Heat makes people lose energy.
            for (Person* person : population)
            {
                person->sleep();
            }
        }
    }


    // Displays all people currently living in the world.
    void displayPopulation() const
    {
        cout << "\n========================================\n";
        cout << "              POPULATION\n";
        cout << "========================================\n";

        for (int i = 0; i < population.size(); i++)
        {
            cout << i + 1
                 << ". "
                 << population[i]->getName()
                 << " | Age: "
                 << population[i]->getAge()
                 << " | Money: Rs."
                 << population[i]->getMoney()
                 << '\n';
        }
    }


    // Allows the user to inspect an individual person.
    void displayPerson()
    {
        displayPopulation();

        cout << "\nEnter person number: ";

        int index;
        cin >> index;

        if (index < 1 || index > population.size())
        {
            cout << "Invalid person.\n";
            return;
        }

        population[index - 1]->display();
    }


    // Calculates and displays statistics for the entire population.
    void displayStatistics() const
    {
        if (population.empty())
            return;

        int totalMoney = 0;
        int totalHealth = 0;
        int totalHappiness = 0;
        int totalEnergy = 0;

        // Add together everyone's statistics.
        for (Person* person : population)
        {
            totalMoney += person->getMoney();

            totalHealth +=
                person->getNeeds().getHealth();

            totalHappiness +=
                person->getNeeds().getHappiness();

            totalEnergy +=
                person->getNeeds().getEnergy();
        }

        cout << "\n========================================\n";
        cout << "              STATISTICS\n";
        cout << "========================================\n";

        cout << fixed << setprecision(1);

        cout << "Population        : "
             << population.size()
             << '\n';

        cout << "Average Money     : Rs."
             << (double)totalMoney / population.size()
             << '\n';

        cout << "Average Health    : "
             << (double)totalHealth / population.size()
             << '\n';

        cout << "Average Happiness : "
             << (double)totalHappiness / population.size()
             << '\n';

        cout << "Average Energy    : "
             << (double)totalEnergy / population.size()
             << '\n';

        cout << "Year              : "
             << year
             << '\n';

        cout << "Day               : "
             << day
             << '\n';
    }
};


// ============================================================
// MAIN
// ============================================================

// Program execution starts here.
int main()
{
    SimulationEngine simulation;

    // --------------------------------------------------------
    // CREATE INITIAL POPULATION
    // --------------------------------------------------------

    // Create students.
    simulation.addPerson(
        new Student("Alex", 19, 1000, *(new Random()))
    );

    simulation.addPerson(
        new Student("Maya", 21, 1500, *(new Random()))
    );

    // Create employees.
    simulation.addPerson(
        new Employee("Daniel", 28, 5000, *(new Random()))
    );

    simulation.addPerson(
        new Employee("Sarah", 32, 7000, *(new Random()))
    );

    // Create entrepreneurs.
    simulation.addPerson(
        new Entrepreneur("Ryan", 35, 10000, *(new Random()))
    );

    simulation.addPerson(
        new Entrepreneur("Olivia", 40, 15000, *(new Random()))
    );


    // --------------------------------------------------------
    // MAIN MENU
    // --------------------------------------------------------

    while (true)
    {
        cout << "\n\n";

        cout << "========================================\n";
        cout << "       LIFE SIMULATION ENGINE\n";
        cout << "========================================\n";

        cout << "1. Simulate one day\n";
        cout << "2. Simulate 7 days\n";
        cout << "3. Simulate 30 days\n";
        cout << "4. View population\n";
        cout << "5. View person\n";
        cout << "6. View statistics\n";
        cout << "7. Exit\n";

        cout << "\nChoose: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:

                simulation.simulateDay();

                break;


            case 2:

                // Simulate seven days.
                for (int i = 0; i < 7; i++)
                {
                    simulation.simulateDay();
                }

                break;


            case 3:

                // Simulate thirty days.
                for (int i = 0; i < 30; i++)
                {
                    simulation.simulateDay();
                }

                break;


            case 4:

                simulation.displayPopulation();

                break;


            case 5:

                simulation.displayPerson();

                break;


            case 6:

                simulation.displayStatistics();

                break;


            case 7:

                cout << "\nSimulation shutting down...\n";

                return 0;


            default:

                cout << "\nInvalid option.\n";
        }
    }
}