// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

bool UTankAimingComponent::IsBarrelMoving() const
{
	if (!ensure(Barrel)) { return false; }
	FVector BarrelForwardVector{ Barrel->GetForwardVector() };
	return !BarrelForwardVector.Equals(AimDirection, 0.01f);
}

UTankBarrel* UTankAimingComponent::GetBarrel() const
{
	return Barrel;
}

UTankTurret* UTankAimingComponent::GetTurret() const
{
	return Turret;
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (FPlatformTime::Seconds() - LastFiringTime < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }
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
		AimDirection = LaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		MoveTurretTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel)) { return; }
	// Calculate the rotation difference between 
	// current barrel rotation and the desired rotation(AimDirection) to launch the projectile
	FRotator BarrelRotator{ Barrel->GetForwardVector().Rotation() };
	FRotator AimAsRotator{ AimDirection.Rotation() };
	FRotator DeltaRotator{ AimAsRotator - BarrelRotator };

	// Move the barrel according to the rotation difference this frame
	// given a max elevation speed, and the frame time
	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::MoveTurretTowards(FVector AimDirection)
{
	if (!ensure(Turret)) { return; }
	// Calculate the rotation difference between 
	// current turret rotation and the desired rotation(AimDirection) to launch the projectile
	FRotator TurretRotator{ Turret->GetForwardVector().Rotation() };
	FRotator AimAsRotator{ AimDirection.Rotation() };
	FRotator DeltaRotator{ AimAsRotator - TurretRotator };

	// Move the turret according to the rotation difference this frame
	// given a max azimuth speed, and the frame time
	FMath::Abs(DeltaRotator.Yaw) < 180.f ? Turret->Turn(DeltaRotator.Yaw) : Turret->Turn(-DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel)) { return; }
	if (!ensure(ProjectileBlueprint)) { return; }
	
	if (FiringState != EFiringState::Reloading)
	{
		// Spawn a projectile at the socket location on the barrel
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile")));

		if (!ensure(Projectile)) { return; }
		Projectile->LaunchProjectile(LaunchSpeed);
		LastFiringTime = FPlatformTime::Seconds();
	}

}