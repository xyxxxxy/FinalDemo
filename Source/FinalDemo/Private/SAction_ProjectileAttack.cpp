// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_ProjectileAttack.h"

#include "UGamePlayBPLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName="ring_01_l";
	AttackAnimDelay=0.2f;
}


void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character=Cast<ACharacter>(Instigator);
	if(Character)
	{
		//UGameplayStatics::SpawnEmitterAttached();
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"AttackDelay_Elapsed",Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay,Delegate,AttackAnimDelay,false);
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if(ensureAlways(ProjectileClass))
	{
		FVector HandLocation= InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName);
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator=InstigatorCharacter;
		
	
		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		//ignore player
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		AFinalDemoCharacter* Player;
		if(!UUGamePlayBPLibrary::GetPlayer(InstigatorCharacter,Player))
		{
			return;
		}
		UCameraComponent* CameraComp=Cast<UCameraComponent>(Player->GetComponentByClass(UCameraComponent::StaticClass()));
		FVector TraceStart=CameraComp->GetComponentLocation();
		FVector TraceEnd=TraceStart+CameraComp->GetForwardVector()*5000.0f;
		

		FHitResult Hit;
		if(GetWorld()->SweepSingleByObjectType(Hit,TraceStart,TraceEnd,FQuat::Identity,ObjParams,Shape,Params))
		{
			TraceEnd=Hit.ImpactPoint;
		}

		//find new direction from head to impact point
		FRotator ProjRotation=FRotationMatrix::MakeFromX(TraceEnd-HandLocation).Rotator();

		//spawn actor
		FTransform SpawnTM=FTransform(ProjRotation,HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM,SpawnParams);
	}
	StopAction(InstigatorCharacter);
}


