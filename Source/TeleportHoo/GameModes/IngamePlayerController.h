#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IngamePlayerController.generated.h"

UCLASS()
class TELEPORTHOO_API AIngamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UIngameHUD> HUDClass;
	
	UPROPERTY()
	class UIngameHUD* HUD;

public:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Unreliable)
	void Server_SendChat(const FText& TextToSend);
	UFUNCTION(Client, Unreliable)
	void Client_SendChat(const FText& Name, const FText& TextToSend);

	// Getters
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UIngameHUD* GetIngameHUD() { return HUD; }
};
