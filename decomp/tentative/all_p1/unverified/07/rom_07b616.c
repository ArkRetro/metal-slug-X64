/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07b616.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7b616(void)

{
  int iVar1;
  int unaff_A6;
  undefined8 uVar2;

  if (*(short *)(unaff_A6 + 0x28) < 0xaae) {
    candidate_2eb4();
    iVar1 = *(int *)(unaff_A6 + 0x76) + 0x4000;
    if (0x17fff < iVar1) {
      iVar1 = 0x18000;
    }
    *(int *)(unaff_A6 + 0x76) = iVar1;
    uVar2 = candidate_2e7c();
    *(undefined8 *)(unaff_A6 + 0x30) = uVar2;
  }
  else {
    *(undefined4 *)(unaff_A6 + 0x30) = 0;
    *(undefined4 *)(unaff_A6 + 0x34) = 0;
    *(undefined4 *)(unaff_A6 + 0x76) = 0;
  }
  return;
}
