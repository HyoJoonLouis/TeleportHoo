
#include "RoundResultWidget.h"
#include "Components/TextBlock.h"

void URoundResultWidget::ShowResult(int Round, bool isWin)
{
	RoundText->SetText(FText::FromString(FString::Printf(TEXT("%i 라운드"),Round)));
	ResultText->SetText(FText::FromString(isWin ? TEXT("승리") : TEXT("패배")));
}
