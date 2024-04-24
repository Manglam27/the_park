// Fill out your copyright notice in the Description page of Project Settings.


#include "char/FpsAnim.h"

#include "Camera/CameraComponent.h"
#include "char/Myfpschar.h"
#include "Kismet/KismetMathLibrary.h"

void UFpsAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	/*
	Charactor = Cast<AMyfpschar>(TryGetPawnOwner());
	if(Charactor)
	{
		Mesh = Charactor->GetMesh();
		Charactor->CurrentWeaponChangedDelegate.AddDynamic(this,&UFpsAnim::CurrentWeaponChanged);
		CurrentWeaponChanged(Charactor->CurrentWeapon,nullptr);
		
	}
	*/
}
	
void UFpsAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(!Charactor)
	{
		Charactor = Cast<AMyfpschar>(TryGetPawnOwner());
		if(Charactor)
		{
			Mesh = Charactor->GetMesh();
			Charactor->CurrentWeaponChangedDelegate.AddDynamic(this,&UFpsAnim::CurrentWeaponChanged);
			CurrentWeaponChanged(Charactor->CurrentWeapon,nullptr);
			
		}
		else return;
	}

	Setvariables(DeltaTime);
	CalcluateWeaponSway(DeltaTime);

	LastRotation = CameraTransform.Rotator();
}

void UFpsAnim::CurrentWeaponChanged(Aweapon* NewWeapon, const Aweapon* OldWeapon)
{
	Currentweapon = NewWeapon;
	if(Currentweapon)
	{
		Ikprop= Currentweapon->Ikprop;

		GetWorld()->GetTimerManager().SetTimerForNextTick(this,&UFpsAnim::SetIkTransform);
		
	}
	
}


void UFpsAnim::Setvariables(const float DelataTime)
{
	CameraTransform = FTransform(Charactor->GetBaseAimRotation(),Charactor->Camera->GetComponentLocation());

	const FTransform RootOffset = Mesh->GetSocketTransform(FName("root"),RTS_Component).Inverse() * Mesh->GetSocketTransform(FName("ik_hand_root"));
	RelativeCameraTransform = CameraTransform.GetRelativeTransform(RootOffset);

	ADSWeight= Charactor->AdsWight;

	/// Offset

	//  Accumulative Rotation
	const FRotator& AddRotation = CameraTransform.Rotator() - LastRotation;
	constexpr float Angleclamp = 6.f;
	FRotator AddRotarionclamp = FRotator(FMath::ClampAngle(AddRotation.Pitch,-Angleclamp,Angleclamp)*1.5f,
		FMath::ClampAngle(AddRotation.Yaw,-Angleclamp,Angleclamp),0.f);
	AddRotarionclamp.Roll =	AddRotation.Yaw *0.7;

	AccumOffset += AddRotarionclamp;
	AccumOffset = UKismetMathLibrary::RInterpTo(AccumOffset,FRotator::ZeroRotator,DelataTime,30.f);
	AccumRotationInterp = UKismetMathLibrary::RInterpTo(AccumRotationInterp,AccumOffset,DelataTime,5.f);
	
}
void UFpsAnim::CalcluateWeaponSway(const float DeltaTime)
{
	FVector LocationOffset = FVector::ZeroVector;
	FRotator RotationOffset = FRotator::ZeroRotator;

	const FRotator& accumRotationinverse = AccumRotationInterp.GetInverse();
	RotationOffset += accumRotationinverse;
	LocationOffset += FVector(0.f,accumRotationinverse.Yaw,accumRotationinverse.Pitch)/6.f;


	LocationOffset *= Ikprop.WeightScale;
	RotationOffset.Pitch *= Ikprop.WeightScale;
	RotationOffset.Yaw *= Ikprop.WeightScale;
	RotationOffset.Roll *= Ikprop.WeightScale;

	OffsetTransform = FTransform(RotationOffset,LocationOffset);
}


void UFpsAnim::SetIkTransform()
{
	RHandAdsTransform = Currentweapon->GetSightWorldTransform().GetRelativeTransform(Mesh->GetSocketTransform("Weapon_Point"));
}



