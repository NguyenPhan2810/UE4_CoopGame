// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include <Net/UnrealNetwork.h>

ASGameState::ASGameState()
{

}

void ASGameState::OnRep_WaveState(EWaveState oldState)
{
	WaveStateChanged(oldState, waveState);
}

void ASGameState::SetWaveState(EWaveState newState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		auto oldState = waveState;
		waveState = newState;

		// Manually call this on server
		OnRep_WaveState(oldState);
	}
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASGameState, waveState);
}
