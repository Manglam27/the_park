// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimInstance.h"
#include "weapons/weapon.h"
#include "FpsAnim.generated.h"


UCLASS()
class THE_PARK_API UFpsAnim : public UAnimInstance
{
	GENERATED_BODY()
public:

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UFUNCTION()
	virtual void CurrentWeaponChanged(class Aweapon* NewWeapon,const class Aweapon* OldWeapon);

	
	virtual void Setvariables(const float DelataTime);
	virtual void CalcluateWeaponSway(const float DeltaTime);

	virtual void SetIkTransform();

public:
	/**
	 *  Refrences of char
	 */
	UPROPERTY(BlueprintReadWrite,Category="Anim")
	class AMyfpschar* Charactor;

	UPROPERTY(BlueprintReadWrite,Category="Anim")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite,Category="Anim")
	class Aweapon* Currentweapon;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	Fikprop Ikprop;


	/**
	 * States
	 */

	UPROPERTY(BlueprintReadWrite,Category="Anim")
	FRotator LastRotation;
	

	///  Ik Variables
	///

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	FTransform CameraTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	FTransform RelativeCameraTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	FTransform RHandAdsTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	float ADSWeight = 0.f;

	///
	///     Offsets
	///

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	FTransform OffsetTransform;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	FRotator AccumOffset ;

	UPROPERTY(BlueprintReadWrite,Category="Anim")
	FRotator AccumRotationInterp;
	
	
};
