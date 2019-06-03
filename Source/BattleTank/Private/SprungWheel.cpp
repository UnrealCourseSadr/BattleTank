// Fill out your copyright notice in the Description page of Project Settings.


#include "SprungWheel.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/PrimitiveComponent.h"


// Sets default values
ASprungWheel::ASprungWheel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Axle = CreateDefaultSubobject<USphereComponent>(FName("Axle"));
	Wheel = CreateDefaultSubobject<USphereComponent>(FName("Wheel"));
	MassAxleConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Spring Physics Constraint"));
	AxleWheelConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName("Axle Wheel Constraint"));
	if (!ensure(Wheel && MassAxleConstraint && Axle && AxleWheelConstraint)) { return; }
	SetRootComponent(MassAxleConstraint);
	Axle->SetupAttachment(RootComponent);
	Wheel->SetupAttachment(Axle);
	AxleWheelConstraint->SetupAttachment(Axle);
	
}

// Called when the game starts or when spawned
void ASprungWheel::BeginPlay()
{
	Super::BeginPlay();

	SetupConstraints();
}

void ASprungWheel::SetupConstraints()
{
	if (!GetAttachParentActor()) { return; }
	UPrimitiveComponent* BodyRoot = Cast<UPrimitiveComponent>(GetAttachParentActor()->GetRootComponent());
	if (!BodyRoot) { return; }
	MassAxleConstraint->SetConstrainedComponents(BodyRoot, NAME_None, Axle, NAME_None);

	AxleWheelConstraint->SetConstrainedComponents(Axle, NAME_None, Wheel, NAME_None);
}

// Called every frame
void ASprungWheel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

