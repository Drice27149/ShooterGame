// Drice

#include "UI/MatchInfoWidget.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Containers/UnrealString.h"
#include "GameFramework/Actor.h"

void UMatchInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    SetNameText(FString("Player"));
    SetScoreText(FString("0"));
}

void UMatchInfoWidget::SetNameText(FString Name)
{
    if(ShooterName != NULL)
    {
        ShooterName->SetText(FText::FromString(Name));
    }
}
    
void UMatchInfoWidget::SetScoreText(FString Score)
{
    if(ShooterScore != NULL)
    {
        ShooterScore->SetText(FText::FromString(Score));
    }
}

void UMatchInfoWidget::SetMessageText(FString Message)
{
    if(MatchMessage != NULL){
        MatchMessage->SetText(FText::FromString(Message));
    }
}

void UMatchInfoWidget::SetRemainTimeText(FString RemainTime)
{
    if(MatchRemainTime != NULL)
    {
        MatchRemainTime->SetText(FText::FromString(RemainTime));
    }
}
