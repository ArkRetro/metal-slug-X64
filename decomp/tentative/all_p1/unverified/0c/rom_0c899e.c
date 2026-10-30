/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c899e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c899e(void)

{
  int iVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  char *in_A1;
  int unaff_A6;

  iVar1 = *(int *)(unaff_A6 + 0x38);
  if ((iVar1 != 0x10c72e) && (iVar1 != 0x10d7b6)) {
    return;
  }
  sVar5 = (short)(-0x40 - *(short *)(iVar1 + 0x14)) >> 3;
  sVar2 = (short)*in_A1 + (*(short *)(unaff_A6 + 0x28) >> 3);
  sVar3 = in_A1[1] * 2 + (*(short *)(unaff_A6 + 0x2c) >> 3);
  sVar4 = ((short)(0x80 - *(short *)(iVar1 + 0x18)) >> 3) + 0x3f;
  sVar6 = -sVar4;
  if (sVar2 < sVar5) {
    return;
  }
  if ((short)(sVar5 + 0x40) <= (short)(in_A1[2] + sVar2)) {
    return;
  }
  if (sVar3 != sVar4 && SBORROW2(sVar6,-sVar3) == (short)(sVar6 + sVar3) < 0) {
    return;
  }
  if ((short)(sVar6 + 0x40) <= (short)((short)in_A1[3] + -sVar3)) {
    return;
  }
  return;
}
