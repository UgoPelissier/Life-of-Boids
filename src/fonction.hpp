
class Vec {
private:
	int x;
	int y;
	double angle;
	double velocity;

public:
	Vec() {
		this->x = 0;
		this->y = 0;
		this->angle = 0;
		this->velocity = 0;
	}
	Vec(int x, int y, double angle, double velocity) {
		this->x = x;
		this->y = y;
		this->angle = angle;
		this->velocity = velocity;
	}
	int& X() { return x; }
	int &Y() { return y; }
	double &theta() { return angle; }
	double &speed() { return velocity; }

	Vec operator+(Vec b) {
		Vec result = Vec(this->X() + b.X(),
						this->Y() + b.Y(),
						this->theta() + b.theta(),
						this->speed()+b.speed());


		return result;
	}

	Vec operator-(Vec b) {
		Vec result = Vec(this->X() - b.X(),
			this->Y() - b.Y(),
			this->theta() - b.theta(),
			this->speed() - b.speed());

		return result;
	}
	Vec operator*(Vec b) {
		Vec result = Vec(this->X() * b.X(),
			this->Y() * b.Y(),
			this->theta() * b.theta(),
			this->speed() * b.speed());

		return result;
	}
	Vec operator*(double b) {
		Vec result = Vec(this->X() * b,
			this->Y() * b,
			this->theta() * b,
			this->speed() * b);

		return result;
	}

};

class Agent {
private:
	Vec vec;

public:
	Agent(int x, int y, double angle, double velocity) {
		this->vec = Vec(x, y, angle, velocity);
	}
};

