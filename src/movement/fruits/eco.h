#pragma once
#include "../object.h"
#include "../obstacles/obstacle.h"

class Eco : public Object {
protected:
	bool m_obstacle;
public:
	
	Eco();
	Eco(Real const& x, Real const& y);

	bool get_obstacle() const;
	bool nearBorder() const;
	void closestObstacle(std::vector<Obstacle> const& obstacles);
	
	~Eco();
};