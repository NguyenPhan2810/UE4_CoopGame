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

	UFUNCTION()
	void OnRep_Exploded();

	UFUNCTION()
	void OnRep_ExplosionSequenceStarted();


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// Explode and deal damage
	void Explode();

	// Start explosion sequence when near player
	void StartExplosionSequence();

	void SelfDamage();

	void PlayExplosionEffects();
	void PlayStartExplosionEffects();

protected: // Components
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UStaticMeshComponent* meshComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class USHealthComponent* healthComponent;

	UPROPERTY(VisibleAnywhere, Category = Components)
	class USphereComponent* sphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UAudioComponent* audioComponent;

protected: // Variables with UPROPERTIES
	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	class UParticleSystem* explosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	FName materialParamLastTimeDamageTaken;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	class USoundBase* explosionSound;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	class USoundBase* explosionSequenceSound;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	class USoundBase* rollingSound;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	TSubclassOf<UDamageType> damageType;

	//
	// 
	//

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float requiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float movementForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = TrackerBot)
	float maxSpeed;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	bool bUseAccelerationChange;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float explosionRadius;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float explosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float selfDamageInteral;

	UPROPERTY(EditDefaultsOnly, Category = TrackerBot)
	float selfDamageDamage;

	UPROPERTY(ReplicatedUsing=OnRep_Exploded)
	bool bExploded;

	UPROPERTY(ReplicatedUsing=OnRep_ExplosionSequenceStarted)
	bool bExplosionSequenceStarted;

protected: // Normal variable
	
	// The target point to reach
	FVector currentSegmentEndPoint;

	// The start point of the tracker bot trying to reach the target
	FVector currentSegmentBeginPoint;

	FTimerHandle timerHandle_ExplosionSequence;

	float currentSegmentLength;

	// Material to pulse on damage
	class UMaterialInstanceDynamic* materialInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
