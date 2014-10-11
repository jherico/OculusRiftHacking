#include <iostream>
#include <RawHID/BusType.h>
#include <RawHID/Device.h>

#include "HMDModel.h"

/****************
Helper functions:
****************/

std::ostream& operator<<(std::ostream& os,const Geometry::ComponentArray<HMDModel::Scalar,3>& point)
	{
	os<<'(';
	for(int i=0;i<3;++i)
		{
		if(i>0)
			os<<", ";
		os<<point[i];
		}
	os<<')';
	
	return os;
	}

/*************************************************
Helper class to enumerate Oculus Rift DK2 devices:
*************************************************/

class OculusRiftDK2Matcher:public RawHID::Device::DeviceMatcher
	{
	/* Methods from RawHID::Device::DeviceMatcher: */
	public:
	virtual bool operator()(int busType,unsigned short vendorId,unsigned short productId) const
		{
		return busType==RawHID::BUSTYPE_USB&&vendorId==0x2833U&&productId==0x0021U;
		}
	};

int main(void)
	{
	/* Open the first Oculus Rift DK2: */
	RawHID::Device rift(OculusRiftDK2Matcher(),0);
	
	/* Initialize the HMD model: */
	HMDModel hmdModel;
	hmdModel.readFromRiftDK2(rift);
	
	/* Print the IMU position: */
	std::cout<<"IMU position: "<<hmdModel.getIMU()<<std::endl;
	
	/* Print all LED positions and directions: */
	for(unsigned int i=0;i<hmdModel.getNumMarkers();++i)
		std::cout<<"LED "<<i<<": "<<hmdModel.getMarkerPos(i)<<", "<<hmdModel.getMarkerDir(i)<<std::endl;
	
	return 0;
	}
