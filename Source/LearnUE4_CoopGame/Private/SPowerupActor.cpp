// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include <Net/UnrealNetwork.h>

// Sets default values
ASPowerupActor::ASPowerupActor()
: powerUpInterval(0)
, totalNumberOfTicks(1) // Initially only tick once
, ticksCount(0)
, isPowerupActive(false)
{
	SetReplicates(true);
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

		// Replicates powerup state to clients
		isPowerupActive = true;

		// Manually call the replication function on server it self
		OnRep_PowerupActive();

		// Delete timer
		GetWorldTimerManager().ClearTimer(timerHandle_PowerupTick);
		return;
	}

	ticksCount++;
	OnPowerupTicking();
}

void ASPowerupActor::ActivatePowerup(AActor* actorActivated)
{
	OnPowerupActivated(actorActivated);

	isPowerupActive = true;
	OnRep_PowerupActive();

	if (powerUpInterval > 0)
		GetWorldTimerManager().SetTimer(timerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, powerUpInterval, true, 0);
	else
		OnTickPowerup();
}

void ASPowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChanged(isPowerupActive);
}


void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, isPowerupActive);
}