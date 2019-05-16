// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, const float LaunchSpeed)
{
	if (!Barrel) { return; }
	FVector LaunchVelocity{};
	FVector StartLocation{ Barrel->GetSocketLocation(FName("Projectile")) };

	bool bHaveAimSolution{ UGameplayStatics::SuggestProjectileVelocity(
		this,
		LaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray<AActor*>(),
		false
	) };
	if (bHaveAimSolution)
	{
		FVector AimDirection{ LaunchVelocity.GetSafeNormal() };
		MoveBarrelTowards(AimDirection);
		
		// If ready then fire
	}
}


void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Calculate the rotation difference between 
	// current barrel rotation and the desired rotation(AimDirection) to launch the projectile
	FRotator BarrelRotator{ Barrel->GetForwardVector().Rotation() };
	FRotator AimAsRotator{ AimDirection.Rotation() };
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	// Move the barrel according to the rotation difference this frame
	Barrel->Elevate(DeltaRotator.Pitch);

	// given a max elevation speed, and the frame time
}