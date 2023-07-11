// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"

#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SActionComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "SWorldUserWidget.h"



ASAICharacter::ASAICharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp=CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp=CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp=CreateDefaultSubobject<USActionComponent>("ActionComp");
	
	AutoPossessAI=EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	TimeToHitParamName="TimeToHit";
	
}



void ASAICharacter::PostInitializeComponents()
{

	Super::PostInitializeComponents();
	PawnSensingComp->OnSeePawn.AddDynamic(this,&ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this,&ASAICharacter::AIOnHealthChanged);
}



void ASAICharacter::AIOnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float Delta)
{
	if(Delta<0.0f)
	{
		if(InstigatorActor!=this)
		{
			SetTargetActor(InstigatorActor);
		}

		// UI
		if(ActiveHealthBar==nullptr)
		{
			ActiveHealthBar=CreateWidget<USWorldUserWidget>(GetWorld(),HealthBarWidgetClass);
			if(ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor=this;
				ActiveHealthBar->AddToViewport();
			}
		}
		
		// hit flash
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName,GetWorld()->TimeSeconds);

		//died
		if(NewHealth<=0.0f)
		{
			// stop BT
			AAIController* AIC=Cast<AAIController>(GetController());
			if(AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			//rag doll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();

			//life span
			SetLifeSpan(10.0f);
		}
	}
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AIC=Cast<AAIController>(GetController());
	if(AIC)
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor",NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
}


