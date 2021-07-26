// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"


UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart, // Default state
	
	WaveInProgress,

	WaitingToComplete, // No longer spawning bots, waiting for players to kill remaining bots

	WaveComplete,

	GameOver,
};

/**
 * 
 */
UCLASS()
class LEARNUE4_COOPGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
	

protected:
	UFUNCTION()
	void OnRep_WaveState(EWaveState oldState);

	UFUNCTION(BlueprintImplementableEvent, Category = GameState)
	void WaveStateChanged(EWaveState oldState, EWaveState newState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = GameState)
	EWaveState waveState;
public:

	void SetWaveState(EWaveState newState);
};
