// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTrack.h"
#include "Components/PrimitiveComponent.h"
#include "SprungWheel.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFucntion)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFucntion);
}

TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<USceneComponent*> TrackComponents{};
	GetChildrenComponents(true, TrackComponents);
	TArray<ASprungWheel*> Wheels{};
	for (USceneComponent* Component : TrackComponents)
	{
		if (Component->IsA<USpawnPoint>())
		{
			AActor* actor{ Cast<USpawnPoint>(Component)->GetSpawnedActor() };
			if (actor && actor->IsA<ASprungWheel>())
			{
				Wheels.Push(Cast<ASprungWheel>(Cast<USpawnPoint>(Component)->GetSpawnedActor()));
			}
			
		}
	}
	return Wheels;
}

void UTankTrack::SetThrottle(float Throttle)
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1.f, +1.f);
	DriveTrack(CurrentThrottle);
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	float ForceApplied{ CurrentThrottle * MaxDrivingForce };
	TArray<ASprungWheel*> Wheels{ GetWheels() };
	float ForcePerWheel{ ForceApplied / Wheels.Num() };
	for (ASprungWheel* Wheel : Wheels)
	{
		Wheel->AddDrivingForce(ForceApplied);
	}
}
