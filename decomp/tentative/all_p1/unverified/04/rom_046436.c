/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046436.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_46436(void)

{
  ushort uVar1;
  short sVar2;
  int unaff_A6;

  uVar1 = candidate_29c6();
  *(int *)(unaff_A6 + 0x30) = (int)(short)((uVar1 & 0xff) + 0x181) << 8;
  sVar2 = candidate_29f2();
  if (0x9f < *(short *)(unaff_A6 + 0x3c)) {
    sVar2 = -sVar2;
  }
  *(short *)(unaff_A6 + 0x8e) = sVar2 + 0xa0;
  if ((short)(sVar2 + 0xa0) <= *(short *)(unaff_A6 + 0x3c)) {
    *(int *)(unaff_A6 + 0x30) = -*(int *)(unaff_A6 + 0x30);
  }
  return;
}
