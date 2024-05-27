#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class TELEPORTHOO_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	//FUNCTION
	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchIsWaitingToStart() override;
	virtual bool IsMatchInProgress() const override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleLeavingMap() override;
	virtual void HandleMatchAborted() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	UFUNCTION(NetMulticast, Reliable)
	void OnPlayerInfoUpdated();

protected:
	// UPROPERTY(VisibleAnywhere)
	// TArray<class ALobbyPlayerController*> ConnectedPlayers;
	
	
	// IncomePlayer의 OnDeadDelegate 델리게이트에 AInGameGameMode::OnPlayerDiedDelegate 함수를 바인딩합니다.
	// 이로 인해 플레이어가 사망할 때 OnPlayerDiedDelegate 함수가 호출됩니다.
	// AddUniqueDynamic 메서드는 델리게이트에 중복 항목을 허용하지 않습니다.
	
};
