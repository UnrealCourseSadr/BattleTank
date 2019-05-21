// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFucntion)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFucntion);

	float SlippageSpeed{ FVector::DotProduct(GetRightVector(), GetComponentVelocity()) };

	// Work-out the required acceleration this frame to correct
	FVector FixingAcceleration{ -SlippageSpeed / DeltaTime * GetRightVector() };

	// Calculate and apply sideways (F = m * a)
	UStaticMeshComponent* TankRoot{ Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()) };
	FVector FixingForce{ (TankRoot->GetMass() * FixingAcceleration) / 2 };
	TankRoot->AddForce(FixingForce);
}
void UTankTrack::SetThrottle(float Throttle)
{
	FVector ForceApplied{ GetForwardVector() * Throttle * MaxDrivingForce };
	FVector ForceLocation{ GetComponentLocation() };
	UPrimitiveComponent* TankRootComponent{ Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()) };
	TankRootComponent->AddForceAtLocation(ForceApplied, ForceLocation, NAME_None);
}
