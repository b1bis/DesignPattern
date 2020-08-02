#include <iostream>
#include <memory>

class OperationStrategy
{
public:
    virtual int Calculate(int a, int b) = 0;
    virtual ~OperationStrategy() = default;
};

class AddStrategy : public OperationStrategy
{
public:
    int Calculate(int a, int b) override
    {
        return a + b;
    }
};

class SubtractStrategy : public OperationStrategy
{
public:
    int Calculate(int a, int b) override
    {
        return a - b;
    }
};

class MultiplyStrategy : public OperationStrategy
{
public:
    int Calculate(int a, int b) override
    {
        return a * b;
    }
};

class DivideStrategy : public OperationStrategy
{
public:
    int Calculate(int a, int b) override
    {
        return a / b;
    }
};

class Calculator
{
public:
    Calculator() = default;
    Calculator(std::unique_ptr<OperationStrategy> strategy) : m_strategy { std::move(strategy) } {}
    void SetStrategy(std::unique_ptr<OperationStrategy> strategy) { m_strategy = std::move(strategy); }

    int Calculate(int a, int b)
    {
        if(!m_strategy)
            throw std::runtime_error("Attempt to calculate with no strategy");
        return m_strategy->Calculate(a, b);
    }
private:
    std::unique_ptr<OperationStrategy> m_strategy;
};

int main(int argc, char** argv)
{
    Calculator calculator;

    calculator.SetStrategy(std::make_unique<AddStrategy>());
    std::cout << "5 + 6 = " << calculator.Calculate(5, 6) << std::endl;

    calculator.SetStrategy(std::make_unique<SubtractStrategy>());
    std::cout << "8 - 65 = " << calculator.Calculate(8, 65) << std::endl;

    calculator.SetStrategy(std::make_unique<MultiplyStrategy>());
    std::cout << "7 x 6 = " << calculator.Calculate(7, 6) << std::endl;

    calculator.SetStrategy(std::make_unique<DivideStrategy>());
    std::cout << "24 / 2 = " << calculator.Calculate(24, 2) << std::endl;
    
    return EXIT_SUCCESS;
}