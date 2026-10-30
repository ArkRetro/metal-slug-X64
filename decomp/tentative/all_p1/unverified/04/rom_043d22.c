/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x043d22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_43d22(void)

{
  short sVar1;
  short sVar2;
  int in_A0;
  int unaff_A6;
  undefined8 uVar3;
  short sStack_6;

  sVar1 = *(short *)(in_A0 + 0x3c);
  sVar2 = *(short *)(unaff_A6 + 0x3c);
  candidate_2f1a();
  uVar3 = candidate_2e7c();
  return CONCAT44(CONCAT22((short)(char)((ulonglong)uVar3 >> 0x38),
                           *(short *)(unaff_A6 + 0x28) +
                           ((sVar1 - sVar2) - (short)((ulonglong)uVar3 >> 0x28))),
                  CONCAT22((short)(char)((ulonglong)uVar3 >> 0x18),
                           *(short *)(unaff_A6 + 0x2c) + (sStack_6 - (short)((ulonglong)uVar3 >> 8))
                          ));
}
