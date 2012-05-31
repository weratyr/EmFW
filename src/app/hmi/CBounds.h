#ifndef CBOUNDS_H_
#define CBOUNDS_H_

#include "CPosition.h"

#ifndef _GLOBAL_H
#include "Global.h"
#endif

class CBounds
{
public:
	friend std::ostream &operator<< (std::ostream &ostr, const CBounds& Bounds);

	CBounds();
	CBounds(const CPosition& NorthEast,const CPosition& SouthWest);
	CBounds(const double& North, const double& East,const double& South, const double& West);
	//~CBounds(){}

	double getNorthBound() const;

	double getSouthBound() const;

	double getWestBound() const;

	double getEastBound() const;

	void setNorthBound(const double& North);

	void setSouthBound(const double& South);

	void setWestBound(const double& West);

	void setEastBound(const double& East );

	const bool intersects(CBounds& target) const;

	const bool contains(CPosition& target) const;

/*	void addBB(CPosition& addPoint);

	void addBB(CBounds& addBounds);

	void StartBB(CPosition& StartPoint);
*/
private:
	CPosition m_NorthEast;
	CPosition m_SouthWest;
};

#endif /*CBOUNDS_H_*/
