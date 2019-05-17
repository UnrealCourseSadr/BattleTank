// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::Turn(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, +1.f);
	float AzimuthChange{ MaxDegreesPerSecond * RelativeSpeed * GetWorld()->GetDeltaSeconds() };
	float NewAzimuth{ RelativeRotation.Yaw + AzimuthChange };
	SetRelativeRotation(FRotator(0.f, NewAzimuth, 0.f));
	
}