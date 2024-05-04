// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBox.generated.h"

/**
 * 
 */
UCLASS()
class TELEPORTHOO_API UChatBox : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatEditText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat")
	TSubclassOf<class UChat> ChatClass;

public:
	virtual void NativeConstruct() override;

	void AddChat(const FText& Name, const FText& TextToSend);

	UFUNCTION()
	void ChatOnCommitted(const FText& Text, ETextCommit::Type CommitMethod);
};
