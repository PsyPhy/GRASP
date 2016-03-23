/*********************************************************************************/
/*                                                                               */
/*                                    Trackers.cpp                               */
/*                                                                               */
/*********************************************************************************/

// Base class for a 3D/6D Tracker.

#include "stdafx.h"
#include "Trackers.h"

/***************************************************************************/

void Tracker::Initialize( void ) {}
double Tracker::GetSamplePeriod( void ) {
	return( samplePeriod );
}
int  Tracker::Update( void ) {
	return( 0 );
}
void Tracker::Quit() {}

void Tracker::StartAcquisition( float max_duration ) {}
void Tracker::StopAcquisition( void ) {}
bool Tracker::GetAcquisitionState( void ) {
	return( false );
}
bool Tracker::CheckAcquisitionOverrun( void ) {
	return( false );
}

/***************************************************************************/

bool  PointTracker::GetCurrentMarkerFrame( TrackerMarkerFrame &frame ) { 
	return( false );
}

/***************************************************************************/

int  CodaTracker::RetrieveMarkerFrames( TrackerMarkerFrame frames[], int max_frames, int unit ) { 
	return( 0 );
}
bool  CodaTracker::GetCurrentMarkerFrameUnit( TrackerMarkerFrame &frame, int unit ) { 
	// If the tracker has no concept of separate units, just get the data from the default unit.
	return( GetCurrentMarkerFrame( frame ) );
}
int CodaTracker::GetNumberOfCodas( void ) {
	return( 0 );
}

void CodaTracker::GetUnitTransform( int unit, Vector3 &offset, Matrix3x3 &rotation ) {
	fMessageBox( MB_OK, "DexTracker", "GetUnitTransform() undefined." );
}

void CodaTracker::GetUnitPlacement( int unit, Vector3 &pos, Quaternion &ori ) {

	Vector3		offset;
	Matrix3x3	rotation;
	Matrix3x3	ortho;

	GetUnitTransform( unit, offset, rotation );

	// I have to check: Is the position equal to its offset, 
	//  or the negative of the offset?
	// ScaleVector( pos, offset, -1.0 );
	CopyVector( pos, offset );
	// I want to be sure that the rotation matrix is really a rotation matrix.
	// This means that it must be orthonormal.
	// In fact, I am not convinced that CODA guarantees an orthonormal matrix.
	OrthonormalizeMatrix( ortho, rotation );
	// Express the orientation of the CODA unit as a quaternion.
	MatrixToQuaternion( ori, ortho );

}


/**************************************************************************************/

int CodaTracker::PerformAlignment( int origin, int x_negative, int x_positive, int xy_negative, int xy_positive ) {
	return( NORMAL_EXIT );
}

/**************************************************************************************/

void CodaTracker::CopyMarkerFrame( TrackerMarkerFrame &destination, TrackerMarkerFrame &source ) {
	int mrk;
	destination.time = source.time;
	for ( mrk = 0; mrk < nMarkers; mrk++ ) {
		destination.marker[mrk].visibility = source.marker[mrk].visibility;
		CopyVector( destination.marker[mrk].position, source.marker[mrk].position );
	}
}

/**************************************************************************************/

// Get the latest frame of marker data from the specified unit.
// Marker positions are expressed in the intrinsic reference frame of the unit.

bool CodaTracker::GetCurrentMarkerFrameIntrinsic( TrackerMarkerFrame &iframe, int unit ) {

	TrackerMarkerFrame	frame;
	int			status;

	Vector3		offset;
	Matrix3x3	rotation;
	Matrix3x3	inverse;
	Vector3		delta;

	// Retrieve the offset and rotation matrix from the Coda for this unit.
	GetUnitTransform( unit, offset, rotation );
	// Inverse of a rotation matrix is just its transpose.
	TransposeMatrix( inverse, rotation );

	// Get the current frame in aligned coordinates.
	status = GetCurrentMarkerFrameUnit( frame, unit );
//	status = GetCurrentMarkerFrameUnit( frame, unit );
	// I'm not sure what could go wrong, but signal if it does.
	if ( !status ) return( false );
	// Compute the position of each maker in intrinsic coordinates.
	for ( int mrk = 0; mrk < nMarkers; mrk++ ) {
		iframe.marker[mrk].visibility = frame.marker[mrk].visibility;
		if ( frame.marker[mrk].visibility ) {
			SubtractVectors( delta, frame.marker[mrk].position, offset );
			MultiplyVector( iframe.marker[mrk].position, delta, inverse );
		}
	}

	return( true );
}
