// Fill out your copyright notice in the Description page of Project Settings.


#include "TutoTools.h"

TArray<bool> UTutoTools::GetPasswordStrength(const FString& Password)
{
    int32 Length = Password.Len();
    int32 tqt = 0;
    bool bHasLower = false, bHasUpper = false;
    bool bHasDigit = false, bHasSpecialChar = false;
    bool bGreaterThan8 = Length >= 8;
    FString NormalChars = TEXT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ");

    for (int32 i = 0; i < Length; i++)
    {
        if (!bHasLower) bHasLower = FChar::IsLower(Password[i]);
        if (!bHasUpper) bHasUpper = FChar::IsUpper(Password[i]);
        if (!bHasDigit) bHasDigit = FChar::IsDigit(Password[i]);
        if (!bHasSpecialChar) bHasSpecialChar = !NormalChars.FindChar(Password[i], tqt);
    }

    TArray<bool> flags = { bHasLower, bHasUpper, bHasSpecialChar, bHasDigit, bGreaterThan8 };

    int32 nbFlagValid = 0;
    for (bool flag : flags)
    {
        if (flag) nbFlagValid += 1;
    }
    return flags;
}

void UTutoTools::SavePassword(const FString& Password)
{
    password = Password;
}

bool UTutoTools::IsEqualToOldPassword(const FString& Password)
{
    return password == Password;
}
