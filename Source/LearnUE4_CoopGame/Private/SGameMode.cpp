// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "Components/SHealthComponent.h"

ASGameMode::ASGameMode()
: timeBetweenWave(2)
{	
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1;
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();
}

void ASGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckWaveState();
}


void ASGameMode::StartWave()
{
	waveCount++;

	numberOfBotsToSpawn = waveCount * 2;

	// SpawnNewBot each 1 second
	GetWorldTimerManager().SetTimer(timerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 0.2, true);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(timerHandle_BotSpawner);
}

void ASGameMode::PrepareForNextwave()
{
	GetWorldTimerManager().SetTimer(timerHandle_PrepareForNextWave, this, &ASGameMode::StartWave, timeBetweenWave);
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();

	numberOfBotsToSpawn--;

	if (numberOfBotsToSpawn <= 0)
	{
		EndWave();
	}
}

void ASGameMode::CheckWaveState()
{
	if (numberOfBotsToSpawn > 0 || GetWorldTimerManager().IsTimerActive(timerHandle_PrepareForNextWave))
	{
		return;
	}

	bool isAnyBotsAlive = false;

	for (auto it = GetWorld()->GetPawnIterator(); it; ++it)
	{
		APawn* pawn = it->Get();

		if (pawn == nullptr || pawn->IsPlayerControlled())
		{
			continue;
		}

		auto healthComp = pawn->FindComponentByClass<USHealthComponent>();
		if (healthComp && healthComp->Gethealth() > 0)
		{
			isAnyBotsAlive = true;
			break;
		}
	}

	if (!isAnyBotsAlive)
	{
		PrepareForNextwave();
	}
}