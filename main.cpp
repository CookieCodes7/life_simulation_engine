#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace std;

// ============================================================
// RANDOM NUMBER GENERATOR
// ============================================================

class Random
{
private:
    mt19937 generator;

public:
    Random() : generator(random_device{}()) {}

    int integer(int min, int max)
    {
        uniform_int_distribution<int> distribution(min, max);
        return distribution(generator);
    }

    bool chance(int percentage)
    {
        return integer(1, 100) <= percentage;
    }
};

// ============================================================
// ENUMS
// ============================================================

enum class JobType
{
    Student,
    Employee,
    Entrepreneur,
    Unemployed
};

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
// PERSON
// ============================================================

class Person
{
protected:
    string name;
    int age;
    int money;

    int health;
    int hunger;
    int energy;
    int happiness;

    JobType job;

public:

    Person(string name, int age, int money)
        : name(name),
          age(age),
          money(money),
          health(100),
          hunger(20),
          energy(80),
          happiness(70),
          job(JobType::Unemployed)
    {
    }

    virtual ~Person() = default;

    // --------------------------------------------------------
    // GETTERS
    // --------------------------------------------------------

    string getName() const
    {
        return name;
    }

    int getAge() const
    {
        return age;
    }

    int getMoney() const
    {
        return money;
    }

    int getHealth() const
    {
        return health;
    }

    int getHunger() const
    {
        return hunger;
    }

    int getEnergy() const
    {
        return energy;
    }

    int getHappiness() const
    {
        return happiness;
    }

    JobType getJob() const
    {
        return job;
    }

    // --------------------------------------------------------
    // JOB
    // --------------------------------------------------------

    void setJob(JobType newJob)
    {
        job = newJob;
    }

    // --------------------------------------------------------
    // BASIC ACTIONS
    // --------------------------------------------------------

    virtual void eat()
    {
        if (money >= 50)
        {
            money -= 50;

            hunger -= 35;
            hunger = max(hunger, 0);

            happiness += 5;
            happiness = min(happiness, 100);
        }
    }

    virtual void sleep()
    {
        energy += 40;
        energy = min(energy, 100);

        hunger += 5;
        hunger = min(hunger, 100);
    }

    virtual void work()
    {
        money += 100;

        energy -= 25;
        hunger += 10;
        happiness += 3;

        energy = max(energy, 0);
        hunger = min(hunger, 100);
        happiness = min(happiness, 100);
    }

    virtual void study()
    {
        energy -= 20;
        hunger += 8;
        happiness += 2;

        energy = max(energy, 0);
        hunger = min(hunger, 100);
        happiness = min(happiness, 100);
    }

    virtual void exercise()
    {
        energy -= 20;
        hunger += 8;
        health += 3;
        happiness += 8;

        energy = max(energy, 0);
        hunger = min(hunger, 100);
        health = min(health, 100);
        happiness = min(happiness, 100);
    }

    virtual void socialize()
    {
        energy -= 10;
        happiness += 12;

        energy = max(energy, 0);
        happiness = min(happiness, 100);
    }

    virtual void relax()
    {
        energy += 10;
        happiness += 5;

        energy = min(energy, 100);
        happiness = min(happiness, 100);
    }

    // --------------------------------------------------------
    // DAILY UPDATE
    // --------------------------------------------------------

    virtual void dailyUpdate()
    {
        hunger += 5;
        energy -= 5;

        hunger = min(hunger, 100);
        energy = max(energy, 0);

        // Hunger affects health
        if (hunger > 80)
        {
            health -= 5;
        }

        // Very low energy affects happiness
        if (energy < 20)
        {
            happiness -= 5;
        }

        health = max(health, 0);
        happiness = max(happiness, 0);
    }

    // --------------------------------------------------------
    // DECISION MAKING
    // --------------------------------------------------------

    virtual Action decideAction()
    {
        // Emergency priorities

        if (hunger >= 70 && money >= 50)
            return Action::Eat;

        if (energy <= 20)
            return Action::Sleep;

        if (health < 40)
            return Action::Exercise;

        // Random behaviour

        int decision = rand() % 100;

        if (decision < 30)
            return Action::Work;

        if (decision < 45)
            return Action::Exercise;

        if (decision < 60)
            return Action::Socialize;

        if (decision < 75)
            return Action::Relax;

        return Action::Work;
    }

    // --------------------------------------------------------
    // PERFORM ACTION
    // --------------------------------------------------------

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

    // --------------------------------------------------------
    // DISPLAY
    // --------------------------------------------------------

    virtual void display() const
    {
        cout << "\n----------------------------------------\n";

        cout << "Name       : " << name << '\n';
        cout << "Age        : " << age << '\n';
        cout << "Money      : Rs." << money << '\n';
        cout << "Health     : " << health << "/100\n";
        cout << "Hunger     : " << hunger << "/100\n";
        cout << "Energy     : " << energy << "/100\n";
        cout << "Happiness  : " << happiness << "/100\n";

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

            case JobType::Unemployed:
                cout << "Unemployed";
                break;
        }

        cout << '\n';

        cout << "----------------------------------------\n";
    }
};

// ============================================================
// STUDENT
// ============================================================

class Student : public Person
{
public:

    Student(string name, int age, int money)
        : Person(name, age, money)
    {
        job = JobType::Student;
    }

    Action decideAction() override
    {
        if (hunger >= 70 && money >= 50)
            return Action::Eat;

        if (energy <= 20)
            return Action::Sleep;

        int decision = rand() % 100;

        if (decision < 40)
            return Action::Study;

        if (decision < 60)
            return Action::Socialize;

        if (decision < 75)
            return Action::Exercise;

        return Action::Relax;
    }

    void work() override
    {
        // Students have part-time jobs
        money += 50;

        energy -= 20;
        hunger += 8;

        energy = max(energy, 0);
        hunger = min(hunger, 100);
    }
};

// ============================================================
// EMPLOYEE
// ============================================================

class Employee : public Person
{
public:

    Employee(string name, int age, int money)
        : Person(name, age, money)
    {
        job = JobType::Employee;
    }

    Action decideAction() override
    {
        if (hunger >= 70 && money >= 50)
            return Action::Eat;

        if (energy <= 20)
            return Action::Sleep;

        int decision = rand() % 100;

        if (decision < 50)
            return Action::Work;

        if (decision < 70)
            return Action::Socialize;

        if (decision < 85)
            return Action::Exercise;

        return Action::Relax;
    }

    void work() override
    {
        money += 150;

        energy -= 30;
        hunger += 12;

        energy = max(energy, 0);
        hunger = min(hunger, 100);
    }
};

// ============================================================
// ENTREPRENEUR
// ============================================================

class Entrepreneur : public Person
{
public:

    Entrepreneur(string name, int age, int money)
        : Person(name, age, money)
    {
        job = JobType::Entrepreneur;
    }

    Action decideAction() override
    {
        if (hunger >= 70 && money >= 50)
            return Action::Eat;

        if (energy <= 20)
            return Action::Sleep;

        int decision = rand() % 100;

        if (decision < 60)
            return Action::Work;

        if (decision < 75)
            return Action::Socialize;

        if (decision < 90)
            return Action::Exercise;

        return Action::Relax;
    }

    void work() override
    {
        // Entrepreneurs have more variable income
        int income = 100 + (rand() % 301);

        money += income;

        energy -= 35;
        hunger += 15;

        energy = max(energy, 0);
        hunger = min(hunger, 100);
    }
};

// ============================================================
// WORLD
// ============================================================

class World
{
private:

    vector<Person*> population;

    int day;
    int year;

public:

    World()
        : day(1), year(1)
    {
    }

    ~World()
    {
        for (Person* person : population)
        {
            delete person;
        }
    }

    // --------------------------------------------------------
    // ADD PERSON
    // --------------------------------------------------------

    void addPerson(Person* person)
    {
        population.push_back(person);
    }

    // --------------------------------------------------------
    // GET POPULATION
    // --------------------------------------------------------

    int getPopulation() const
    {
        return population.size();
    }

    // --------------------------------------------------------
    // SIMULATE ONE DAY
    // --------------------------------------------------------

    void simulateDay()
    {
        cout << "\n========================================\n";
        cout << "             DAY " << day << '\n';
        cout << "========================================\n";

        for (Person* person : population)
        {
            person->dailyUpdate();

            Action action = person->decideAction();

            person->performAction(action);
        }

        randomEvent();

        day++;

        if (day > 365)
        {
            day = 1;
            year++;
        }
    }

    // --------------------------------------------------------
    // RANDOM EVENTS
    // --------------------------------------------------------

    void randomEvent()
    {
        int event = rand() % 100;

        if (event < 5)
        {
            cout << "\n*** FESTIVAL ***\n";

            for (Person* person : population)
            {
                // Festival makes everyone happier
                // indirectly through a social event
                person->socialize();
            }
        }

        else if (event < 8)
        {
            cout << "\n*** ECONOMIC RECESSION ***\n";

            for (Person* person : population)
            {
                // Everyone loses some money
                person->work();

                // Reverse most of the income
                // to represent reduced earnings
            }
        }

        else if (event < 10)
        {
            cout << "\n*** HEAT WAVE ***\n";

            for (Person* person : population)
            {
                person->exercise();
            }
        }
    }

    // --------------------------------------------------------
    // DISPLAY POPULATION
    // --------------------------------------------------------

    void displayPopulation() const
    {
        cout << "\n========================================\n";
        cout << "             POPULATION\n";
        cout << "========================================\n";

        for (int i = 0; i < population.size(); i++)
        {
            cout << i + 1 << ". "
                 << population[i]->getName()
                 << " | Age: "
                 << population[i]->getAge()
                 << " | Money: Rs."
                 << population[i]->getMoney()
                 << '\n';
        }
    }

    // --------------------------------------------------------
    // DISPLAY PERSON
    // --------------------------------------------------------

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

    // --------------------------------------------------------
    // STATISTICS
    // --------------------------------------------------------

    void displayStatistics() const
    {
        if (population.empty())
            return;

        int totalMoney = 0;
        int totalHealth = 0;
        int totalHappiness = 0;
        int totalEnergy = 0;

        for (Person* person : population)
        {
            totalMoney += person->getMoney();
            totalHealth += person->getHealth();
            totalHappiness += person->getHappiness();
            totalEnergy += person->getEnergy();
        }

        cout << "\n========================================\n";
        cout << "             STATISTICS\n";
        cout << "========================================\n";

        cout << "Population        : " << population.size() << '\n';

        cout << fixed << setprecision(1);

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

        cout << "Year              : " << year << '\n';
        cout << "Day               : " << day << '\n';
    }
};

// ============================================================
// MAIN
// ============================================================

int main()
{
    srand(time(nullptr));

    World world;

    // --------------------------------------------------------
    // CREATE INITIAL POPULATION
    // --------------------------------------------------------

    world.addPerson(
        new Student("Alex", 19, 1000)
    );

    world.addPerson(
        new Student("Maya", 21, 1500)
    );

    world.addPerson(
        new Employee("Daniel", 28, 5000)
    );

    world.addPerson(
        new Employee("Sarah", 32, 7000)
    );

    world.addPerson(
        new Entrepreneur("Ryan", 35, 10000)
    );

    world.addPerson(
        new Employee("Emma", 26, 4500)
    );

    world.addPerson(
        new Student("Noah", 20, 800)
    );

    world.addPerson(
        new Entrepreneur("Olivia", 40, 15000)
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

                world.simulateDay();

                break;

            case 2:

                for (int i = 0; i < 7; i++)
                    world.simulateDay();

                break;

            case 3:

                for (int i = 0; i < 30; i++)
                    world.simulateDay();

                break;

            case 4:

                world.displayPopulation();

                break;

            case 5:

                world.displayPerson();

                break;

            case 6:

                world.displayStatistics();

                break;

            case 7:

                cout << "\nSimulation shutting down...\n";

                return 0;

            default:

                cout << "\nInvalid option.\n";
        }
    }
}