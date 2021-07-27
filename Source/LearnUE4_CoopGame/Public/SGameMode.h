// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, victimActor, AActor*, killerActor, AController*, killerController);

/**
 * 
 */
UCLASS()
class LEARNUE4_COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode();

protected:
	void StartWave();
	void EndWave();
	void PrepareForNextwave();

	UFUNCTION(BlueprintImplementableEvent, Category = GameMode)
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	void CheckWaveState();
	void CheckAnyPlayerAlive();
	void GameOver();
	void RespawnDeadPlayers();
	void SetWaveState(EWaveState newState);

public:
	virtual void StartPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	FTimerHandle timerHandle_BotSpawner;
	FTimerHandle timerHandle_PrepareForNextWave;

	int32 numberOfBotsToSpawn;

	UPROPERTY(BlueprintReadOnly, Category = GameMode)
	int32 waveCount;

	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	float timeBetweenWave;

public:
	UPROPERTY(BlueprintAssignable, Category = GameMode)
	FOnActorKilled OnActorKilled;
};
