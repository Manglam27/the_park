// Fill out your copyright notice in the Description page of Project Settings.


#include "The_Park/Public/weapons/weapon.h"

#include "Camera/CameraComponent.h"
#include "char/Myfpschar.h"
#include "kismet/GameplayStatics.h"
#include "char/FpsAnim.h"
#include "Components/ArrowComponent.h"

Aweapon::Aweapon()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	
}

void Aweapon::BeginPlay()
{
	Super::BeginPlay();
	//BindToInput();
	if(!Currentowner)
		Mesh->SetVisibility(false);

	
	
}








