

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged,AActor*,InstigatorActor,
	class USAttributeComponent*,OwningComp,float,NewHealth,float,Delta);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINALDEMO_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category="Attributes")
	static USAttributeComponent* GetAttributes(AActor* FromActor);

	UFUNCTION(BlueprintCallable,Category="Attributes",meta=(DisplayName="IsAlive"))
	static bool IsActorAlive(AActor* Actor);
	
	USAttributeComponent();


protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Replicated,Category="Attributes")
	float Health;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Replicated,Category="Attributes")
	float HealthMax;

	UFUNCTION(NetMulticast,Reliable)
	void MulticastHealthChanged(AActor* InstigatorActor,float NewHealth,float Delta);
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintCallable,Category="Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor,float Delta);

	UFUNCTION(BlueprintPure,BlueprintCallable)
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable)
	bool IsFullHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthMax() const;

	UFUNCTION(BlueprintCallable)
	bool Kill(AActor* InstigatorActor);

	
		
};
