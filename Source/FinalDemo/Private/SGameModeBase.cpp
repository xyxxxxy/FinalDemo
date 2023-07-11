

#include "SGameModeBase.h"

#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "SInteractInterface.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "FinalDemo/FinalDemoCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


// can spawn bots
static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"),
	false,TEXT("Enable Spawn."),ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimeInterval=2.0f;

	CreditPerKill=20;

	DesiredPowerupCount=10;
	
	RequiredPowerupDistance=2000;

	PlayerStateClass=ASPlayerState::StaticClass();

	SlotName="SaveName01";
}


void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}


void ASGameModeBase::StartPlay()
{

	Super::StartPlay();

	// Spawn Bots
	GetWorldTimerManager().SetTimer(TimeHandle_SpawnBots,this,&ASGameModeBase::SpawnBotTimeElapsed,
		SpawnTimeInterval,true);

	// Spawn Powerup
	if(ensure(PowerupClasses.Num()>0))
	{
		//Run EQS to find potential power_up spawn locations
		UEnvQueryInstanceBlueprintWrapper* QueryInstance=UEnvQueryManager::RunEQSQuery(this,PowerupSpawnQuery,this,
			EEnvQueryRunMode::AllMatching,nullptr);

		if(ensure(QueryInstance))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnPowerupSpawnQueryCompleted);
		}
	}
}


void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* PS=NewPlayer->GetPlayerState<ASPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}


void ASGameModeBase::SpawnBotTimeElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp,Warning,TEXT("Bots Spawn disabled via cvar 'CVarSpawnBots'."))
		return;
	}
	
	int32 NrOfAliveBots=0;
	for( TActorIterator<ASAICharacter> It(GetWorld());It;++It)
	{
		ASAICharacter* Bot=*It;
		USAttributeComponent* AttributeComp=USAttributeComponent::GetAttributes(Bot);
		
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBotCount=10.0f;
	
	if(DifficultyCurve)
	{
		MaxBotCount=DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if(NrOfAliveBots>=MaxBotCount)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance= UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,
		this,EEnvQueryRunMode::RandomBest5Pct,nullptr);
	if(ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
	}
	
}


void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus!=EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn not EQS is Faild!"));
		return;
	}

	//spawn AI
	TArray<FVector> Locations=QueryInstance->GetResultsAsLocations();

	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0],FRotator::ZeroRotator);
	}
	
}





// re alive
void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();
		GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Red,TEXT("Died"));
		RestartPlayer(Controller);
	}
}


void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus!=EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("Spawn powerup EQS faild!"));
	}

	TArray<FVector> Locations=QueryInstance->GetResultsAsLocations();

	// keep locations
	TArray<FVector> UsedLocations;

	int32 SpawnCounter=0;

	//break out if we reach the desired count or if we have no more potential positions remaining
	while (SpawnCounter<DesiredPowerupCount && Locations.Num()>0)
	{
		//pick a random location
		int32 RandomLocationIndex=FMath::RandRange(0,Locations.Num()-1);

		FVector PickedLocation=Locations[RandomLocationIndex];
		//remove
		Locations.RemoveAt(RandomLocationIndex);

		//check min distance
		bool bValidLocation=true;
		for(FVector OtherLocation:UsedLocations)
		{
			float DistanceTo=(PickedLocation-OtherLocation).Size();

			if(DistanceTo<RequiredPowerupDistance)
			{
				bValidLocation=false;
				break;
			}
		}
		if(!bValidLocation)
		{
			continue;
		}

		// pick a random powerup
		int32 RandomClassIndex=FMath::RandRange(0,PowerupClasses.Num()-1);
		TSubclassOf<AActor> RandomPowerupClass=PowerupClasses[RandomClassIndex];

		GetWorld()->SpawnActor<AActor>(RandomPowerupClass,PickedLocation,FRotator::ZeroRotator);

		// keep for distance checks
		UsedLocations.Add(PickedLocation);
		SpawnCounter++;
		
	}

}


// exec kill all AI
void ASGameModeBase::KillAll()
{
	for( TActorIterator<ASAICharacter> It(GetWorld());It;++It)
	{
		ASAICharacter* Bot=*It;
		USAttributeComponent* AttributeComp=USAttributeComponent::GetAttributes(Bot);
		if(ensure(AttributeComp) && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}



// be killed
void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AFinalDemoCharacter* Player=Cast<AFinalDemoCharacter>(VictimActor);

	// Respawn Player
	if(Player)
	{
		FTimerHandle TimeHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this,"RespawnPlayerElapsed",Player->GetController());

		float RespawnDelay=2.0f;

		GetWorldTimerManager().SetTimer(TimeHandle_RespawnDelay,Delegate,RespawnDelay,false);
	}

	// Give Credits for kill
	APawn* KillerPawn=Cast<APawn>(Killer);
	if(KillerPawn)
	{
		ASPlayerState* PS=KillerPawn->GetPlayerState<ASPlayerState>();
		if(PS)
		{
			PS->AddCredits(CreditPerKill);
		}
	}
}



//save game
void ASGameModeBase::WriteSaveGame()
{
	//save player state
	for(int32 i=0;i<GameState->PlayerArray.Num();++i)
	{
		ASPlayerState* PS=Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	//clear
	CurrentSaveGame->SavedActors.Empty();
	// save actor
	for(FActorIterator It(GetWorld());It;++It)
	{
		AActor* Actor=*It;

		
		if(!Actor->Implements<USInteractInterface>())
		{
			continue;
		}
		
		FActorSaveData ActorData;
		ActorData.ActorName=Actor->GetName();
		//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,ActorData.ActorName);
		ActorData.Transform=Actor->GetActorTransform();
		
		//pass the array to fill with data from actor
		FMemoryWriter MemWriter(ActorData.ByteData);
		
		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);
		//find only var with property(SaveGame)
		Ar.ArIsSaveGame=true;
		//convert actor's SaveGame UPROPERTIES into binary array
		Actor->Serialize(Ar);
		
		
		CurrentSaveGame->SavedActors.Add(ActorData);
	}
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}


//load game
void ASGameModeBase::LoadSaveGame()
{
	if(UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame=Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if(CurrentSaveGame==nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Failed to load SameGame data!"));
			return;
		}
		UE_LOG(LogTemp,Warning,TEXT("Loaded SameGame data!"));

		//load
		for(FActorIterator It(GetWorld());It;++It)
		{
			AActor* Actor=*It;

			if(!Actor->Implements<USInteractInterface>())
			{
				continue;
			}

			for(FActorSaveData ActorData:CurrentSaveGame->SavedActors)
			{
				if(ActorData.ActorName==Actor->GetName())
				{
					//GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Blue,ActorData.ActorName);
					Actor->SetActorTransform(ActorData.Transform);
					
					FMemoryReader MemReader(ActorData.ByteData);
					
					FObjectAndNameAsStringProxyArchive Ar(MemReader,true);
					Ar.ArIsSaveGame=true;
					//convert  binary array into var
					Actor->Serialize(Ar);

					ISInteractInterface::Execute_OnActorLoad(Actor);
					
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame=Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		
		UE_LOG(LogTemp,Warning,TEXT("Created new SameGame data!"));
	}

	
}


