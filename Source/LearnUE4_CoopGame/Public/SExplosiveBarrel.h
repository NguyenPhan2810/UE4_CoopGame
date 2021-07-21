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
	void HandleHealthChanged(USHealthComponent* OwnerHealthComponent, float CurrentHealth, float HealthDelta, const class UDamageType* CauserDamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnRep_Exploded();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ExplosiveBarrel)
	class UParticleSystem* ExplodeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ExplosiveBarrel)
	class USoundBase* ExplodeSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ExplosiveBarrel)
	FVector BoostIntensity;

	UPROPERTY(ReplicatedUsing=OnRep_Exploded)
	bool bExploded;

public:	

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
