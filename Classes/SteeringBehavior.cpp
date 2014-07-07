#include "SteeringBehavior.h"

Vector2D	SteeringBehavior::Seek(Vector2D TargetPos)
{
	Vector2D DesiredVelocity = (TargetPos - m_pVehicle->Pos()).normalize()
						* m_pVehicle->MaxSpeed();

	return (DesiredVelocity - m_pVehicle->Velocity());
}

Vector2D	SteeringBehavior::Calculate()
{
	m_vSteeringForce = Vector2D(0.0f, 0.0f);
	////use space partitioning to calculate the neighbours of this vehicle
	////if switched on. If not, use the standard tagging system
	//if (!isSpacePartitioningOn())
	//{
	////tag neighbors if any of the following 3 group behaviors are switched on
	//if (On(separation) || On(allignment) || On(cohesion))
	//{
	//	m_pVehicle->World()->TagVehiclesWithinViewRange(m_pVehicle, m_dViewDistance);
	//}
	//}
	//else
	//{
	//	//calculate neighbours in cell-space if any of the following 3 group
	//	//behaviors are switched on
	//	if (On(separation) || On(allignment) || On(cohesion))
	//	{
	//		m_pVehicle->World()->CellSpace()->CalculateNeighbors(m_pVehicle->Pos(), m_dViewDistance);
	//	}
	//}

	switch (m_SummingMethod)
	{
	case weighted_average:
    
	m_vSteeringForce = CalculateWeightedSum(); break;

	case prioritized:

	m_vSteeringForce = CalculatePrioritized(); break;

	case dithered:
    
	m_vSteeringForce = CalculateDithered();break;

	default:m_vSteeringForce = Vector2D(0,0); 

	}//end switch

	return m_vSteeringForce;
}

Vector2D	SteeringBehavior::CalculateWeightedSum()
{        
	//if (On(wall_avoidance))
	//{
	//m_vSteeringForce += WallAvoidance(m_pVehicle->World()->Walls()) *
	//						m_dWeightWallAvoidance;
	//}
 //  
	//if (On(obstacle_avoidance))
	//{
	//m_vSteeringForce += ObstacleAvoidance(m_pVehicle->World()->Obstacles()) * 
	//		m_dWeightObstacleAvoidance;
	//}

	//if (On(evade))
	//{
	//assert(m_pTargetAgent1 && "Evade target not assigned");
 //   
	//m_vSteeringForce += Evade(m_pTargetAgent1) * m_dWeightEvade;
	//}


	////these next three can be combined for flocking behavior (wander is
	////also a good behavior to add into this mix)
	//if (!isSpacePartitioningOn())
	//{
	//if (On(separation))
	//{
	//	m_vSteeringForce += Separation(m_pVehicle->World()->Agents()) * m_dWeightSeparation;
	//}

	//if (On(allignment))
	//{
	//	m_vSteeringForce += Alignment(m_pVehicle->World()->Agents()) * m_dWeightAlignment;
	//}

	//if (On(cohesion))
	//{
	//	m_vSteeringForce += Cohesion(m_pVehicle->World()->Agents()) * m_dWeightCohesion;
	//}
	//}
	//else
	//{
	//if (On(separation))
	//{
	//	m_vSteeringForce += SeparationPlus(m_pVehicle->World()->Agents()) * m_dWeightSeparation;
	//}

	//if (On(allignment))
	//{
	//	m_vSteeringForce += AlignmentPlus(m_pVehicle->World()->Agents()) * m_dWeightAlignment;
	//}

	//if (On(cohesion))
	//{
	//	m_vSteeringForce += CohesionPlus(m_pVehicle->World()->Agents()) * m_dWeightCohesion;
	//}
	//}


	//if (On(wander))
	//{
	//m_vSteeringForce += Wander() * m_dWeightWander;
	//}

	if (On(seek))
	{
	m_vSteeringForce += Seek(m_pVehicle->World()->Crosshair()) * m_dWeightSeek;
	}

	if (On(flee))
	{
	m_vSteeringForce += Flee(m_pVehicle->World()->Crosshair()) * m_dWeightFlee;
	}

	if (On(arrive))
	{
	m_vSteeringForce += Arrive(m_pVehicle->World()->Crosshair(), m_Deceleration) * m_dWeightArrive;
	}

	//if (On(pursuit))
	//{
	//assert(m_pTargetAgent1 && "pursuit target not assigned");

	//m_vSteeringForce += Pursuit(m_pTargetAgent1) * m_dWeightPursuit;
	//}

	//if (On(offset_pursuit))
	//{
	//assert (m_pTargetAgent1 && "pursuit target not assigned");
	//assert (!m_vOffset.isZero() && "No offset assigned");

	//m_vSteeringForce += OffsetPursuit(m_pTargetAgent1, m_vOffset) * m_dWeightOffsetPursuit;
	//}

	//if (On(interpose))
	//{
	//assert (m_pTargetAgent1 && m_pTargetAgent2 && "Interpose agents not assigned");

	//m_vSteeringForce += Interpose(m_pTargetAgent1, m_pTargetAgent2) * m_dWeightInterpose;
	//}

	//if (On(hide))
	//{
	//assert(m_pTargetAgent1 && "Hide target not assigned");

	//m_vSteeringForce += Hide(m_pTargetAgent1, m_pVehicle->World()->Obstacles()) * m_dWeightHide;
	//}

	//if (On(follow_path))
	//{
	//m_vSteeringForce += FollowPath() * m_dWeightFollowPath;
	//}

	//m_vSteeringForce.Truncate(m_pVehicle->MaxForce());
 //
	//return m_vSteeringForce;
}
