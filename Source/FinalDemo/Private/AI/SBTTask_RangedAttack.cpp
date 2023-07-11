// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

USBTTask_RangedAttack::USBTTask_RangedAttack()
{
	MaxBulletSpread=2.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController=OwnerComp.GetAIOwner();
	if(ensure(MyController))
	{
		ACharacter* MyPawn=Cast<ACharacter>(MyController->GetPawn());
		if(MyPawn==nullptr)
		{
			return EBTNodeResult::Failed;
		}
		
		FVector HandLocation= MyPawn->GetMesh()->GetSocketLocation("ring_01_l");
		
		AActor* TargetActor=Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if(TargetActor==nullptr)
		{
			return EBTNodeResult::Failed;
		}

		if(!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;	
		}
		
		
		FVector Direction=TargetActor->GetActorLocation()-HandLocation;
		FRotator HeadRotation=Direction.Rotation();

		HeadRotation.Pitch+=FMath::RandRange(0.0f,MaxBulletSpread);
		HeadRotation.Yaw+=FMath::RandRange(-MaxBulletSpread,MaxBulletSpread);
		
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Orange,HandLocation.ToString());
		
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Params.Instigator=MyPawn;
		
		AActor* NewProj=GetWorld()->SpawnActor<AActor>(ProjectileClass,HandLocation,HeadRotation,Params);

		return  NewProj ?EBTNodeResult::Succeeded:EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}


