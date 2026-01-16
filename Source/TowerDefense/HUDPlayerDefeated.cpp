#include "HUDPlayerDefeated.h"
#include "Components/TextBlock.h"

void UHUDPlayerDefeated::StartCountdown(int time)
{
	countdownTime = time;

	FText formatText = FText::FromString(TEXT("Respawn Time - {time}"));
	FFormatNamedArguments args;
	args.Add(TEXT("time"), countdownTime);

	TextTimer->SetText(FText::Format(formatText, args));
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UHUDPlayerDefeated::UpdateCountdownText, 1, true);
}

void UHUDPlayerDefeated::UpdateCountdownText()
{
	countdownTime--;

	FText formatText = FText::FromString(TEXT("Respawn Time - {time}"));
	FFormatNamedArguments args;
	args.Add(TEXT("time"), countdownTime);

	TextTimer->SetText(FText::Format(formatText, args));
}

void UHUDPlayerDefeated::EndCountdown()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
}
