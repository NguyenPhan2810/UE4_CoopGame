// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "Components/SHealthComponent.h"


#include <Components/StaticMeshComponent.h>
#include <NavigationSystem.h>
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem.h>
#include <NavigationPath.h>
#include <DrawDebugHelpers.h>

// Sets default values
ASTrackerBot::ASTrackerBot()
: materialInstance(nullptr)
, bUseAccelerationChange(false)
, requiredDistanceToTarget(50)
, maxSpeed(500)
, movementForce(4000)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	meshComponent->SetCanEverAffectNavigation(false);
	meshComponent->SetSimulatePhysics(true);

	healthComponent = CreateDefaultSubobject<USHealthComponent>("HealthComponent");
	healthComponent->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleHealthChanged);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
	currentSegmentEndPoint = GetActorLocation();
	currentSegmentBeginPoint = GetActorLocation();
	currentSegmentLength = 0;
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto distanceToTarget = (currentSegmentEndPoint - GetActorLocation()).Size();

	if (distanceToTarget < requiredDistanceToTarget)
	{
		// Find new target
		currentSegmentEndPoint = GetNextPathPoint();
		currentSegmentBeginPoint = GetActorLocation();
		currentSegmentLength = (currentSegmentEndPoint - currentSegmentBeginPoint).Size();
	}
	else
	{
		// Keep moving to target
		FVector forceDirection = currentSegmentEndPoint - GetActorLocation();
		forceDirection.Normalize();
		meshComponent->AddForce(forceDirection * movementForce, NAME_None, bUseAccelerationChange);

		if (GetVelocity().Size() > maxSpeed)
		{
			auto velDirection = GetVelocity();
			velDirection.Normalize();
			meshComponent->SetAllPhysicsLinearVelocity(velDirection * maxSpeed);
		}
	}
}

FVector ASTrackerBot::GetNextPathPoint()
{
	auto player = UGameplayStatics::GetPlayerPawn(this, 0);

	if (player)
	{
		// Find path
		auto path = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), player);

		// Return next point excluding it self
		if (path->PathPoints.Num() > 0)
		{
			return path->PathPoints[1];
		}
	}

	// Failed to get next point
	return GetActorLocation();
}

void ASTrackerBot::HandleHealthChanged(USHealthComponent* HealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (CurrentHealth <= 0)
	{
		Destroy();
	}

	if (materialInstance == nullptr)
		materialInstance = meshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, meshComponent->GetMaterial(0));

	if (materialInstance)
		materialInstance->SetScalarParameterValue(materialParamLastTimeDamageTaken, GetWorld()->TimeSeconds);
}