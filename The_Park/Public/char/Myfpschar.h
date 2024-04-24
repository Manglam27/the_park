// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Myfpschar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCurrentWeaponChangedDelegate ,class Aweapon*, CurrentWeapon, const class Aweapon*, Oldweapon);

UCLASS()
class THE_PARK_API AMyfpschar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyfpschar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;\

	virtual  void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	
public:

	////=======================================================================================

	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="PlayerDetails")
	float Healt=10.f;
	
	///=======================================================================================

 
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Components
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Component")
	class USkeletalMeshComponent* ClientMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration")
	class UMaster_Inventory* Master_Inventory;	

	
protected:
	
	// The weapon class spawener by default
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Configuration")
	TArray<TSubclassOf<class Aweapon>> Defaultwepons;
	
public:


	UFUNCTION(BlueprintCallable,Category="Anim")
	virtual  void StartAim ();

	UFUNCTION(BlueprintCallable,Category="Anim")
	virtual  void RevAim();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Anim")
	float AdsWight = 0.f;

	
protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Configuration|Anim")
	class UCurveFloat* AimCurve;
	
	FTimeline AimingTimeline;
	
	UFUNCTION(Server,Reliable)
	void Server_Aim(const bool bForwad = true);
	virtual FORCEINLINE void Server_Aim_Implementation(const bool bForwad = true)
	{
		MultiAim(bForwad);
		MultiAim_Implementation(bForwad);
	}

	UFUNCTION(NetMulticast,Reliable)
	void MultiAim(const bool bForwad = true);
	virtual void MultiAim_Implementation(const bool bForwad = true);

	UFUNCTION()
	virtual void TimeLineProgres(const float value);
	


	
public:

	
	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite,Replicated,Category="State")
	TArray<class Aweapon*>Weapons;

	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite,ReplicatedUsing = OnRep_CurrentWeapon,Category="state")
	class Aweapon* CurrentWeapon;



	//called when current weapon changes
	UPROPERTY(BlueprintAssignable,Category="Delegates")
	FCurrentWeaponChangedDelegate CurrentWeaponChangedDelegate;
	 
	UPROPERTY(VisibleInstanceOnly,BlueprintReadWrite,Category="state")
	int32 CurrentIndex = 0;


	
	UFUNCTION(BlueprintCallable,Category="Character")
	virtual void Equipweapon(const int32 Index);

	
	//update weapons in hand
	

	
protected:

	UFUNCTION()
	virtual void OnRep_CurrentWeapon(const class Aweapon* Oldweapon);

	UFUNCTION(Server,Reliable)
	void Server_SetCurrntWeapon (class Aweapon* Weapon);
	virtual  void Server_SetCurrntWeapon_Implementation(class Aweapon* NewWeapon);


public:
	
	/**
	 *  Firng system
	 */
	UFUNCTION(BlueprintCallable,Category="Fire")
	virtual  void fire();

	UFUNCTION(BlueprintCallable,Category="Fire")
	virtual void stopfire();

	UFUNCTION(BlueprintCallable,Category="Fire")
	virtual void fireshot();

	UFUNCTION(BlueprintCallable,Category="Fire")
	virtual void reload();

	UFUNCTION(BlueprintNativeEvent,Category="Fire")
	void linetrace();

	UFUNCTION(BlueprintImplementableEvent, Category = "Fire")
	void damage();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float timebetweenshots;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class UParticleSystem* MuzzelParticles;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class UParticleSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class USoundBase* gunshot;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	class USoundBase* EmptyGunSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float ammo = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Fire")
	float MagSize;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Fire")
	FHitResult GunHitResult;

	
protected:
	FTimerHandle TimerHandle_Fire;

public:
	
	UPROPERTY()
	bool Firing=false;
	
	UPROPERTY()
	bool bRecoil;

	UPROPERTY()
	bool bRecoilRecovery;

	UPROPERTY()
	FTimerHandle FireTimer;

	UPROPERTY()
	FTimerHandle RecoveryTimer;
	
	UPROPERTY()
	FRotator RecoilStartRot;

	UPROPERTY()
	FRotator RecoilDeltaRot;
	
	UPROPERTY()
	FRotator PlayerDeltaRot;

	UPROPERTY()
	FRotator Del;

	UFUNCTION(BlueprintCallable)
	void recoiltick(float DeltaTime);

	UPROPERTY(BlueprintReadWrite)
	APlayerController* PCRef;

	UPROPERTY(BlueprintReadWrite)
	float RecoveryTime = 1.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float RecoverySpeed =10.0f;
	
protected:
	
	void recoilstart();
	void recoilstop();
	void recoiltimerfunction();
	
	void recoverytimerfunction();
	void recoverystart();
	
	

public:

/// Inventory testing
///

	
protected:
	

	virtual void Nextweapon();
	virtual void Lastweapon();


	//basic movement code
	
	void moveforward(const float Value);
	void moveright(const float Value);
	void lookup(const float Value);
	void lookright(const float Value);
	
};
