#include <iostream>
#include <set>
#include <algorithm>
#include <random>
#include <ctime>

class Observer
{
public:
    virtual void Update() = 0;
    virtual ~Observer() = default;
};

class Observable
{
public:
    void Add(Observer* observer) { m_observers.insert(observer); }
    void Remove(Observer* observer) { m_observers.erase(observer); }
protected:
    void Notify()
    {
        std::for_each(m_observers.begin(), m_observers.end(), [](Observer* observer){
            observer->Update();
        });
    }
private:
    std::set<Observer*> m_observers;
};

class WeatherStation : public Observable
{
public:
    void MakeMeasurement()
    {
        static std::default_random_engine engine {time(nullptr)};
        std::normal_distribution<> normalDis(24, 2);
        m_temperature = normalDis(engine);
        std::uniform_int_distribution<unsigned> uniformDis (0, 100);
        m_humidityRate = uniformDis(engine);
        Notify();
    }

    float GetTemperature() { return m_temperature; }
    int GetHumidityRate() { return m_humidityRate; }
private:
    float m_temperature;
    unsigned m_humidityRate;
};

class WeatherDisplay : public Observer
{
public:
    WeatherDisplay(WeatherStation& station) : m_station {station}
    {
        m_station.Add(this);
    }

    void Update() override
    {
        std::cout << "Temperature : " << m_station.GetTemperature() << "C\t"
            << "Humidity rate : " << m_station.GetHumidityRate() << "%" << std::endl;
    }

    ~WeatherDisplay()
    {
        m_station.Remove(this);
    }
private:
    WeatherStation& m_station;
};

int main (int argc, char** argv)
{
    WeatherStation station;

    WeatherDisplay display {station};

    for(size_t i = 0; i < 50; ++i)
        station.MakeMeasurement();

    return EXIT_SUCCESS;
}