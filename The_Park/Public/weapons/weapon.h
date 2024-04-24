// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveVector.h"
#include "weapon.generated.h"


USTRUCT(BlueprintType)
struct Fikprop
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UAnimSequence* Animpose;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Aimoffset=15.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FTransform CustomoffsetTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float WeightScale = 1.f;
	
	
};

UCLASS(Abstract)
class THE_PARK_API Aweapon : public AActor
{
	GENERATED_BODY()

public:
	
	Aweapon();

	
protected:
	
	virtual  void BeginPlay() override;
	
	
public:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite,Category="state")
	class AMyfpschar* Currentowner;	

	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration")
	Fikprop Ikprop;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration")
	FTransform gunplacment;

	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="IK")
	FTransform GetSightWorldTransform() const;
	virtual FTransform 	GetSightWorldTransform_Implementation()const { return  Mesh->GetSocketTransform(FName("ADS"));}


	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration")
	FName WeaponName;


	/**
	 *  Empty shell for gun 
	 */
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Component")
	TSubclassOf<AActor> Shell;

	
	/**
	 *  fireing stuff
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float timebetweenshots;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class UParticleSystem* MuzzelParticles;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class UParticleSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class USoundBase* gunshotSound;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class USoundBase* EmptyGunSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class USoundBase* ReloadSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float MagSize;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float AmmoinMag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	FString AmmoName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float ReloadSpeed;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float Damage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	UCurveVector* Recoilcurve;
	
};
