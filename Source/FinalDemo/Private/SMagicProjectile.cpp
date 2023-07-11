// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SActionComponent.h"
#include "UGamePlayBPLibrary.h"
#include "FinalDemo/FinalDemoCharacter.h"
#include "Particles/ParticleSystemComponent.h"

#include"SActionEffect.h"


ASMagicProjectile::ASMagicProjectile()
{

	PrimaryActorTick.bCanEverTick = true;

	SphereComp=CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ASMagicProjectile::OnActorOverlap);
	RootComponent=SphereComp;

	EffectComp=CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp=CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed=2000.0f;
	MovementComp->bRotationFollowsVelocity=true;
	MovementComp->bInitialVelocityInLocalSpace=true;

	DamageAmount=20.0f;

	SetReplicates(true);
}



void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor&& OtherActor!=GetInstigator())
	{

		USActionComponent* ActionComp=Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));

		if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity*=-1;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		// apply damage and destroy
		if(UUGamePlayBPLibrary::ApplyDirectionDamage(GetInstigator(),OtherActor,DamageAmount,SweepResult))
		{
			Destroy();

			ActionComp->AddAction(GetInstigator(),BurningActionClass);
		}
	}
}


