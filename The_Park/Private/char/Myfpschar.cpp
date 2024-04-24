// Fill out your copyright notice in the Description page of Project Settings.


#include "The_Park/Public/char/Myfpschar.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Master_Inventory.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "weapons/weapon.h"

// Sets default values
AMyfpschar::AMyfpschar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
		
	GetMesh()->SetTickGroup(ETickingGroup::TG_PostUpdateWork);
	GetMesh()->bVisibleInReflectionCaptures = true;
	GetMesh()->bCastHiddenShadow = true;
	
	
	ClientMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ClientMesh"));
	ClientMesh->SetCastShadow(false);
	ClientMesh->bCastHiddenShadow = false;
	ClientMesh->bVisibleInReflectionCaptures = false;
	ClientMesh->SetTickGroup(ETickingGroup::TG_PostUpdateWork);
	ClientMesh->SetupAttachment(GetMesh());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> bUsePawnControlRotation = true;
	Camera ->SetupAttachment(GetMesh(),FName("head"));


	// Inventory
	
	// Master_Inventory = CreateDefaultSubobject<UMaster_Inventory>("Master Inventory");
	// Master_Inventory->Capacity = 50;
	
}

// Called when the game starts or when spawned
void AMyfpschar::BeginPlay()
{
	Super::BeginPlay();


	//Client Mesh Logic
	if (IsLocallyControlled())
	{
		ClientMesh->HideBoneByName(FName("neck_01"),EPhysBodyOp::PBO_None);
		GetMesh()->SetVisibility(false);
	}
	else
	{
		ClientMesh->DestroyComponent(); 
	}

	//Ads Setup Timeline

	if(AimCurve)
	{
		
	FOnTimelineFloat TimelineFloat;
	TimelineFloat.BindDynamic(this,&AMyfpschar::TimeLineProgres);
		
	AimingTimeline.AddInterpFloat(AimCurve,TimelineFloat);
		
	}

	//Spawing Weapons
	
	if (HasAuthority())
	{
		for(const TSubclassOf<Aweapon>& WeaponClass: Defaultwepons)
		{
			if (!WeaponClass) continue;
			
			FActorSpawnParameters Parameters;
			Parameters.Owner = this;
			Aweapon* Spawnedweapon = GetWorld()->SpawnActor<Aweapon>(WeaponClass,Parameters);
			const int32 Index = Weapons.Add(Spawnedweapon);
			if(Index==CurrentIndex)
			{
				CurrentWeapon = Spawnedweapon;
				OnRep_CurrentWeapon(nullptr);
			}
			
		}
	}
}


void AMyfpschar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AMyfpschar,Weapons,COND_None);
	DOREPLIFETIME_CONDITION(AMyfpschar,CurrentWeapon,COND_None);
	DOREPLIFETIME(AMyfpschar,Healt);
	
	//DOREPLIFETIME_CONDITION(AMyfpschar,AdsWight,COND_None);
}


void AMyfpschar::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	//DOREPLIFETIME_ACTIVE_OVERRIDE( AMyfpschar, AdsWight, AdsWight >= 1.f || AdsWight <=0.f);
	
}


void AMyfpschar::OnRep_CurrentWeapon(const Aweapon* Oldweapon)
{
	if(CurrentWeapon)
	{
		if(!CurrentWeapon->Currentowner)
		{
			const FTransform& PlacementTransform = CurrentWeapon->gunplacment * GetMesh()->GetSocketTransform(FName("Weapon_Point"));
			CurrentWeapon->SetActorTransform(PlacementTransform,false,nullptr,ETeleportType::TeleportPhysics);
			CurrentWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepWorldTransform,FName("Weapon_Point"));
			
			CurrentWeapon->Currentowner = this;
			
		}
		CurrentWeapon->Mesh->SetVisibility(true);	
	}
	if(Oldweapon)
	{
		Oldweapon->Mesh->SetVisibility(false);
	}

	CurrentWeaponChangedDelegate.Broadcast(CurrentWeapon,Oldweapon);
}


// Called to bind functionality to input
void AMyfpschar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("Nextweapon"),EInputEvent::IE_Pressed,this,&AMyfpschar::Nextweapon);
	PlayerInputComponent->BindAction(FName("Lastweapon"),EInputEvent::IE_Pressed,this,&AMyfpschar::Lastweapon);

	PlayerInputComponent->BindAction(FName("Fire"),EInputEvent::IE_Pressed,this,&AMyfpschar::fire);
	PlayerInputComponent->BindAction(FName("Fire"),EInputEvent::IE_Released,this,&AMyfpschar::stopfire);


	PlayerInputComponent->BindAction(FName("Aim"),EInputEvent::IE_Pressed,this,&AMyfpschar::StartAim);
	PlayerInputComponent->BindAction(FName("Aim"),EInputEvent::IE_Released,this,&AMyfpschar::RevAim);

	PlayerInputComponent->BindAxis(FName("Move Forward"),this,&AMyfpschar::moveforward);
	PlayerInputComponent->BindAxis(FName("Move Right"),this,&AMyfpschar::moveright);
	PlayerInputComponent->BindAxis(FName("Look Up"),this,&AMyfpschar::lookup);
	PlayerInputComponent->BindAxis(FName("Look Right"),this,&AMyfpschar::lookright);

}




void AMyfpschar::reload()
{
	if (ammo < MagSize)
	{
		if(CurrentWeapon->ReloadSound)
		{
			UGameplayStatics::PlaySound2D(this,CurrentWeapon->ReloadSound,1);
		}
		ammo = MagSize;
		CurrentWeapon->AmmoinMag = ammo;
	}
}


void AMyfpschar::fire()
{
	if(GetMovementComponent()->IsFalling()) return;
	if(!CurrentWeapon) return;
	if(ammo > 0)
	{
		fireshot();
		GetWorldTimerManager().SetTimer(TimerHandle_Fire,this,&AMyfpschar::fireshot,timebetweenshots,true);
	}
	else
	{
		if(EmptyGunSound)
		{
			UGameplayStatics::PlaySound2D(this,EmptyGunSound,1);
		} 
	}
	
}


void AMyfpschar::stopfire()
{	
	GetWorldTimerManager().ClearTimer(TimerHandle_Fire);
	recoverystart();
	recoilstop();
}

void AMyfpschar::fireshot()
{
	if(ammo<=0) return;
	ammo --;
	CurrentWeapon->AmmoinMag = ammo;
	if(gunshot)
	{
		UGameplayStatics::PlaySound2D(this,gunshot,1);
	}
	
	recoilstart();
	
	linetrace();
	damage();
	
	if(MuzzelParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),MuzzelParticles,CurrentWeapon->Mesh->GetSocketTransform(FName("M")));
	}
	 //error aava che
	FTransform shellspawn = CurrentWeapon->Mesh->GetSocketTransform(FName("Shell"));
	GetWorld()->SpawnActor(CurrentWeapon->Shell,&shellspawn);
	
	
}




void AMyfpschar::recoilstart()
{
	if(CurrentWeapon->Recoilcurve)
	{
		PlayerDeltaRot = FRotator(0.0f, 0.0f, 0.0f);
		RecoilDeltaRot = FRotator(0.0f, 0.0f, 0.0f);
		Del = FRotator(0.0f, 0.0f, 0.0f);
		RecoilStartRot = PCRef->GetControlRotation();//GetControlRotation();
		//->GetControlRotation();

		Firing = true;

		//Timer for the recoil: I have set it to 10s but dependeding how long it takes to empty the gun mag, you can increase the time.

		float a = CurrentWeapon->timebetweenshots * 10;
		if(a==0){a=3;}
		
		
		GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AMyfpschar::recoiltimerfunction,a, false);
			
		bRecoil = true;
		bRecoilRecovery = false;
	}
}

void AMyfpschar::recoiltimerfunction()
{
	bRecoil = false;
	GetWorld()->GetTimerManager().PauseTimer(FireTimer);
}

void AMyfpschar::recoverystart()
{
	if (PCRef->GetControlRotation().Pitch > RecoilStartRot.Pitch || PCRef->GetControlRotation().Pitch < RecoilStartRot.Pitch)
	{
		bRecoilRecovery = true;
		GetWorldTimerManager().SetTimer(RecoveryTimer, this, &AMyfpschar::recoverytimerfunction,RecoveryTime,false);
	}
}

void AMyfpschar::recoverytimerfunction()
{
	bRecoil = false;
	GetWorld()->GetTimerManager().PauseTimer(FireTimer);
}

void AMyfpschar::recoilstop()
{
	Firing = false;
}


void AMyfpschar::recoiltick(float DeltaTime)
{
	float recoiltime;
	FVector RecoilVec;
	if (bRecoil)
	{

		//Calculation of control rotation to update 

		recoiltime = GetWorld()->GetTimerManager().GetTimerElapsed(FireTimer);
		RecoilVec = CurrentWeapon->Recoilcurve->GetVectorValue(recoiltime);
		Del.Roll = 0;
		Del.Pitch = (RecoilVec.Y);
		Del.Yaw = (RecoilVec.Z);
		PlayerDeltaRot = PCRef->GetControlRotation() - RecoilStartRot - RecoilDeltaRot;
		PCRef->SetControlRotation(RecoilStartRot + PlayerDeltaRot + Del);
		RecoilDeltaRot = Del;

		//Conditionally start resetting the recoil

		if (!Firing)
		{
			if (recoiltime > CurrentWeapon->timebetweenshots)
			{
				GetWorld()->GetTimerManager().ClearTimer(FireTimer);
				recoilstop();
				bRecoil = false;
				recoverystart();

			}
		}
	}
	else if (bRecoilRecovery)
	{
		//Recoil resetting
		FRotator tmprot = PCRef->GetControlRotation();
		if (tmprot.Pitch >= RecoilStartRot.Pitch)
		{
			PCRef->SetControlRotation(UKismetMathLibrary::RInterpTo(GetControlRotation(), GetControlRotation() - RecoilDeltaRot, DeltaTime, 10.0f));
			RecoilDeltaRot = RecoilDeltaRot + (GetControlRotation() - tmprot);
		}
		else
		{
			RecoveryTimer.Invalidate();
		}
	}
}






void AMyfpschar::linetrace_Implementation()
{
	
	const float Bulletrange = 200000.f;
	const FVector Starttrace = Camera->GetComponentLocation();
	const FVector startGun = CurrentWeapon->Mesh->GetSocketLocation(FName("M"));
	const FVector Endtrace = ( Camera->GetForwardVector() * Bulletrange )+ Starttrace;
	const FVector EndGun = (Camera->GetForwardVector()*Bulletrange)+startGun;
	
	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(Weapontrace),true,this);
	if(GetWorld()->LineTraceSingleByChannel(GunHitResult,startGun,EndGun,ECC_Visibility,QueryParams))
	{
		//DrawDebugLine(GetWorld(),startGun,EndGun,FColor::Red,true,10.f,0,4.f);
		if(ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles,FTransform(GunHitResult.ImpactNormal.Rotation(),GunHitResult.ImpactPoint));
		}
	}
}

void AMyfpschar::moveforward(const float Value)
{
	const FVector& Direction = FRotationMatrix(FRotator(0.f,GetControlRotation().Yaw,0.f)).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction,Value);
}


void AMyfpschar::moveright(const float Value)
{
	const FVector& Direction = FRotationMatrix(FRotator(0.f,GetControlRotation().Yaw,0.f)).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction,Value);
}


void AMyfpschar::lookup(const float Value)
{
	AddControllerPitchInput(Value);
}


void AMyfpschar::lookright(const float Value)
{
	AddControllerYawInput(Value);
}





void AMyfpschar::Nextweapon()
{
	const int32 Index = Weapons.IsValidIndex(CurrentIndex+1)?CurrentIndex +  1 : 1 ;

	Equipweapon(Index);
}


void AMyfpschar::Lastweapon()
{
	const int32 Index =  Weapons.IsValidIndex(CurrentIndex-1)? (0> CurrentIndex ? CurrentIndex -1 : Weapons.Num() -1 ) : Weapons.Num() - 1 ;
	Equipweapon(Index);
}


void AMyfpschar::Equipweapon(const int32 Index)
{
	if (!Weapons.IsValidIndex(Index) || CurrentWeapon == Weapons[Index]) return;

	if(IsLocallyControlled()|| HasAuthority())
	{
		CurrentIndex = Index;
		const Aweapon* Oldweapon= CurrentWeapon;
		CurrentWeapon = Weapons[Index];
		OnRep_CurrentWeapon(Oldweapon);
	}
	if (!HasAuthority())
	{
		Server_SetCurrntWeapon(Weapons[Index]);
	}
	timebetweenshots = Weapons[Index]->timebetweenshots;
	gunshot = CurrentWeapon->gunshotSound;
	MagSize = CurrentWeapon->MagSize;
	ImpactParticles = CurrentWeapon->ImpactParticles;
	MuzzelParticles = CurrentWeapon->MuzzelParticles;
	EmptyGunSound = CurrentWeapon->EmptyGunSound;
	ammo = CurrentWeapon->AmmoinMag;
}


void AMyfpschar::Server_SetCurrntWeapon_Implementation(Aweapon* NewWeapon)
{
	const Aweapon* Oldweapon= CurrentWeapon;
	CurrentWeapon = NewWeapon;
	OnRep_CurrentWeapon(Oldweapon);
	
}

void AMyfpschar::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	AimingTimeline.TickTimeline(DeltaSeconds);
	
}

void AMyfpschar::TimeLineProgres(const float value)
{
	AdsWight = value;
}





void AMyfpschar::StartAim()
{
	//if(!CurrentWeapon) return;
	if(IsLocallyControlled()||HasAuthority())
	{
		Server_Aim_Implementation(true);
	}
	if(!HasAuthority())
	{
		Server_Aim(true);
	}
}
void AMyfpschar::RevAim()
{
	if(IsLocallyControlled()||HasAuthority())
	{
		Server_Aim_Implementation(false);
	}
	if(!HasAuthority())
	{
		Server_Aim(false);
	}
	
}
void AMyfpschar::MultiAim_Implementation(const bool bForwad)
{
	if(bForwad)
	{
		AimingTimeline.Play();
	}
	else
	{
		AimingTimeline.Reverse();
	}
}


//======================================      Inventory       =================================================
/**
void AMyfpschar::OnuseItem(AItem_Master* Items)
{
	if(Items)
	{
		Items->Use(this);
		//for bp Calling function	
		Items->OnUse(this);
	}
}
**/





