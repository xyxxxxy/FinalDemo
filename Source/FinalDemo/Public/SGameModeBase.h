// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USSaveGame;
class ASPowerupActor;
class ASPlayerState;

UCLASS()
class FINALDEMO_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
protected:

	FString SlotName;

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimeHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimeInterval;

	UPROPERTY(EditDefaultsOnly,Category="Credits")
	float CreditPerKill;
	
	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	float RequiredPowerupDistance;

	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	UEnvQuery* PowerupSpawnQuery;

	UPROPERTY(EditDefaultsOnly,Category="Powerups")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	UFUNCTION()
	void SpawnBotTimeElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	
public:
	
	ASGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;
	
	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void OnActorKilled(AActor* VictimActor,AActor* Killer);

	UFUNCTION(Exec)
	void KillAll();

	UFUNCTION(BlueprintCallable,Category="SaveGame")
	void WriteSaveGame();

	UFUNCTION(BlueprintCallable,Category="SaveGame")
	void LoadSaveGame();

	
};
