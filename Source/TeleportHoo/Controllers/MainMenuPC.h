#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MainMenuPC.generated.h"

UCLASS()
class TELEPORTHOO_API AMainMenuPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainMenuPC();

protected:
	virtual void BeginPlay() override;
	
protected:
	// VARIABLES
	UPROPERTY()
	class UUserWidget* MainMenuWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	class TSubclassOf<UUserWidget> MainMenuWidgetClass;

	IOnlineSessionPtr Sessions;

public:
	// Functions
	UFUNCTION()
	void HostSession();

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UFUNCTION()
	void FindSession();

	UFUNCTION()
	void JoinSession();
	
	UFUNCTION()
	void OnFindSessionsComplete(bool bWasSuccessful);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};