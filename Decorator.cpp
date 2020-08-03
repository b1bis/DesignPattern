#include <iostream>
#include <string>
#include <memory>

class Consumable
{
public:
    Consumable(float cost = 0., std::string description = "") : m_cost { cost }, 
        m_description { std::move(description) } {}
    virtual float GetCost() { return m_cost; }
    virtual std::string GetDescription() { return m_description; }
    virtual ~Consumable() = 0;
private:
    float m_cost;
    std::string m_description;
};

Consumable::~Consumable() {}

class Beverage : public Consumable
{
public:
    Beverage(float cost, std::string name) : Consumable { cost, std::move(name) } {}
    ~Beverage() = default;
};

class Decorator : public Consumable
{
public:
    Decorator(Consumable& base, float cost = 0., std::string description = "") : 
        Consumable { cost, std::move(description) }, m_base { base } {}
    ~Decorator() = 0;
protected:
    Consumable& m_base;
};

Decorator::~Decorator() {}

class SupplementDecorator : public Decorator
{
public:
    SupplementDecorator(Consumable& base, float cost, std::string name) :
        Decorator { base, cost, std::move(name) } {}
    
    float GetCost() override
    {
        return m_base.GetCost() + Consumable::GetCost();
    }

    std::string GetDescription() override
    {
        return m_base.GetDescription() + " + " + Consumable::GetDescription();
    }
};

int main(int argc, char** argv)
{
    Beverage cafe (3.2, "Cafe");
    SupplementDecorator cafeAuLait (cafe, 0.5, "Lait");
    SupplementDecorator cafeAuLaitSucre (cafeAuLait, 0.2, "Sucre");

    std::cout << cafeAuLaitSucre.GetDescription() << " : " 
        << cafeAuLaitSucre.GetCost() << "F" << std::endl;
    return EXIT_SUCCESS;
}