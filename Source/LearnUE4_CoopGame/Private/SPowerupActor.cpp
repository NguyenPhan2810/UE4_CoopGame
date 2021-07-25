// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
: powerUpInterval(0)
, totalNumberOfTicks(0)
, ticksCount(0)
{


}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerupActor::OnTickPowerup()
{
	ticksCount++;

	if (ticksCount == 1)
	{
		OnPowerupActivated();
	}

	OnPowerupTicking();


	if (ticksCount >= totalNumberOfTicks)
	{
		// last tick ticked so the OnExpired() is called
		OnPowerupExpired();

		// Delete timer
		GetWorld()->GetTimerManager().ClearTimer(timerHandle_PowerupTick);
	}
}

void ASPowerupActor::ActivatePowerup()
{
	if (powerUpInterval > 0)
		GetWorld()->GetTimerManager().SetTimer(timerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, powerUpInterval, true, 0);
	else
		OnTickPowerup();
}

