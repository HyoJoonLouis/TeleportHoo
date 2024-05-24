#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IngamePlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadDelegate, AIngamePlayerController*, DeadCharacter);

UCLASS()
class TELEPORTHOO_API AIngamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UIngameHUD> HUDClass;
	
	UPROPERTY()
	class UIngameHUD* HUD;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	ETeam PlayerTeam;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Server, Unreliable)
	void Server_SendChat(const FText& TextToSend);
	UFUNCTION(Client, Unreliable)
	void Client_SendChat(const FText& Name, const FText& TextToSend);
	UFUNCTION(Client, Reliable)
	void Client_UpdateScore();

	// Delegates
	FOnDeadDelegate OnDeadDelegate;

	// Setters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetControllerTeam(ETeam NewTeam) { PlayerTeam = NewTeam; }

	// Getters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UIngameHUD* GetIngameHUD() { return HUD; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETeam GetControllerTeam() { return PlayerTeam; }
};
