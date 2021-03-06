// Fill out your copyright notice in the Description page of Project Settings.


#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	if (!LeftTrackToSet || !RightTrackToSet) { return; }
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
	// TODO Prevent double-speed due to dual control
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!LeftTrack || !RightTrack) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
	// TODO Prevent double-speed due to dual control
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	FVector TankForward{ GetOwner()->GetActorForwardVector().GetSafeNormal() };
	FVector AIForwardIntention{ MoveVelocity.GetSafeNormal() };
	float ForwardThrow{ FVector::DotProduct(TankForward, AIForwardIntention) };
	IntendMoveForward(ForwardThrow);
	float RightThrow{ FVector::CrossProduct(TankForward, AIForwardIntention).Z };
	IntendTurnRight(RightThrow);
}