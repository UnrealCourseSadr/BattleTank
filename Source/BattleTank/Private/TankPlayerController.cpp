// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		// Tell controlled tank to aim at this point
		GetControlledTank()->AimAt(HitLocation);
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
	else
	{
		return false;
	}
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
		ECollisionChannel::ECC_Visibility,
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