#include "TPSHelper.h"

#include "TPSSystemManager.h"
#include "Components/TPSAttributeComponent.h"
#include "Game/TPSGameInstance.h"
#include "Game/TPSGameMode.h"
#include "Kismet/GameplayStatics.h"

TWeakObjectPtr<UTPSGameInstance> TPSHelper::GameInstance = nullptr;

UWorld* TPSHelper::GetWorld()
{
	UWorld* World = UTPSSystemManager::Get() == nullptr ? nullptr : UTPSSystemManager::Get()->GetWorld();
	if (World == nullptr && GEngine->GameViewport != nullptr)
	{
		World = GEngine->GameViewport->GetWorld();
	}

	return World;
}

UTPSGameInstance* TPSHelper::GetGameInstance()
{
	if (GameInstance.IsValid())
	{
		return GameInstance.Get();
	}
	return nullptr;
}


APlayerController* TPSHelper::GetPlayerController(UWorld* InWorld)
{
	if (InWorld != nullptr)
	{
		return UGameplayStatics::GetPlayerController(InWorld, 0);
	}
	return UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

ULocalPlayer* TPSHelper::GetLocalPlayer(UWorld* InWorld)
{
	if (APlayerController* PlayerController = GetPlayerController(InWorld))
	{
		return PlayerController->GetLocalPlayer();
	}
	return nullptr;
}

APlayerCameraManager* TPSHelper::GetPlayeCameraManager(UWorld* InWorld)
{
	APlayerController* PlayerController = GetPlayerController(InWorld);
	if (PlayerController != nullptr)
	{
		return PlayerController->PlayerCameraManager;
	}
	return nullptr;
}

APlayerController* TPSHelper::GetFirstLocalPlayerController(UWorld* InWorld)
{
	if (InWorld == nullptr)
	{
		return nullptr;
	}
	APlayerController* playerController = Cast<APlayerController>(
		(GetWorld() && GEngine) ? GEngine->GetFirstLocalPlayerController(GetWorld()) : nullptr);
	return playerController;
}

bool TPSHelper::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	if (UTPSAttributeComponent* AC = UTPSAttributeComponent::GetAttributes(TargetActor))
	{
		return AC->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool TPSHelper::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser,TargetActor,DamageAmount) == true)
	{
		if (UPrimitiveComponent* HitComp = HitResult.GetComponent(); HitComp->IsSimulatingPhysics(HitResult.BoneName) == true)
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction* 300000.f, HitResult.Location,HitResult.BoneName);
		}
		return true;
	}
	return false;
}
