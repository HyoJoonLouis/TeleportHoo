#include "IngamePlayerController.h"
#include "../UI/IngameHUD.h"
#include "../UI/ChatBox.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "Net/UnrealNetwork.h"


void AIngamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		HUD = Cast<UIngameHUD>(CreateWidget(this, HUDClass));
		HUD->AddToViewport();
	}

	PlayerSequence();
}

void AIngamePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsInputKeyDown(EKeys::Enter))
	{
		SetShowMouseCursor(true);
		SetInputMode(FInputModeUIOnly());
		GetIngameHUD()->GetChatBox()->SetFocus();
	}
}

void AIngamePlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngamePlayerController, PlayerTeam);
}

void AIngamePlayerController::Client_ShowRoundResult_Implementation(int Round, bool isWin)
{
	GetIngameHUD()->ShowResult(Round, isWin);
}

void AIngamePlayerController::OnLevelSequenceEnd()
{
	SetViewTargetWithBlend(GetPawn());
}

void AIngamePlayerController::PlayerSequence()
{
	FStringAssetReference SequenceAssetRef(TEXT("/Game/Seq/All_Seq.All_Seq"));
	ULevelSequence* Sequence = Cast<ULevelSequence>(SequenceAssetRef.TryLoad());

	if (Sequence)
	{
		FActorSpawnParameters SpawnParams;
		ALevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), SpawnParams);

		if (SequenceActor)
		{
			ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, FMovieSceneSequencePlaybackSettings(), SequenceActor);

			if (SequencePlayer)
			{
				SequencePlayer->Play();
				SequencePlayer->OnFinished.AddDynamic(this, &AIngamePlayerController::OnSequenceFinished);
			}
		}
	}
}

void AIngamePlayerController::OnSequenceFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Sequence finished"));
}

void AIngamePlayerController::Server_SendChat_Implementation(const FText& TextToSend)
{
	TArray<TObjectPtr<APlayerState>> PlayerStates = UGameplayStatics::GetGameState(GetWorld())->PlayerArray;
	for (const auto& PlayerStateForChat : PlayerStates)
	{
		Cast<AIngamePlayerController>(PlayerStateForChat->GetPlayerController())->Client_SendChat(FText::FromString(PlayerStateForChat->GetName()), TextToSend);
	}
}

void AIngamePlayerController::Client_SendChat_Implementation(const FText& Name, const FText& TextToSend)
{
	if (IsValid(GetIngameHUD()))
	{
		GetIngameHUD()->SendChat(Name, TextToSend);
	}
}

void AIngamePlayerController::Client_UpdateScore_Implementation()
{
	if (IsValid(GetIngameHUD()))
	{
		GetIngameHUD()->UpdateScore();
	}
}

void AIngamePlayerController::Client_FadeInOut_Implementation(bool FadeOut)
{
	if (IsValid(GetIngameHUD()))
	{
		GetIngameHUD()->FadeInOut(FadeOut);
	}
}

void AIngamePlayerController::Client_StartLevelSequence_Implementation(ULevelSequence* LevelSequence)
{
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bDisableLookAtInput = true;
	Settings.bDisableMovementInput = true;
	Settings.bHideHud = true;

	ALevelSequenceActor* SequenceActor;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, SequenceActor);
	LevelSequencePlayer->OnFinished.AddUniqueDynamic(this, &AIngamePlayerController::OnLevelSequenceEnd);
	LevelSequencePlayer->Play();
}