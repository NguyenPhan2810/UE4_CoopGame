// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	FVector GetNextPathPoint();

protected:
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UStaticMeshComponent* meshComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float requiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float movementForce;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	bool bUseAccelerationChange;

	// The target point to reach
	FVector currentSegmentEndPoint;

	// The start point of the tracker bot trying to reach the target
	FVector currentSegmentBeginPoint;

	float currentSegmentLength;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
