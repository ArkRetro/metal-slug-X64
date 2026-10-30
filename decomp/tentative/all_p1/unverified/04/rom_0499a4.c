/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0499a4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_499a4(void)

{
  short sVar1;
  ushort uVar2;
  int unaff_A6;
  undefined8 uVar3;

  sVar1 = candidate_2eb4();
  uVar2 = *(ushort *)(unaff_A6 + 0x74) & 0x1f;
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    uVar2 = -uVar2;
  }
  *(ushort *)(unaff_A6 + 0x82) = uVar2 + sVar1 & 0xff;
  candidate_1b29c();
  uVar3 = candidate_2e7c();
  *(int *)(unaff_A6 + 0x30) =
       ((int)((ulonglong)uVar3 >> 0x20) - *(int *)(unaff_A6 + 0x30) >> 3) +
       *(int *)(unaff_A6 + 0x30);
  *(int *)(unaff_A6 + 0x34) =
       ((int)uVar3 - *(int *)(unaff_A6 + 0x34) >> 4) + *(int *)(unaff_A6 + 0x34);
  return;
}
