// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

public:	
	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
	virtual void OnPowerupActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
	virtual void OnPowerupTicking();

	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
	virtual void OnPowerupExpired();

	UFUNCTION();
	virtual void OnTickPowerup();

	virtual void ActivatePowerup();

protected: // Member variables

	// Seconds between powerup ticks
	UPROPERTY(EditDefaultsOnly, Category = Powerups)
	float powerUpInterval;

	// Number of times the powerup applies
	UPROPERTY(EditDefaultsOnly, Category = Powerups)
	float totalNumberOfTicks;

	float ticksCount;

	FTimerHandle timerHandle_PowerupTick;
};
