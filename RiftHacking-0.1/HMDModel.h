/***********************************************************************
HMDModel - Class to represent 3D models of head-mounted displays for
optical tracking purposes, including LED positions and emission vectors
and the position of the integrated inertial measurement unit.
Copyright (c) 2014 Oliver Kreylos
***********************************************************************/

#ifndef HMDMODEL_INCLUDED
#define HMDMODEL_INCLUDED

#include <Geometry/Point.h>
#include <Geometry/Vector.h>

/* Forward declarations: */
namespace RawHID {
class Device;
}

class HMDModel
	{
	/* Embedded classes: */
	public:
	typedef float Scalar;
	typedef Geometry::Point<Scalar,3> Point;
	typedef Geometry::Vector<Scalar,3> Vector;
	
	struct Marker // Structure to represent a single passive tracking marker or active tracking LED
		{
		/* Elements: */
		public:
		Point pos; // Marker position in HMD coordinate system in meters
		Vector dir; // Direction of optimal marker visibility or LED emission
		};
	
	/* Elements: */
	private:
	Point imu; // Position of the integrated IMU in HMD coordinate system in meters
	unsigned int numMarkers; // Number of tracking markers attached to the HMD
	Marker* markers; // Array of tracking markers attached to the HMD
	
	/* Constructors and destructors: */
	public:
	HMDModel(void); // Creates an empty HMD model
	private:
	HMDModel(const HMDModel& source); // Prohibit copy constructor
	HMDModel& operator=(const HMDModel& source); // Prohibit assignment operator
	public:
	~HMDModel(void); // Destroys the HMD model
	
	/* Methods: */
	void readFromRiftDK2(RawHID::Device& rift); // Reads HMD model from an Oculus Rift DK2 via HID feature reports
	
	const Point& getIMU(void) const // Returns the IMU position
		{
		return imu;
		}
	unsigned int getNumMarkers(void) const // Returns the number of tracking markers
		{
		return numMarkers;
		}
	const Point& getMarkerPos(unsigned int markerIndex) const // Returns the position of the given tracking marker
		{
		return markers[markerIndex].pos;
		}
	const Vector& getMarkerDir(unsigned int markerIndex) const // Returns the direction of the given tracking marker
		{
		return markers[markerIndex].dir;
		}
	};

#endif
