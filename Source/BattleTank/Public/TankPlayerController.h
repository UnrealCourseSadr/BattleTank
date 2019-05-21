// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UTankAimingComponent* TankAimingComponent{ nullptr };

	UFUNCTION(BlueprintImplementableEvent, CAtegory = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

private:
	UPROPERTY(EditDefaultsOnly)
	float CrossHairLocationX{ 0.5f };

	UPROPERTY(EditDefaultsOnly)
	float CrossHairLocationY{ 0.33333f };

	UPROPERTY(EditDefaultsOnly)
	float MaxAimRange{ 1000000.f };

	// Start the tank moving the barrel so that a shot would hit where
	// the crosshair intersects with the world
	void AimTowardsCrosshair();

	// Returns true if hit landscape
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& OutLookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const;
};
