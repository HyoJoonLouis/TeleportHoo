// Fill out your copyright notice in the Description page of Project Settings.


#include "CinemaWidget.h"

void UCinemaWidget::PlayCinemaAnimation(bool bStart)
{
	bStart ? PlayAnimationForward(StartCinema) : PlayAnimationReverse(StartCinema);
}
