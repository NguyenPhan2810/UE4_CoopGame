// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void Respawn();

protected:
	UPROPERTY(VisibleAnywhere, Category = Components)
	class USphereComponent* sphereComponent;
	
	UPROPERTY(VisibleAnywhere, Category = Components)
	class UDecalComponent* decalComponent;

	UPROPERTY(EditInstanceOnly, Category = PickupActor)
	TSubclassOf<class ASPowerupActor> powerupClass;

	UPROPERTY(EditDefaultsOnly, Category = PickupActor)
	FVector spawnLocationOffset;

	class ASPowerupActor* powerupInstance;

	float cooldownDuration;

	FTimerHandle timerHandle_RespawnTimer;
public:	
};
