// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"


ASPowerupActor::ASPowerupActor()
{

	SphereComp=CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Powerup");
	RootComponent=SphereComp;

	MeshComp=CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetupAttachment(RootComponent);

	RespawnTime=10.0f;

	SetReplicates(true);
	
}

void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTime,this,&ASPowerupActor::ShowPowerup,RespawnTime);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	//set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive,true);
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	//
}







