// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Tank.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO Move towards the player

	// Aim towards the player
	ATank* ControlledTank{ GetControlledTank() };
	ATank* PlayerTank{ GetPlayerTank() };
	if (!PlayerTank && !ControlledTank) { return; }
	ControlledTank->AimAt(PlayerTank->GetActorLocation());

	// TODO Fire if ready
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) { return nullptr; }
	return Cast<ATank>(PlayerPawn);
}
