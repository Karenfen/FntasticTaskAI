// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"


bool UMyAnimInstance::HandleNotify(const FAnimNotifyEvent& AnimNotifyEvent) {
    if (OnPlaySound.IsBound()) {
        OnPlaySound.Broadcast(AnimNotifyEvent.NotifyName);
    }

    return Super::HandleNotify(AnimNotifyEvent);
}