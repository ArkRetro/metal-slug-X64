/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x046d4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_46d4e(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  if (*(char *)(*(int *)((int)unaff_A6 + 0x6e) + 0x81) == '\0') {
    *(undefined1 *)(unaff_A6[3] + 0x8f) = 1;
    *unaff_A6 = 0x46d6a;
  }
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
