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
	
	currentSegmentEndPoint = GetActorLocation();
	currentSegmentBeginPoint = GetActorLocation();
	currentSegmentLength = 0;
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


		FVector calculatedForce = forceDirection * movementForce * (distanceToTarget / currentSegmentLength);

		

		//The nearer the distance the smaller the force
		meshComponent->AddForce(calculatedForce, NAME_None, bUseAccelerationChange);
	}
}