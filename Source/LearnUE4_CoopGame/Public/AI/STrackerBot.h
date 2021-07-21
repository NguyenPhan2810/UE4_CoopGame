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

protected: // Self methods
	FVector GetNextPathPoint();

	UFUNCTION()
	void HandleHealthChanged(USHealthComponent* HealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void SelfDestruct();

protected: // Components
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UStaticMeshComponent* meshComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class USHealthComponent* healthComponent;

protected:
	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float requiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float movementForce;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float maxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	bool bUseAccelerationChange;

	// The target point to reach
	FVector currentSegmentEndPoint;

	// The start point of the tracker bot trying to reach the target
	FVector currentSegmentBeginPoint;

	float currentSegmentLength;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	FName materialParamLastTimeDamageTaken;

	// Material to pulse on damage
	class UMaterialInstanceDynamic* materialInstance;

	bool bExploded;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	class UParticleSystem* explosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	class USoundBase* explosionSound;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	TSubclassOf<UDamageType> damageType;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float explosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float explosionDamage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
