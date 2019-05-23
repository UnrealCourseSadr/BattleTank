// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "Components/PrimitiveComponent.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();

	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFucntion)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFucntion);

	
}

void UTankTrack::ApplySidewaysForce()
{
	// Work-out the required acceleration this frame to correct
	float SlippageSpeed{ FVector::DotProduct(GetRightVector(), GetComponentVelocity()) };
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector FixingAcceleration{ -SlippageSpeed / DeltaTime * GetRightVector() };

	// Calculate and apply sideways (F = m * a)
	UStaticMeshComponent* TankRoot{ Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()) };
	FVector FixingForce{ (TankRoot->GetMass() * FixingAcceleration) / 2 };
	TankRoot->AddForce(FixingForce);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.f;
}

void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1.f, +1.f);
}

void UTankTrack::DriveTrack()
{
	FVector ForceApplied{ GetForwardVector() * CurrentThrottle * MaxDrivingForce };
	FVector ForceLocation{ GetComponentLocation() };
	UPrimitiveComponent* TankRootComponent{ Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()) };
	TankRootComponent->AddForceAtLocation(ForceApplied, ForceLocation, NAME_None);
}
