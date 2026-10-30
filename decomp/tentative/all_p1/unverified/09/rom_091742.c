/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x091742.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_91742(void)

{
  byte bVar1;
  int unaff_A6;
  undefined8 uVar2;

  if (((*(byte *)(unaff_A6 + 0x96) & 2) != 0) && (*(int *)(unaff_A6 + 0x76) != 0)) {
    candidate_3b332();
    candidate_91638();
    candidate_9160a();
    uVar2 = candidate_3b332();
    *(int *)(unaff_A6 + 0x30) = (int)((ulonglong)uVar2 >> 0x20);
    *(int *)(unaff_A6 + 0x34) = (int)uVar2 << 1;
    candidate_91638();
    bVar1 = candidate_916c4();
    if (*(short *)(*(int *)(unaff_A6 + 0x76) + 0x40) < *(short *)(unaff_A6 + 0x40)) {
      bVar1 = 0x10 - bVar1 & 0xf;
    }
    *(byte *)(unaff_A6 + 0x8f) = bVar1;
    return;
  }
  return;
}
