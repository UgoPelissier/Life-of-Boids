#include "common.hpp"

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

Vec::operator+(Vec b) {
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
}
//===============Vec class functions definitions end================


//===============Agent class functions definitions================

Agent::Agent(int x, int y, double angle, double velocity) {

    this->vec = Vec(x, y, angle, velocity);
}

bool Agent::update_vector() {

    this->vec.X() = x;
    this->vec.Y() = y;
    this->vec.theta() = angle;
    this->vec.speed() = velocity;
}


//===============Agent class functions definitions end================
