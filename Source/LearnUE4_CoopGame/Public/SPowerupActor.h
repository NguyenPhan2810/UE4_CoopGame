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

public:	
	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
	void OnPowerupActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
	void OnPowerupTicking();

	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
	void OnPowerupExpired();

	// This function will be called both on client and server
	UFUNCTION(BlueprintImplementableEvent, Category = Powerups)
	void OnPowerupStateChanged(bool newState);

	UFUNCTION()
	virtual void OnTickPowerup();

	virtual void ActivatePowerup();

	UFUNCTION()
	void OnRep_PowerupActive();

protected: // Member variables

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
	bool isPowerupActive;

	// Seconds between powerup ticks
	UPROPERTY(EditDefaultsOnly, Category = Powerups)
	float powerUpInterval;

	// Number of times the powerup applies
	UPROPERTY(EditDefaultsOnly, Category = Powerups)
	float totalNumberOfTicks;

	float ticksCount;

	FTimerHandle timerHandle_PowerupTick;
};
