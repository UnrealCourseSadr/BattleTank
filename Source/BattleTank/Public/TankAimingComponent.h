// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

UENUM()
enum class EFiringState : uint8
{
	Locked,
	Aiming,
	Reloading,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	UTankBarrel* GetBarrel() const { return Barrel; }
	UTankTurret* GetTurret() const { return Turret; }

	UFUNCTION(BlueprintCallable, Category = "Aiming")
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRoundsLeft() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed{ 4000.f };

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds{ 3.f };

	double LastFiringTime{ 0. };

	UTankBarrel* Barrel{ nullptr };
	UTankTurret* Turret{ nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint{ nullptr };

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState{ EFiringState::Reloading };

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 RoundsLeft{ 10 };

private:
	FVector AimDirection{ 0.f };

	void MoveBarrelTowards(FVector AimDirection);
	void MoveTurretTowards(FVector AimDirection);

	bool IsBarrelMoving() const;
};
