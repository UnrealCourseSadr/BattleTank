// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * Responsible for driving the tank tracks
 */
UCLASS( meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet);

	// Handle tank movement to forward and backward based on the throw (-1 max backward, and +1 max forward)
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void IntendTurnRight(float Throw);
private:
	UTankTrack* LeftTrack{ nullptr };
	UTankTrack* RightTrack{ nullptr };

	// Called by the pathfinding logic
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
};
