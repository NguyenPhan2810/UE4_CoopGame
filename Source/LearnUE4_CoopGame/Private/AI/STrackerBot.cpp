// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include <Components/StaticMeshComponent.h>
#include <NavigationSystem.h>
#include <Kismet/GameplayStatics.h>
#include <NavigationSystem.h>
#include <NavigationPath.h>

// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	meshComponent->SetCanEverAffectNavigation(false);
	meshComponent->SetSimulatePhysics(true);
	bUseAccelerationChange = false;
	requiredDistanceToTarget = 100;
	movementForce = 1000;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ASTrackerBot::GetNextPathPoint()
{
	auto player = UGameplayStatics::GetPlayerPawn(this, 0);

	// Find path
	auto path = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), player);
	
	// Return next point excluding it self
	if (path->PathPoints.Num() > 0)
	{
		return path->PathPoints[1];
	}


	// Failed to get next point
	return GetActorLocation();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto distanceToTarget = (nextPathPoint - GetActorLocation()).Size();

	if (distanceToTarget < requiredDistanceToTarget)
	{
		// Find new target
		nextPathPoint = GetNextPathPoint();
	}
	else
	{
		// Keep moving to target
		FVector forceDirection = nextPathPoint - GetActorLocation();
		forceDirection.Normalize();

		meshComponent->AddForce(forceDirection * movementForce, NAME_None, bUseAccelerationChange);
	}
}