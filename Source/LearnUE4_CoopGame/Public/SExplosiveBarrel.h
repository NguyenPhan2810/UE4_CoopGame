// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

UCLASS()
class LEARNUE4_COOPGAME_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleHealthChanged(USHealthComponent* OwnerHealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void PlayEffect();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	class USHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	class URadialForceComponent* RadialForceComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ExplosiveBarrel)
	class UMaterialInterface* MaterialNormal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ExplosiveBarrel)
	class UMaterialInterface* MaterialExploded;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExplosiveBarrel)
	class UParticleSystem* ExplodeEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExplosiveBarrel)
	FVector BoostIntensity;
public:	

};
