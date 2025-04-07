#include "TPSGameMode.h"

#include "AISpawnSubSystem.h"
#include "TPSGameManager.h"
#include "TPSSystemManager.h"
#include "Character/TPSAIController.h"
#include "Character/TPSPlayer.h"
#include "Character/TPSPlayerState.h"
#include "Game/AISpawnSubSystem.h"

ATPSGameMode::ATPSGameMode()
{
}

void ATPSGameMode::StartPlay()
{
	Super::StartPlay();

	UTPSSystemManager::Get()->SetWorld(GetWorld());

	UTPSSystemManager::Get()->GetGameManager()->InitData(this); 
	
	UTPSSystemManager::Get()->BeginPlay();

	UTPSSystemManager::Get()->SetGameState(GameplayState);
}

void ATPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	AISpawnSubSystem = GetWorld()->GetSubsystem<UAISpawnSubSystem>();
	if (AISpawnSubSystem != nullptr)
	{
		AISpawnSubSystem->StartSpawnAI();
	}

	OnDifficultyUpdate.AddUObject(AISpawnSubSystem.Get(), &UAISpawnSubSystem::OnDifficultyChanged);
}

void ATPSGameMode::BeginDestroy()
{	
	Super::BeginDestroy();
	UE_LOG(LogTemp, Log, TEXT("Begin Destroy GameMode!!"));
}

void ATPSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UTPSSystemManager::Get()->EndPlay(EndPlayReason);
	Super::EndPlay(EndPlayReason);
}

void ATPSGameMode::OnActorKilled(AActor* KilledActor, AActor* InstigatorActor)
{
	// UE_LOG(LogTemp, Log, TEXT("KilledActor Actor : %s InstigatorActor : %s"), *GetNameSafe(KilledActor),
	//        *InstigatorActor->GetName());
	
	if (const ATPSPlayer* TPSPlayer = Cast<ATPSPlayer>(KilledActor))
	{
		for (ATPSCharacter* SpawnedCharacter : UTPSSystemManager::Get()->GetGameManager()->GetAllCharacters())
		{
			if (ATPSAIController* AIController = Cast<ATPSAIController>(SpawnedCharacter->GetController()))
			{
				//AIController->StopAI();
			}
		}
	}
	else
	{
		if ( ATPSCharacter* AICharacter = Cast<ATPSCharacter>(KilledActor))
		{
			if (ATPSAIController* AIController = Cast<ATPSAIController>(AICharacter->GetController()))
			{
				AIController->StopAI();
			}			
		}
		
		TPSPlayer = Cast<ATPSPlayer>(InstigatorActor);
		if (TPSPlayer != nullptr)
		{
			if(ATPSPlayerState* PS = Cast<ATPSPlayerState>(TPSPlayer->GetPlayerState()))
			{
				PS->AddKillCount();
			}			
		}
	}
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda(
		[KilledActor, this]()
		{
			UTPSGameManager* GameManager = UTPSSystemManager::Get()->GetGameManager();
			
			ATPSCharacter* DieCharacter = Cast<ATPSCharacter>(KilledActor);
			ATPSPlayer* Player = Cast<ATPSPlayer>(DieCharacter);
			
			if (Player != nullptr)
			{
				if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
				{
					Player->UninitAndDestroy();
					PC->UnPossess();
					GameManager->SpawnPlayer(TEXT("Player"),0);
					
					for (ATPSCharacter* SpawnedCharacter : UTPSSystemManager::Get()->GetGameManager()->GetAllCharacters())
					{
						if (ATPSAIController* AIController = Cast<ATPSAIController>(SpawnedCharacter->GetController()))
						{
							AIController->RunAI(GameManager->GetPlayer());
						}
					}
				}
			}
			else
			{
				AISpawnSubSystem->DespawnCharacter(DieCharacter);
				GameManager->DespawnCharacter(DieCharacter);
				DieCharacter->UninitAndDestroy();
			}
		});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,5,false);
}
