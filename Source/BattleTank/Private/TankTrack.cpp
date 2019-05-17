// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"


void UTankTrack::SetThrottle(float Throttle)
{
	// TODO Clamp actual throttle value so that players can't over-drive
	FVector ForceApplied{ GetForwardVector() * Throttle * MaxDrivingForce };
	FVector ForceLocation{ GetComponentLocation() };
	UPrimitiveComponent* TankRootComponent{ Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()) };
	TankRootComponent->AddForceAtLocation(ForceApplied, ForceLocation, NAME_None);
}
