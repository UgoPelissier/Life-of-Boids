#include "common.hpp"

//====================USEFUL FUNCTIONS================================
using vec2 = std::array<float, 2>;

double distance(vec2 p1, vec2 p2) {
    double d(0);
    d = std::sqrt( (p2[0]-p1[0])*(p2[0]-p1[0]) +  (p2[1]-p1[1])*(p2[1]-p1[1]) );
    return d;
}

bool overlap(vec2 c, std::vector<vec2> centers) {
    float h = triangleSize*std::sqrt(3)/2;
    float l = 2*h/3;
    for (auto& center : centers) {
        if (distance(c, center)<2*l) {
            return true;
        }
    }
    return false;
}

bool outsideWindow(vec2 center, float ratio) {
    float h = triangleSize*std::sqrt(3)/2;
    float l = 2*h/3;
    if ( std::abs(center[0])>ratio-l or std::abs(center[1])>1-l ) {
        return true;
    }
    return false;
}

std::tuple<std::vector<vec2>, std::vector<double>, std::vector<double>> initialiazeAgents() {

    std::vector<vec2> points;
    vec2 randomPoint;

    std::vector<double> angles;
    double randomAngle;

    std::vector<double> velocities;
    double randomVelocity;

    float ratio = (float)WIDTH / (float)HEIGHT;

    std::uniform_real_distribution<double> unif(0, 1); // Uniform distribution on [0:1] => Random number between 0 and 1
    std::mt19937_64 rng;

    for (size_t i = 0; i<DEFAULT_NUM_AGENTS; ++i) {

        randomPoint = {(float)(ratio*((2*unif(rng))-1)), (float)((2*unif(rng))-1)};
        randomAngle = 2*PI*unif(rng);
        randomVelocity = 2*unif(rng)-1;

        if ( (not overlap(randomPoint, points)) and (not outsideWindow(randomPoint, ratio)) ) {
            points.push_back(randomPoint);
            angles.push_back(randomAngle);
            velocities.push_back(randomVelocity);
        }
    }

    return std::make_tuple(points, angles, velocities);
}
//====================USEFUL FUNCTIONS END============================

//===============Vec class functions definitions================

Vec::Vec() {
    this->x = 0;
    this->y = 0;
    this->angle = 0;
    this->velocity = 0;
}

Vec::Vec(int x, int y, double angle, double velocity) {
    this->x = x;
    this->y = y;
    this->angle = angle;
    this->velocity = velocity;
}
int& Vec::X() {
    return x;
}

int& Vec::Y() {
    return y;
}

double& Vec::theta() {
    return angle;
}

double& Vec::speed() {
    return velocity;
}

/*Vec::operator+(Vec b) {
    Vec result = Vec(this->X() + b.X(),
                     this->Y() + b.Y(),
                     this->theta() + b.theta(),
                     this->speed() + b.speed());


    return result;
}

Vec::operator-(Vec b) {
    Vec result = Vec(this->X() - b.X(),
                     this->Y() - b.Y(),
                     this->theta() - b.theta(),
                     this->speed() - b.speed());

    return result;
}

Vec::operator*(Vec b) {
    Vec result = Vec(this->X() * b.X(),
                     this->Y() * b.Y(),
                     this->theta() * b.theta(),
                     this->speed() * b.speed());

    return result;
}

Vec::Vec operator*(double b) {
    Vec result = Vec(this->X() * b,
                     this->Y() * b,
                     this->theta() * b,
                     this->speed() * b);

    return result;
}*/
//===============Vec class functions definitions end================


//===============Agent class functions definitions================

Agent::Agent(int x, int y, double angle, double velocity) {

    this->vec = Vec(x, y, angle, velocity);
}

/*bool Agent::update_vector() {

    this->vec.X() = x;
    this->vec.Y() = y;
    this->vec.theta() = angle;
    this->vec.speed() = velocity;
}*/


//===============Agent class functions definitions end================
