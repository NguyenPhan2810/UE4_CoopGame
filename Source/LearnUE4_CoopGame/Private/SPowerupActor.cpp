// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
: powerUpInterval(0)
, totalNumberOfTicks(1) // Initially only tick once
, ticksCount(0)
{

}


// Tick at the begin of each interval
// For example: max number of ticks is 2 and tick interval is 0.2 then 
// OnTickPowerup will be called at 0 seconds and 0.2 seconds then
// OnPowerupExpired() will be called at 0.4 seconds
// OnPowerupActivated will be called at 0 seconds 
void ASPowerupActor::OnTickPowerup()
{
	if (ticksCount >= totalNumberOfTicks)
	{
		// last tick ticked so the OnExpired() is called
		OnPowerupExpired();

		// Delete timer
		GetWorldTimerManager().ClearTimer(timerHandle_PowerupTick);
		return;
	}

	ticksCount++;

	if (ticksCount == 1)
	{
		OnPowerupActivated();
	}

	OnPowerupTicking();
}

void ASPowerupActor::ActivatePowerup()
{
	if (powerUpInterval > 0)
		GetWorldTimerManager().SetTimer(timerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, powerUpInterval, true, 0);
	else
		OnTickPowerup();
}

