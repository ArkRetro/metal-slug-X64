/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a7de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a7de(void)

{
  undefined4 uVar1;
  ushort uVar2;
  undefined4 *unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x1c) = 0;
  *unaff_A6 = 0x3a7e8;
  uVar2 = *(short *)(unaff_A6 + 0x1c) + 1;
  *(ushort *)(unaff_A6 + 0x1c) = uVar2;
  if (uVar2 < 0x19) {
    *unaff_A6 = 0x3a768;
  }
  uVar1 = unaff_A6[0xb];
  unaff_A6[0xb] = *(short *)((short)((uVar2 & 1) << 7) + 0x29fc) * 0x100 + unaff_A6[0xb];
  candidate_1abc8();
  candidate_427a();
  unaff_A6[0xb] = uVar1;
  return;
}
