// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"


USAttributeComponent::USAttributeComponent()
{
	

	HealthMax=100;
	Health=HealthMax;

	SetIsReplicatedByDefault(true);
}


bool USAttributeComponent::IsAlive() const
{
	return Health>0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health==HealthMax;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor,-GetHealthMax());
}



bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if(!GetOwner()->CanBeDamaged() && Delta<0.0f)
	{
		return false;
	}
	float OldHealth=Health;
	Health=FMath::Clamp(Health+Delta,0.0f,HealthMax);

	float ActualDelta=Health-OldHealth;
	//OnHealthChanged.Broadcast(InstigatorActor,this,Health,ActualDelta);

	if(ActualDelta!=0)
	{
		MulticastHealthChanged(InstigatorActor,Health,ActualDelta);
	}
	

	// died
	if(ActualDelta<0.0f && Health==0.0f)
	{
		ASGameModeBase* GM =GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if(GM)
		{
			GM->OnActorKilled(GetOwner(),InstigatorActor);
		}
	}
	
	return  ActualDelta!=0;
}


USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if(FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}


bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp=GetAttributes(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return  false;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this,NewHealth,Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent,Health);
	DOREPLIFETIME(USAttributeComponent,HealthMax);

	//DOREPLIFETIME_CONDITION(USAttributeComponent,HealthMax,COND_OwnerOnly);
}
