

#pragma once

#include "CoreMinimal.h"
#include "SInteractInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class FINALDEMO_API ASPowerupActor : public AActor,public ISInteractInterface
{
	GENERATED_BODY()
	
public:	

	ASPowerupActor();

protected:
	UPROPERTY(EditAnywhere,Category="Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTime;

	UFUNCTION()
	void ShowPowerup();
	
	void HideAndCooldownPowerup();
	
	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(EditDefaultsOnly,Category="Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly,Category="Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly,Category="Credits")
	float CreditAmount;
	

public:	
	 void Interact_Implementation(APawn* InstigatorPawn) override;
	

};
