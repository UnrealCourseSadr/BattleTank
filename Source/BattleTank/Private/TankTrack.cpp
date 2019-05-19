// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"


void UTankTrack::SetThrottle(float Throttle)
{
	FVector ForceApplied{ GetForwardVector() * Throttle * MaxDrivingForce };
	FVector ForceLocation{ GetComponentLocation() };
	UPrimitiveComponent* TankRootComponent{ Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()) };
	TankRootComponent->AddForceAtLocation(ForceApplied, ForceLocation, NAME_None);
}
