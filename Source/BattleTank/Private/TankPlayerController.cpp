// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"


void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		ATank* PlayerTank{ Cast<ATank>(GetPawn()) };
		if (!ensure(PlayerTank)) { return; }
		PlayerTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
	}
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	FoundAimingComponent(TankAimingComponent);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AimTowardsCrosshair();
}



void ATankPlayerController::AimTowardsCrosshair()
{
	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		// Tell aiming component to aim at this point
		TankAimingComponent->AimAt(HitLocation);
	}
}

// Get world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	// Find the crosshair position in pixel coordinates
	int32 ViewportSizeX{}, ViewportSizeY{};
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation{ ViewportSizeX * CrossHairLocationX, ViewportSizeY * CrossHairLocationY };

	// De-project the screen position of the crosshair to a world direction 
	FVector LookDirection{};
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Line-trace along the look direction and see what we hit(up to max range)
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
	return false;
}

// Deproject the screen position to a world direction
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& OutLookDirection) const
{
	FVector CameraWorldLocation{};	// To be discarded
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, OutLookDirection);
}

// Line-traces along the LookDirection by visibility and return the hit location
bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	FHitResult HitResult{};
	FVector StartLocation{ PlayerCameraManager->GetCameraLocation() };
	FVector EndLocation{ StartLocation + LookDirection * MaxAimRange };
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetPawn());

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Camera,
		TraceParams
	))
	{
		OutHitLocation = HitResult.Location;
		return true;
	}
	else
	{
		OutHitLocation = FVector::ZeroVector;
		return false;
	}
}

void ATankPlayerController::OnTankDeath()
{
	StartSpectatingOnly();
}