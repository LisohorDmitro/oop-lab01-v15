#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

struct PhoneCall {
    int first = 1;
    double second = 1;
    void Init(int minutes, double price) {
        if (minutes <= 0) throw std::invalid_argument("Minutes must be positive");
        if (!std::isfinite(price) || price <= 0)
            throw std::invalid_argument("Price must be finite and positive");
        first = minutes; second = price;
    }
    void Read() {
        int minutes; double price;
        if (!(std::cin >> minutes >> price)) throw std::runtime_error("Invalid input");
        Init(minutes, price);
    }
    void Display() const {
        std::cout << "Minutes: " << first << "; price per minute: "
                  << std::fixed << std::setprecision(2) << second << " UAH\n";
    }
    double cost() const {
        if (first <= 0 || !std::isfinite(second) || second <= 0)
            throw std::domain_error("Invalid call state");
        long double result = static_cast<long double>(first) * second;
        if (!std::isfinite(result) || result > std::numeric_limits<double>::max())
            throw std::overflow_error("Call cost overflow");
        return static_cast<double>(result);
    }
};
void Require(bool ok) { if (!ok) throw std::logic_error("Self-check failed"); }
void SelfCheck() {
    PhoneCall call;
    call.Init(7, 2.35); Require(std::fabs(call.cost() - 16.45) < 1e-10);
    for (int minutes : {0, -3}) {
        bool rejected = false;
        try { call.Init(minutes, 2.35); }
        catch (const std::invalid_argument&) { rejected = true; }
        Require(rejected && call.first == 7);
    }
    for (double price : {0.0, -1.0, std::numeric_limits<double>::infinity()}) {
        bool rejected = false;
        try { call.Init(7, price); }
        catch (const std::invalid_argument&) { rejected = true; }
        Require(rejected && call.second == 2.35);
    }
    bool rejected = false;
    try { call.Init(2, std::numeric_limits<double>::max()); call.cost(); }
    catch (const std::overflow_error&) { rejected = true; }
    Require(rejected);
    call.first = 0; rejected = false;
    try { call.cost(); } catch (const std::domain_error&) { rejected = true; }
    Require(rejected);
    std::cout << "Self-checks: PASS\n";
}
int main() {
    try {
        SelfCheck();
        PhoneCall call;
        std::cout << "Enter minutes and price per minute: "; call.Read();
        std::cout << '\n'; call.Display();
        std::cout << "Total cost: " << std::fixed << std::setprecision(2)
                  << call.cost() << " UAH\n";
        try { call.Init(0, 2.35); }
        catch (const std::invalid_argument& e) {
            std::cout << "Expected error: " << e.what() << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n'; return 1;
    }
}
