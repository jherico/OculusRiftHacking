/***********************************************************************
HMDModel - Class to represent 3D models of head-mounted displays for
optical tracking purposes, including LED positions and emission vectors
and the position of the integrated inertial measurement unit.
Copyright (c) 2014 Oliver Kreylos
***********************************************************************/

#include "HMDModel.h"

#include <Misc/ThrowStdErr.h>
#include <RawHID/Device.h>

/*************************
Methods of class HMDModel:
*************************/

HMDModel::HMDModel(void)
	:imu(Point::origin),
	 numMarkers(0),markers(0)
	{
	}

HMDModel::~HMDModel(void)
	{
	delete[] markers;
	}

void HMDModel::readFromRiftDK2(RawHID::Device& rift)
	{
	/* Read a sequence of 0x0fU feature reports to get the position of all LEDs: */
	unsigned int firstReportIndex=0;
	unsigned int numReports=0;
	unsigned int markerIndex=0;
	while(true)
		{
		/* Read the next LED feature report: */
		RawHID::Device::Byte buffer[30];
		buffer[0]=0x0fU;
		size_t reportSize=rift.readFeatureReport(buffer,sizeof(buffer));
		if(reportSize!=sizeof(buffer))
			Misc::throwStdErr("HMDModel::readFromRiftDK2: Received LED feature report of %u bytes instead of %u bytes",(unsigned int)reportSize,(unsigned int)sizeof(buffer));
		
		/* Extract the report index and total number of reports: */
		unsigned int ri=buffer[24];
		unsigned int nr=buffer[26];
		
		if(numReports==0) // Is this the first received report?
			{
			numReports=nr;
			firstReportIndex=ri;
			
			/* Allocate the marker array: */
			numMarkers=numReports-1; // One of the reports defines the IMU's position
			markers=new Marker[numMarkers];
			}
		else if(ri==firstReportIndex) // Is this the first received report again?
			break;
		
		/* Helper structure to convert two little-endian bytes to a 16-bit signed integer: */
		union
			{
			Misc::UInt8 b[2];
			Misc::SInt16 i;
			} convert16;
		
		/* Helper structure to convert four little-endian bytes to a 32-bit signed integer: */
		union
			{
			Misc::UInt8 b[4];
			Misc::SInt32 i;
			} convert32;
		
		/* Extract the LED position: */
		Point ledPos;
		RawHID::Device::Byte* bufPtr=buffer+4;
		for(int i=0;i<3;++i)
			{
			/* Convert four little-endian bytes to a 32-bit signed integer: */
			for(int j=0;j<4;++j,++bufPtr)
				convert32.b[j]=*bufPtr;
			ledPos[i]=Scalar(convert32.i)*1.0e-6; // Convert from micrometers to meters
			}
		
		/* Check if this is a LED or the IMU: */
		if(buffer[3]==0x02U)
			{
			/* It's an LED: */
			markers[markerIndex].pos=ledPos;
			
			/* Extract the LED's direction vector: */
			for(int i=0;i<3;++i)
				{
				for(int j=0;j<2;++j,++bufPtr)
					convert16.b[j]=*bufPtr;
				markers[markerIndex].dir[i]=Scalar(convert16.i);
				}
			
			/* Normalize the direction vector: */
			markers[markerIndex].dir.normalize();
			
			++markerIndex;
			}
		else
			{
			/* It's the IMU: */
			imu=ledPos;
			} 
		}
	}
