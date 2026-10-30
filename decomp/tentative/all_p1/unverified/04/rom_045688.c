/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x045688.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_45688(void)

{
  short *psVar1;
  undefined4 *unaff_A6;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0xe00;
  *unaff_A6 = 0x4569e;
  unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x76) + unaff_A6[0xd];
  candidate_1ac38();
  candidate_427a();
  if (0xff < (ushort)(*(short *)(unaff_A6 + 0x10) - 0x100U)) {
    *unaff_A6 = 0x456c8;
  }
  psVar1 = *(short **)((int)unaff_A6 + 0x86);
  if (*(short *)(unaff_A6[3] + 0x8a) == *psVar1) {
    *(char *)(unaff_A6 + 0x23) = (char)*psVar1;
    *unaff_A6 = *(undefined4 *)(psVar1 + 1);
    *(short **)((int)unaff_A6 + 0x86) = psVar1 + 3;
  }
  return;
}
