#include "CBounds.h"

std::ostream &operator<< (std::ostream &ostr, const CBounds& Bounds) {
#ifdef QNX
	DEBUG_PRINT("no << operator for CBounds");
#else
	ostr <<  "North=" << Bounds.getNorthBound() << " East=" << Bounds.getEastBound();
	ostr << " South=" << Bounds.getSouthBound() << " West=" << Bounds.getWestBound();
#endif
	return ostr;
}

CBounds::CBounds()
{
	m_NorthEast.setLatitude(0.0);
	m_NorthEast.setLongitude(0.0);
	m_SouthWest.setLatitude(0.0);
	m_SouthWest.setLongitude(0.0);
}

CBounds::CBounds(const CPosition& NorthEast,const CPosition& SouthWest)
{
	m_NorthEast.setLatitude(NorthEast.getLatitude());
	m_NorthEast.setLongitude(NorthEast.getLongitude());
	m_SouthWest.setLatitude(SouthWest.getLatitude());
	m_SouthWest.setLongitude(SouthWest.getLongitude());
}

CBounds::CBounds(const double& North, const double& East,const double& South, const double& West)
{
	m_NorthEast.setLatitude(North);
	m_NorthEast.setLongitude(East);
	m_SouthWest.setLatitude(South);
	m_SouthWest.setLongitude(West);
}


double CBounds::getNorthBound() const
{
	double debug;
	debug = m_NorthEast.getLatitude();
	return debug;
}

double CBounds::getSouthBound() const
{
	double debug;
	debug = m_SouthWest.getLatitude();
	return debug;
}

double CBounds::getWestBound() const
{
	double debug;
	debug = m_SouthWest.getLongitude();
	return debug;
}

double CBounds::getEastBound() const
{
	double debug;
	debug = m_NorthEast.getLongitude();
	return debug;
}

void CBounds::setNorthBound(const double& North)
{
	m_NorthEast.setLatitude(North);
}

void CBounds::setSouthBound(const double& South)
{
	m_SouthWest.setLatitude(South);
}

void CBounds::setWestBound(const double& West)
{
	m_SouthWest.setLongitude(West);
}

void CBounds::setEastBound(const double& East )
{
	m_NorthEast.setLongitude(East);
}


const bool CBounds::intersects(CBounds& target) const
{
	if(

	((getNorthBound()>target.getNorthBound()&&getSouthBound()<target.getNorthBound()) ||
	(getNorthBound()>target.getSouthBound()&&getSouthBound()<target.getSouthBound()) ||
	(getNorthBound()<target.getNorthBound()&&getSouthBound()>target.getSouthBound()))     &&

	((getEastBound()>target.getEastBound()&&getWestBound()<target.getEastBound()) ||
	(getEastBound()>target.getWestBound()&&getWestBound()<target.getWestBound()) ||
	(getEastBound()<target.getEastBound()&&getWestBound()>target.getWestBound()))

	)return true;

	return false;
}

const bool CBounds::contains(CPosition& target) const
{
	if(target.getLatitude()>= getSouthBound() && target.getLatitude()<=getNorthBound()
	&& target.getLongitude()>=getWestBound() && target.getLongitude()<=getEastBound())
	{
		return true;
	}

	return false;
}
/*

void CBounds::addBB(CPosition& addPoint)
{
	if(addPoint.getLatitude()>m_NorthEast.getLatitude())
		m_NorthEast.setLatitude(addPoint.getLatitude());
	if(addPoint.getLatitude()<m_SouthWest.getLatitude())
		m_SouthWest.setLatitude(addPoint.getLatitude());
	if(addPoint.getLongitude()>m_NorthEast.getLongitude())
		m_NorthEast.setLongitude(addPoint.getLongitude());
	if(addPoint.getLongitude()<m_SouthWest.getLongitude())
		m_SouthWest.setLongitude(addPoint.getLongitude());
}

void CBounds::addBB(CBounds& addBounds)
{
	if(addBounds.getNorthBound()>m_NorthEast.getLatitude())
		m_NorthEast.setLatitude(addBounds.getNorthBound());

	if(addBounds.getSouthBound()<m_SouthWest.getLatitude())
		m_SouthWest.setLatitude(addBounds.getSouthBound());

	if(addBounds.getEastBound()>m_NorthEast.getLongitude())
		m_NorthEast.setLongitude(addBounds.getEastBound());

	if(addBounds.getWestBound()<m_SouthWest.getLongitude())
		m_SouthWest.setLongitude(addBounds.getWestBound());
}

void CBounds::StartBB(CPosition& StartPoint)
{
	m_NorthEast=StartPoint;
	m_SouthWest=StartPoint;
}
*/
